<?php

class Group extends MongoEntity {
  /**
   * {@inheritdoc}
   */
  function __construct(array $values, $entity_type = NULL) {
    parent::__construct($values, $entity_type);
    if (!isset($this->_bundle)) {
      $this->_bundle = 'group';
    }
  }

  public static function getCurrentUserGroups() {
    global $user;

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'group_membership');
    $query->propertyCondition('uid', $user->uid);
    $query->propertyOrderBy('group_name', 'ASC');

    $result = $query->execute();
    $group_memberships = array();
    if (isset($result['group_membership'])) {
      $group_memberships = entity_load('group_membership', array_keys($result['group_membership']));
    }

    $groups = array();
    foreach ($group_memberships as $gm) {
      $groups[$gm->gid] = entity_load_single('group', $gm->gid);
    }

    return $groups;
  }
}

class GroupController extends MongoEntityController {
  /**
   * {@inheritdoc}
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'group_membership');
    $query->propertyCondition('gid', $entity->_id);
    $query->propertyCondition('grid', 10);

    $result = $query->execute();
    $save_new_owner = TRUE;
    if (isset($result['group_membership'])) {
      $group_memberships = entity_load('group_membership', array_keys($result['group_membership']));
      $save_new_owner = FALSE;
      foreach ($group_memberships as $gm) {
        if ($gm->uid != $entity->field_owner[LANGUAGE_NONE][0]['target_id']) {
          $gm->delete();

          $save_new_owner = TRUE;
        }
      }
    }

    if ($save_new_owner) {
      $gm = entity_create('group_membership', array());
      $gm->gid = $entity->_id;
      $gm->grid = 10;
      $gm->uid = $entity->field_owner[LANGUAGE_NONE][0]['target_id'];

      $gm->save();
    }

  }
}

class GroupUIController extends MongoEntityDefaultUIController {

}

function group_form($form, &$form_state, $entity, $op = 'edit', $entity_type = NULL) {
  $entity_info = entity_get_info($entity->entityType());
  $property_info = entity_get_property_info($entity->entityType());

  list(,,$bundle) = entity_extract_ids('group', $entity);

  if (isset($entity_info['entity keys']['label'])) {
    $label_key = $entity_info['entity keys']['label'];
    if (isset($property_info['bundles'][$bundle]['properties'][$label_key])) {
      $property = $property_info['bundles'][$bundle]['properties'][$label_key];
    }
    elseif (isset($property_info['properties'][$label_key])) {
      $property = $property_info['properties'][$label_key];
    }

    $form[$label_key] = array(
      '#type' => 'textfield',
      '#title' => isset($property['label']) ? $property['label'] : t('Name'),
      '#required' => isset($property['required']) ? $property['required'] : TRUE,
      '#default_value' => isset($entity->$label_key) ? $entity->$label_key : NULL,
    );
  }

  field_attach_form('group', $entity, $form, $form_state);
  $form['#entity'] = $entity;

  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );
  $form['#validate'] = array('group_form_validate');
  $form['#submit'] = array('group_form_submit');
  return $form;

}

function group_form_validate($form, &$form_state) {

  $entity = $form['#entity'];
  field_attach_form_validate($entity->entityType(), $entity, $form, $form_state);

}

function group_form_submit($form, &$form_state) {

  $entity = $form['#entity'];
  entity_form_submit_build_entity($entity->entityType(), $entity, $form, $form_state);
  $entity->is_revision = TRUE;
  entity_save($entity->entityType(), $entity);

  $form_state['redirect'] = 'admin/group';
}
