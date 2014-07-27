<?php

class GroupContent extends Entity {
  public $gid;
  public $eid;
  public $entity_type;
  public $entity_bundle;
  public $updated;
  public $created;

  /**
   * {@inheritdoc}
   */
  function __construct(array $values, $entity_type = NULL) {
    parent::__construct($values, $entity_type);
    if (!isset($this->type)) {
      $this->type = 'group_content';
    }
  }

  /**
   * Returns the groups that this entity belongs to that the user is also a
   * member of.
   */
  static function getNodeGroupOwnersByUser($node, $user = NULL) {
    if ($user == NULL) {
      global $user;
    }
    $groups = array();

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'group_content');
    $query->propertyCondition('entity_type', 'node');
    $query->propertyCondition('eid', $node->nid);
    $result = $query->execute();

    if (isset($result['group_content'])) {
      $gcs = entity_load('group_content', array_keys($result['group_content']));

      foreach ($gcs as $gc) {
        $group = $gc->getGroup();

        if ($group->isUserMember($user->uid)) {
          $groups[$group->gid] = $group;
        }
      }
    }

    return $groups;
  }

  static function getNodeGroupOwners($node) {
    $groups = array();

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'group_content');
    $query->propertyCondition('entity_type', 'node');
    $query->propertyCondition('eid', $node->nid);
    $result = $query->execute();

    if (isset($result['group_content'])) {
      $gcs = entity_load('group_content', array_keys($result['group_content']));

      foreach ($gcs as $gc) {
        $group = $gc->getGroup();
        $groups[$group->gid] = $group;
      }
    }

    return $groups;
  }

  public function getGroup() {
    return entity_load_single('group', $this->gid);
  }

}

class GroupContentController extends EntityAPIController {

}

class GroupContentUIController extends EntityDefaultUIController {

}
