<?php

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

}

class GroupMembershipController extends EntityAPIController {

}

class GroupMembershipUIController extends EntityDefaultUIController {

}
