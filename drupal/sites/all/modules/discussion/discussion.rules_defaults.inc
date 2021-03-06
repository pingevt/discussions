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
                  "subject" : "[site:name]: New [node:content-type] added.",
                  "message" : "New content created for [node:title].\r\n\r\n[node:body]\r\n\r\n[node:url]",
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
  $items['rules_send_group_comment_notification'] = entity_import('rules_config', '{ "rules_send_group_comment_notification" : {
      "LABEL" : "Send Group Comment notification",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Group notification" ],
      "REQUIRES" : [ "rules", "discussion_group", "comment" ],
      "ON" : { "comment_insert--comment_node_discussion" : { "bundle" : "comment_node_discussion" } },
      "DO" : [
        { "variable_add" : {
            "USING" : { "type" : "node", "value" : [ "comment:node" ] },
            "PROVIDE" : { "variable_added" : { "node" : "Comment node" } }
          }
        },
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
                  "subject" : "[site:name]: Comment added.",
                  "message" : "New comment added to [node:title].\r\n\r\n[comment:body]\r\n\r\n[node:url]#comment-[comment:cid]",
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
                  "subject" : "[site:name]: New [node:content-type] updated.",
                  "message" : "Content updated for [node:title].\r\n\r\n[node:body]\r\n\r\n[node:url]",
                  "from" : "Discussions \u003Cdiscussions@peteinge.com\u003E",
                  "language" : [ "node:language" ]
                }
              }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_send_group_comment_notification_edit'] = entity_import('rules_config', '{ "rules_send_group_comment_notification_edit" : {
      "LABEL" : "Send Group Comment notification edit",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Group notification" ],
      "REQUIRES" : [ "rules", "discussion_group", "comment" ],
      "ON" : { "comment_update--comment_node_discussion" : { "bundle" : "comment_node_discussion" } },
      "DO" : [
        { "variable_add" : {
            "USING" : { "type" : "node", "value" : [ "comment:node" ] },
            "PROVIDE" : { "variable_added" : { "node" : "Comment node" } }
          }
        },
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
                  "subject" : "[site:name]: Comment added.",
                  "message" : "Comment edited on [node:title].\r\n\r\n[comment:body]\r\n\r\n[node:url]#comment-[comment:cid]",
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
  $items['rules_notify_user_of_group_membership_'] = entity_import('rules_config', '{ "rules_notify_user_of_group_membership_" : {
      "LABEL" : "Notify user of group Membership.",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Group membership" ],
      "REQUIRES" : [ "rules", "discussion_group" ],
      "ON" : { "user_added_to_discussion_group" : [] },
      "DO" : [
        { "mail" : {
            "to" : "[user:mail]",
            "subject" : "You have been added to a new group.",
            "message" : "[user:name],\r\n\r\nA group administrator has added you to their group, [group:group-name].\r\n\r\nYou can visit the group at [group:group-url]\r\n\r\nMore about this group:\r\n[group:field_description]\r\n\r\n--  [site:name] team"
          }
        }
      ]
    }
  }');
  return $items;
}
