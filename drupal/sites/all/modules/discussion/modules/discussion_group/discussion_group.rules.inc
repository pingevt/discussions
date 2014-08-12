<?php
/**
 * @file
 * discussion_group.rules.inc
 */

/**
 * Implements hook_rules_event_info()
 */
function discussion_group_rules_event_info() {
  return array(
    'discussion_group_node_insert_after_groups' => array(
      'label' => t('Node insert, after groups update'),
      'module' => 'discussion_group',
      'group' => t('Discussion Groups'),
      'variables' => array(
        'node' => array('type' => 'node', 'label' => t('Node')),
      ),
    ),
    'discussion_group_node_edit_after_groups' => array(
      'label' => t('Node edit, after groups update'),
      'module' => 'discussion_group',
      'group' => t('Discussion Groups'),
      'variables' => array(
        'node' => array('type' => 'node', 'label' => t('Node')),
      ),
    ),
  );
}

/**
 * Implements hook_rules_action_info()
 */
function discussion_group_rules_action_info() {
  $actions = array(
    'discussion_group_list_users_in_group' => array(
      'label' => t('List of users in groups content belongs to.'),
      'group' => t('Discussion Groups'),
      'parameter' => array(
        // The content we are sending an email for.
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
          'description' => t('The node we are sending an email for.'),
        ),
      ),
      'provides' => array(
        'users' => array(
          'label' => t('Users'),
          'type' => 'list<user>',
          'save' => FALSE,
        ),
      ),
    ),
  );

  return $actions;
}

function discussion_group_list_users_in_group($node) {
  $users = array();

  $groups = GroupContent::getNodeGroupOwners($node);
  foreach ($groups as $group) {
    $memberships = $group->getGroupMembers();

    foreach ($memberships as $membership) {
      if (isset($membership->_user->uid)) {
        $users[$membership->_user->uid] = $membership->_user;
      }
    }
  }

  return array('users' => $users);
}
