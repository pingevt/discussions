<?php
/**
 * @file
 * discussion.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function discussion_user_default_roles() {
  $roles = array();

  // Exported role: Group Owner.
  $roles['Group Owner'] = array(
    'name' => 'Group Owner',
    'weight' => 4,
  );

  // Exported role: Site Admin.
  $roles['Site Admin'] = array(
    'name' => 'Site Admin',
    'weight' => 3,
  );

  return $roles;
}
