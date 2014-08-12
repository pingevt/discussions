<?php
/**
 * @file
 * discussion.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function discussion_default_rules_configuration() {
  $items = array();
  $items['rules_send_group_notifications'] = entity_import('rules_config', '{ "rules_send_group_notifications" : {
      "LABEL" : "Send group notifications new",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Group notification" ],
      "REQUIRES" : [ "rules", "discussion_group" ],
      "ON" : { "discussion_group_node_insert_after_groups" : [] },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "discussion" : "discussion", "event" : "event", "gallery" : "gallery" } }
          }
        }
      ],
      "DO" : [
        { "discussion_group_list_users_in_group" : {
            "USING" : { "node" : [ "node" ] },
            "PROVIDE" : { "users" : { "users" : "Users" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "users" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "mail" : {
                  "to" : "[list-item:mail]",
                  "subject" : "New [node:content-type] added.",
                  "message" : "[node:body]",
                  "from" : "[site:mail]",
                  "language" : [ "node:language" ]
                }
              }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_send_group_notifications_edit'] = entity_import('rules_config', '{ "rules_send_group_notifications_edit" : {
      "LABEL" : "Send group notifications edit",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Group notification" ],
      "REQUIRES" : [ "rules", "discussion_group" ],
      "ON" : { "discussion_group_node_edit_after_groups" : [] },
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "discussion" : "discussion", "event" : "event", "gallery" : "gallery" } }
          }
        }
      ],
      "DO" : [
        { "discussion_group_list_users_in_group" : {
            "USING" : { "node" : [ "node" ] },
            "PROVIDE" : { "users" : { "users" : "Users" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "users" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "mail" : {
                  "to" : "[list-item:mail]",
                  "subject" : "[node:content-type] updated.",
                  "message" : "[node:body]",
                  "from" : "[site:mail]",
                  "language" : [ "node:language" ]
                }
              }
            ]
          }
        }
      ]
    }
  }');
  return $items;
}
