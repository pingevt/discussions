<?php

/**
 * @file
 * EntityFieldQueryByGroup.inc
 */

class EntityFieldQueryByGroup extends EntityFieldQuery {
  public $groupConditions = array();

  public function __construct(array $group_ids) {
    $this->groupConditions = $group_ids;
  }

  protected function propertyQuery() {
    if (empty($this->entityConditions['entity_type'])) {
      throw new EntityFieldQueryException(t('For this query an entity type must be specified.'));
    }
    $entity_type = $this->entityConditions['entity_type']['value'];
    $entity_info = entity_get_info($entity_type);
    if (empty($entity_info['base table'])) {
      throw new EntityFieldQueryException(t('Entity %entity has no base table.', array('%entity' => $entity_type)));
    }
    $base_table = $entity_info['base table'];
    $base_table_schema = drupal_get_schema($base_table);
    $select_query = db_select($base_table);

    // Add in join for group
    $glue = $base_table . '.' . $entity_info['entity keys']['id'] . ' = dgc.eid ';
    $select_query->join('discussion_group_content', 'dgc', $glue);
    $select_query->condition('dgc.entity_type', $entity_type);
    $select_query->condition('dgc.gid', $this->groupConditions, 'IN');

    $select_query->addExpression(':entity_type', 'entity_type', array(':entity_type' => $entity_type));
    // Process the property conditions.
    foreach ($this->propertyConditions as $property_condition) {
      $this->addCondition($select_query, $base_table . '.' . $property_condition['column'], $property_condition);
    }
    // Process the four possible entity condition.
    // The id field is always present in entity keys.
    $sql_field = $entity_info['entity keys']['id'];
    $id_map['entity_id'] = $sql_field;
    $select_query->addField($base_table, $sql_field, 'entity_id');
    if (isset($this->entityConditions['entity_id'])) {
      $this->addCondition($select_query, $base_table . '.' . $sql_field, $this->entityConditions['entity_id']);
    }

    // If there is a revision key defined, use it.
    if (!empty($entity_info['entity keys']['revision'])) {
      $sql_field = $entity_info['entity keys']['revision'];
      $select_query->addField($base_table, $sql_field, 'revision_id');
      if (isset($this->entityConditions['revision_id'])) {
        $this->addCondition($select_query, $base_table . '.' . $sql_field, $this->entityConditions['revision_id']);
      }
    }
    else {
      $sql_field = 'revision_id';
      $select_query->addExpression('NULL', 'revision_id');
    }
    $id_map['revision_id'] = $sql_field;

    // Handle bundles.
    if (!empty($entity_info['entity keys']['bundle'])) {
      $sql_field = $entity_info['entity keys']['bundle'];
      $having = FALSE;

      if (!empty($base_table_schema['fields'][$sql_field])) {
        $select_query->addField($base_table, $sql_field, 'bundle');
      }
    }
    else {
      $sql_field = 'bundle';
      $select_query->addExpression(':bundle', 'bundle', array(':bundle' => $entity_type));
      $having = TRUE;
    }
    $id_map['bundle'] = $sql_field;
    if (isset($this->entityConditions['bundle'])) {
      if (!empty($entity_info['entity keys']['bundle'])) {
        $this->addCondition($select_query, $base_table . '.' . $sql_field, $this->entityConditions['bundle'], $having);
      }
      else {
        // This entity has no bundle, so invalidate the query.
        $select_query->where('1 = 0');
      }
    }

    // Order the query.
    foreach ($this->order as $order) {
      if ($order['type'] == 'entity') {
        $key = $order['specifier'];
        if (!isset($id_map[$key])) {
          throw new EntityFieldQueryException(t('Do not know how to order on @key for @entity_type', array('@key' => $key, '@entity_type' => $entity_type)));
        }
        $select_query->orderBy($id_map[$key], $order['direction']);
      }
      elseif ($order['type'] == 'property') {
        $select_query->orderBy($base_table . '.' . $order['specifier'], $order['direction']);
      }
    }
    return $this->finishQuery($select_query);
  }

  public function queryCallback() {
    // Use the override from $this->executeCallback. It can be set either
    // while building the query, or using hook_entity_query_alter().
    if (function_exists($this->executeCallback)) {
      return $this->executeCallback;
    }
    // If there are no field conditions and sorts, and no execute callback
    // then we default to querying entity tables in SQL.
    if (empty($this->fields)) {
      return array($this, 'propertyQuery');
    }
    // If no override, find the storage engine to be used.
    foreach ($this->fields as $field) {
      if (!isset($storage)) {
        $storage = $field['storage']['module'];
      }
      elseif ($storage != $field['storage']['module']) {
        throw new EntityFieldQueryException(t("Can't handle more than one field storage engine"));
      }
    }
    if ($storage) {
      // Use hook_field_storage_query() from the field storage.
      return $storage . '_field_storage_query_domain';
    }
    else {
      throw new EntityFieldQueryException(t("Field storage engine not found."));
    }
  }
}


/**
 * Implements hook_field_storage_query().
 */
function field_sql_storage_field_storage_query_domain(EntityFieldQuery $query) {
  if ($query->age == FIELD_LOAD_CURRENT) {
    $tablename_function = '_field_sql_storage_tablename';
    $id_key = 'entity_id';
  }
  else {
    $tablename_function = '_field_sql_storage_revision_tablename';
    $id_key = 'revision_id';
  }
  $table_aliases = array();
  // Add tables for the fields used.
  foreach ($query->fields as $key => $field) {
    $tablename = $tablename_function($field);
    // Every field needs a new table.
    $table_alias = $tablename . $key;
    $table_aliases[$key] = $table_alias;
    if ($key) {
      $select_query->join($tablename, $table_alias, "$table_alias.entity_type = $field_base_table.entity_type AND $table_alias.$id_key = $field_base_table.$id_key");
    }
    else {
      $select_query = db_select($tablename, $table_alias);
      // Allow queries internal to the Field API to opt out of the access
      // check, for situations where the query's results should not depend on
      // the access grants for the current user.
      if (!isset($query->tags['DANGEROUS_ACCESS_CHECK_OPT_OUT'])) {
        $select_query->addTag('entity_field_access');
      }
      $select_query->addMetaData('base_table', $tablename);
      $select_query->fields($table_alias, array('entity_type', 'entity_id', 'revision_id', 'bundle'));
      $field_base_table = $table_alias;

      // Add in join for group
      $entity_type = $query->entityConditions['entity_type']['value'];
      $glue = $table_alias . '.entity_id = dgc.eid ';
      $select_query->join('discussion_group_content', 'dgc', $glue);
      $select_query->condition('dgc.entity_type', $entity_type);
      $select_query->condition('dgc.gid', $query->groupConditions, 'IN');

    }
    if ($field['cardinality'] != 1 || $field['translatable']) {
      $select_query->distinct();
    }
  }

  // Add field conditions. We need a fresh grouping cache.
  drupal_static_reset('_field_sql_storage_query_field_conditions');
  _field_sql_storage_query_field_conditions($query, $select_query, $query->fieldConditions, $table_aliases, '_field_sql_storage_columnname');

  // Add field meta conditions.
  _field_sql_storage_query_field_conditions($query, $select_query, $query->fieldMetaConditions, $table_aliases, '_field_sql_storage_query_columnname');

  if (isset($query->deleted)) {
    $select_query->condition("$field_base_table.deleted", (int) $query->deleted);
  }

  // Is there a need to sort the query by property?
  $has_property_order = FALSE;
  foreach ($query->order as $order) {
    if ($order['type'] == 'property') {
      $has_property_order = TRUE;
    }
  }

  if ($query->propertyConditions || $has_property_order) {
    if (empty($query->entityConditions['entity_type']['value'])) {
      throw new EntityFieldQueryException('Property conditions and orders must have an entity type defined.');
    }
    $entity_type = $query->entityConditions['entity_type']['value'];
    $entity_base_table = _field_sql_storage_query_join_entity($select_query, $entity_type, $field_base_table);
    $query->entityConditions['entity_type']['operator'] = '=';
    foreach ($query->propertyConditions as $property_condition) {
      $query->addCondition($select_query, "$entity_base_table." . $property_condition['column'], $property_condition);
    }
  }
  foreach ($query->entityConditions as $key => $condition) {
    $query->addCondition($select_query, "$field_base_table.$key", $condition);
  }

  // Order the query.
  foreach ($query->order as $order) {
    if ($order['type'] == 'entity') {
      $key = $order['specifier'];
      $select_query->orderBy("$field_base_table.$key", $order['direction']);
    }
    elseif ($order['type'] == 'field') {
      $specifier = $order['specifier'];
      $field = $specifier['field'];
      $table_alias = $table_aliases[$specifier['index']];
      $sql_field = "$table_alias." . _field_sql_storage_columnname($field['field_name'], $specifier['column']);
      $select_query->orderBy($sql_field, $order['direction']);
    }
    elseif ($order['type'] == 'property') {
      $select_query->orderBy("$entity_base_table." . $order['specifier'], $order['direction']);
    }
  }
  return $query->finishQuery($select_query, $id_key);
}
