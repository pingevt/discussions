<?php

class GroupMembership extends MongoEntity {

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
    if (!isset($this->_bundle)) {
      $this->_bundle = 'group_membership';
    }
  }

}

class GroupMembershipController extends MongoEntityController {

}

class GroupMembershipUIController extends MongoEntityDefaultUIController {

}
