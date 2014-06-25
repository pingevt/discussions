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

}

class GroupContentController extends EntityAPIController {

}

class GroupContentUIController extends EntityDefaultUIController {

}
