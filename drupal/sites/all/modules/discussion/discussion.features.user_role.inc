<?php
/**
 * @file
 * discussion.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function discussion_user_default_roles() {
  $roles = array();

  // Exported role: Group Member.
  $roles['Group Member'] = array(
    'name' => 'Group Member',
    'weight' => 4,
  );

  // Exported role: Group Owner.
  $roles['Group Owner'] = array(
    'name' => 'Group Owner',
    'weight' => 3,
  );

  // Exported role: Site Admin.
  $roles['Site Admin'] = array(
    'name' => 'Site Admin',
    'weight' => 2,
  );

  return $roles;
}
