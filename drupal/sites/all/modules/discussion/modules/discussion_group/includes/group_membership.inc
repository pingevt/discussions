<?php
/**
 * @file
 * group_membership.inc
 */

class GroupMembership extends Entity {

  public $gid;
  public $uid;
  public $grid;
  public $updated;
  public $created;

  /**
   * {@inheritdoc}
   */
  function __construct(array $values, $entity_type = NULL) {
    parent::__construct($values, $entity_type);
    if (!isset($this->type)) {
      $this->type = 'membership';
    }
  }

  function getGroup() {
    return entity_load_single('group', $this->gid);
  }

  function createChangeRolesList() {
    drupal_add_js(drupal_get_path('theme', 'discussions') . '/bootstrap/js/dropdown.js');
    $group = $this->getGroup();
    $group_roles = $group->group_roles;

    $str = '';
    $str .= '<div class="btn-group">
    <button type="button" class="btn btn-xs btn-default dropdown-toggle" data-toggle="dropdown">
    Change Group Role <span class="caret"></span></button>
    <ul class="dropdown-menu" role="menu">';

    foreach ($group_roles as $rid => $role) {
      $str .= '<li>' . l(t('Change to ' . $role), 'group/4/memberships/' . $this->_user->uid . '/change-role/' . $rid, array('query' => array('destination' => $_GET['q']))) . '</li>';
    }

    $str .= '</ul></div>';

    return $str;
  }
}

class GroupMembershipController extends EntityAPIController {

}

class GroupMembershipUIController extends EntityDefaultUIController {

}
