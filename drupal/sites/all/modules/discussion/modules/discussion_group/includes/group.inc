<?php

/*
define('GROUP_ROLE_OWNER_LABEL', 'Owner');
define('GROUP_ROLE_EDITOR_LABEL', 'Editor');
define('GROUP_ROLE_MEMBER_LABEL', 'Member');
define('GROUP_ROLE_OWNER_ID', 10);
define('GROUP_ROLE_EDITOR_ID', 5);
define('GROUP_ROLE_MEMBER_ID', 0);
*/

class Group extends Entity {

  private $group_roles = array(
    0 => 'Member',
    5 => 'Editor',
    10 => 'Owner',
  );

  /**
   * {@inheritdoc}
   */
  function __construct(array $values, $entity_type = NULL) {
    parent::__construct($values, $entity_type);
    if (!isset($this->type)) {
      $this->type = 'group';
    }
  }

  /**
   *  provides a statis function to get groups for a currently logged in user.
   */
  public static function getCurrentUserGroups() {
    global $user;

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'group_membership');
    $query->propertyCondition('uid', $user->uid);

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

  /**
   *  provides members of a group.
   */
  public function getGroupMembers() {
    $group_memberships = array();

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'group_membership');
    $query->propertyCondition('gid', $this->gid);
    $query->propertyOrderBy('grid', 'DESC');

    $result = $query->execute();
    $group_memberships = array();
    if (isset($result['group_membership'])) {
      $group_memberships = entity_load('group_membership', array_keys($result['group_membership']));


      foreach ($group_memberships as &$gm) {
        $gm->_user = user_load($gm->uid);
      }
    }

    return $group_memberships;
  }

  /**
   *  Get role label
   */
  public function getUsersRole($group_membership) {
    return $this->group_roles[intVal($group_membership->grid)];
  }

  /**
   *  Check to see if a user has membership.
   */
  public function isUserMember($uid) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'group_membership');
    $query->propertyCondition('uid', $uid);
    $query->propertyCondition('gid', $this->gid);

    $result = $query->execute();
    $group_memberships = array();
    if (isset($result['group_membership'])) {
      return TRUE;
    }

    return FALSE;
  }

  public function isOwner($user) {
    if ($this->field_owner[LANGUAGE_NONE][0]['target_id'] == $user->uid) {
      return TRUE;
    }

    return FALSE;
  }

  public function isNodeInGroup($node) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'group_content');
    $query->propertyCondition('entity_type', 'node');
    $query->propertyCondition('gid', $this->gid);
    $query->propertyCondition('eid', $node->nid);

    $results = $query->execute();

    if (isset($results['group_content'])) {
      return TRUE;
    }
    else {
      return FALSE;
    }

  }
}

class GroupController extends EntityAPIController {
  /**
   * {@inheritdoc}
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    parent::save($entity, $transaction);

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'group_membership');
    $query->propertyCondition('gid', $entity->gid);
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
      $gm->gid = $entity->gid;
      $gm->grid = 10;
      $gm->uid = $entity->field_owner[LANGUAGE_NONE][0]['target_id'];

      $gm->save();
    }

  }
}

class GroupUIController extends EntityDefaultUIController {

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

/**
 * Form functions to add a user to a group.
 */
function group_add_user_by_email_form($form, &$form_state, $group) {
  $form = array();

  $form_state['#group'] = $group;

  $form['fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Add user',
  );

  $form['fieldset']['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#description' => t('Enter and email address and the user will be added or created.'),
    '#required' => TRUE,
  );

  $form['fieldset']['actions'] = array();

  $form['fieldset']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Enroll',
  );



  return $form;
}

function group_add_user_by_email_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', 'You need to enter a valid email address');
  }
}

function group_add_user_by_email_form_submit($form, &$form_state) {
  $group = $form_state['#group'];

  // First check to see if user exists.
  if ($user = user_load_by_mail($form_state['values']['email'])) {
    // If user exists, check to see if user is alreadu in the group.
    if ($group->isUserMember($user->uid)) {
      drupal_set_message('User already in group.');
    }
    else {

      $gm = entity_create('group_membership', array());
      $gm->gid = $group->gid;
      $gm->grid = 0;
      $gm->uid = $user->uid;

      $gm->save();

      drupal_set_message('User added');
    }
  }
  // If not, create and send an email.
  else {

  }
}

function group_admin_page() {
  // TODO: add this page
  print 'hi';
}
