<?php
/**
 * @file
 * group_content.inc
 */

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

  static function setGroupContent($entity_type, $entity, $group_ids) {

    list($id, $vid, $bundle) = entity_extract_ids($entity_type, $entity);
    if ($bundle == NULL) $bundle = $entity_type;
    $original_group_ids = $group_ids;

    // Check for existing, update or add new.
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'group_content');
    $query->propertyCondition('entity_type', $entity_type);
    $query->propertyCondition('eid', $id);

    $results = $query->execute();

    if (isset($results['group_content'])) {
      $group_contents = entity_load('group_content', array_keys($results['group_content']));

      foreach ($group_contents as $group) {
        if (in_array($group->gid, $group_ids)) {
          // remove so we don't add later.
          unset($group_ids[array_search($group->gid, $group_ids)]);

          // update only with node type and save
          $group->entity_bundle = $bundle;
          $group->save();
        }
        else {
          // If it is not in the array we need to delete it.
          $group->delete();
        }
      }
    }

    foreach ($group_ids as $group_id) {
      $gc = entity_create('group_content', array());

      $gc->eid = $id;
      $gc->gid = $group_id;
      $gc->entity_type = $entity_type;
      $gc->entity_bundle = $bundle;

      $gc->save();
    }
  }

}

class GroupContentController extends EntityAPIController {

}

class GroupContentUIController extends EntityDefaultUIController {

}
