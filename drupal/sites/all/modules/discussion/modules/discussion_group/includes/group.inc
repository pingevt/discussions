<?php

class Group extends MongoEntity {

}

class GroupController extends MongoEntityController {

}

class GroupUIController extends MongoEntityDefaultUIController {

}

function group_form($form, &$form_state, $entity, $op = 'edit', $entity_type = NULL) {
  $entity_info = entity_get_info($entity->entityType());
  $property_info = entity_get_property_info($entity->entityType());

  list(,,$bundle) = entity_extract_ids($entity->entityType(), $entity);

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

  field_attach_form($entity->entityType(), $entity, $form, $form_state);
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
  entity_save($entity->entityType(), $entity);

  $form_state['redirect'] = 'admin/group';
}
