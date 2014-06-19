<?php

class GroupContent extends MongoEntity {
  public $gid;
  public $eid;
  public $entity_type;
  public $updated;
  public $created;

  /**
   * {@inheritdoc}
   */
  function __construct(array $values, $entity_type = NULL) {
    parent::__construct($values, $entity_type);
    if (!isset($this->_bundle)) {
      $this->_bundle = 'group_content';
    }
  }

}

class GroupContentController extends MongoEntityController {

}

class GroupContentUIController extends MongoEntityDefaultUIController {

}
