<?php
/**
 * @file
 * group.inc
 */

/*
define('GROUP_ROLE_OWNER_LABEL', 'Owner');
define('GROUP_ROLE_EDITOR_LABEL', 'Editor');
define('GROUP_ROLE_MEMBER_LABEL', 'Member');
define('GROUP_ROLE_OWNER_ID', 10);
define('GROUP_ROLE_EDITOR_ID', 5);
define('GROUP_ROLE_MEMBER_ID', 0);
*/

class Group extends Entity {

  public $group_roles = array(
    0 => 'Member',
    5 => 'Editor',
    10 => 'Admin',
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
    //if ($user->uid != 1) {
      $query->propertyCondition('uid', $user->uid);
    //}

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
  public function getGroupMembers($refresh = FALSE) {
    static $group_memberships = array();

    if (empty($group_memberships) || $refresh) {
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
    }

    return $group_memberships;
  }

  public function getGroupMemebrship($user) {
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'group_membership');
    $query->propertyCondition('gid', $this->gid);
    $query->propertyCondition('uid', $user->uid);
    $result = $query->execute();

    if (isset($result['group_membership'])) {
      $group_memberships = entity_load('group_membership', array_keys($result['group_membership']));

      if (count($group_memberships) > 1) {
        drupal_set_message(t('There a more than one group_memberships for this group/user'), 'error', TRUE);
        watchdog('discussion_group', 'There a more than one group_memberships for this group/user: Group: @group_id User: @user_id', array('@group_id' => $this->gid, '@user_id' => $user->uid), WATCHDOG_ERROR);
      }

      return current($group_memberships);
    }
    else {
      return FALSE;
    }

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
  public function isUserMember($uid, $refresh = FALSE) {
    $group_members_list = &drupal_static(__FUNCTION__);

    if (isset($group_members_list[$this->gid][$uid]) && !$refresh) {
      return $group_members_list[$this->gid][$uid];
    }
    else {
      $group_members_list[$this->gid][$uid] = FALSE;

      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'group_membership');
      $query->propertyCondition('uid', $uid);
      $query->propertyCondition('gid', $this->gid);

      $result = $query->execute();
      $group_memberships = array();
      if (isset($result['group_membership'])) {
        $group_members_list[$this->gid][$uid] = TRUE;
      }

      return $group_members_list[$this->gid][$uid];
    }
  }

  /**
   * This is actual owner not group role.
   */
  public function isOwner($user) {
    if ($this->field_owner[LANGUAGE_NONE][0]['target_id'] == $user->uid) {
      return TRUE;
    }

    return FALSE;
  }

  public function isNodeInGroup($node) {

    if (!isset($node->nid)) return FALSE;

    // TODO: if it is a disdcussion we need to check the parent node.

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

  public function getGroupEvents() {
    $items = array();

    $q = new EntityFieldQueryByGroup(array($this->gid));
    $q->entityCondition('entity_type', 'node');
    $q->entityCondition('bundle', 'event');
    $q->fieldOrderBy('field_dates', 'value', 'ASC');

    $r = $q->execute();

    if (isset($r['node'])) {
      $items = node_load_multiple(array_keys($r['node']));
    }

    return $items;
  }

  public function getGroupGalleries() {
    $items = array();

    $q = new EntityFieldQueryByGroup(array($this->gid));
    $q->entityCondition('entity_type', 'node');
    $q->entityCondition('bundle', 'gallery');
    $q->propertyOrderBy('title', 'ASC');

    $r = $q->execute();

    if (isset($r['node'])) {
      $items = node_load_multiple(array_keys($r['node']));
    }

    return $items;
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

  list(, , $bundle) = entity_extract_ids('group', $entity);

  if (isset($entity_info['entity keys']['label'])) {
    $label_key = $entity_info['entity keys']['label'];
    if (isset($property_info['bundles'][$bundle]['properties'][$label_key])) {
      $property = $property_info['bundles'][$bundle]['properties'][$label_key];
    }
    elseif (isset($property_info['properties'][$label_key])) {
      $property = $property_info['properties'][$label_key];
    }

    $label_key_title = isset($property['label']) ? $property['label'] : t('Name');

    $form[$label_key] = array(
      '#type' => 'textfield',
      '#title' => check_plain($label_key_title),
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

  if (!isset($form_state['values'])) {
    $step = 1;
  }
  else {
    $step = $form_state['values']['step'] + 1;
  }
  $form['step'] = array(
    '#type' => 'hidden',
    '#value' => $step,
  );


  $form_state['#group'] = !isset($form_state['#group'])? $group : $form_state['#group'];

  $form['fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Add user',
  );

  $form['fieldset']['email'] = array(
    '#type' => 'textfield',
    '#title' => 'Email',
    '#description' => t('Enter an email address and the user will be added or created.'),
    '#default_value' => isset($form_state['values']['email']) ? $form_state['values']['email'] : NULL,
    '#required' => TRUE,
  );

  switch ($step) {
  case 2:
    $form['fieldset']['username'] = array(
      '#type' => 'textfield',
      '#title' => 'Username',
      '#description' => t('Please enter a user name.'),
      '#required' => TRUE,
    );
    break;
  }



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

  if ($form_state['values']['step'] == 1) {
    // First check to see if user exists.
    if ($user = user_load_by_mail($form_state['values']['email'])) {
      // If user exists, check to see if user is alreadu in the group.
      if ($group->isUserMember($user->uid)) {
        drupal_set_message(t('User already in group.'));
      }
      else {

        $gm = entity_create('group_membership', array());
        $gm->gid = $group->gid;
        $gm->grid = 0;
        $gm->uid = $user->uid;

        $gm->save();

        drupal_set_message(t('User added'));
      }
    }
    // If not, create and send an email.
    else {
      $form_state['rebuild'] = TRUE;
    }
  }
  elseif ($form_state['values']['step'] == 2) {

    $password = user_password(8);

    $fields = array(
      'name' => $form_state['values']['username'],
      'mail' => $form_state['values']['email'],
      'pass' => $password,
      'status' => 1,
      'init' => 'email address',
      'roles' => array(
        DRUPAL_AUTHENTICATED_RID => 'authenticated user',
        // TODO: change this so it is not hard coded
        4 => 'Group Member',
      ),
    );

    //the first parameter is left blank so a new user is created
    $account = user_save('', $fields);

    // If you want to send the welcome email, use the following code

    // Manually set the password so it appears in the e-mail.
    $account->password = $fields['pass'];

    // Send the e-mail through the user module.
    drupal_mail('user', 'register_no_approval_required', $form_state['values']['email'], NULL, array('account' => $account), variable_get('site_mail', '<a href="mailto:noreply@example..com">noreply@example..com</a>'));
    drupal_set_message(t('User created. Email was sent to user with further instructions.'));

    // Add to group
    $gm = entity_create('group_membership', array());
    $gm->gid = $group->gid;
    $gm->grid = 0;
    $gm->uid = $account->uid;

    $gm->save();
  }
}

function group_admin_page() {
  // TODO: add this page
  print 'hi';
}
