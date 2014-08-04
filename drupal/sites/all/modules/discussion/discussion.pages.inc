<?php
/**
 * @file
 * discussion.pages.inc
 */

function discussion_login_page_view() {
  $build = array();

  return $build;
}



function discussion_dashboard_page_view() {
  global $user;

  drupal_set_title(t('Dashboard'));

  $groups = Group::getCurrentUserGroups();

  // If they only belong to one group, forwward them to the group dashboard page.
  if (count($groups) == 1) {
    drupal_goto('group/' . current($groups)->gid);
  }

  // Add main content
  $build = array(
    '#type' => 'container',
    'items' => array(),
    '#prefix' => '<div id="latest-content-item-list">',
    '#suffix' => '</div>',
  );

  // Pull all comments and nodes...
  if (!empty($groups)) {
    $page = isset($_GET['page'])? $_GET['page'] : 0;
    $page_count = 20;
    $offset = $page * $page_count;

    $count = _pull_latest_content(NULL, NULL, NULL);

    $items = _pull_latest_content(NULL, $offset, $page_count);

    foreach ($items as $item) {

      $entity_type = $item->__nc;

      if ($entity_type == 'node') {
        $content = node_view($item, 'latest_feed', $item->language);
      }
      else {
        $content = comment_view($item, node_load($item->nid), 'latest_feed', $item->language);
      }

      $content += array(
        '#theme' => 'entity',
        '#entity_type' => $entity_type,
        '#entity' => $item,
        '#view_mode' => 'latest_feed',
        '#language' => $item->language,
        '#page' => NULL,
      );

      $build['items'][] = $content;
    }

    $max = ceil($count/$page_count) - 1;
    if ($max < 0) $max = 0;

    $build['pager'] = array(
      '#theme' => 'latest_feed_pager',
      '#page' => $page,
      '#max' => $max,
    );
  }
  else {
    // TODO: dosomething here...
    $build['#items'][] = 'You do not belong to any groups.';
  }

  return $build;
}

function discussion_group_dashboard_page_view($group) {

  drupal_set_title($group->label());

  // Build Breadcrumbs
  $breadcrumb = array();
  $breadcrumb[] = l(t('Dashboard'), '<front>');

  // Set Breadcrumbs
  drupal_set_breadcrumb($breadcrumb);

  return _latest_feed_landing_page($group, NULL);
}


function discussion_group_membership_page_view($group) {
  $build = array();
  drupal_set_title($group->label() . ' Group Memberships');

  // Build Breadcrumbs
  $breadcrumb = array();
  $breadcrumb[] = l(t('Dashboard'), '<front>');
  $breadcrumb[] = l(t($group->label() . ' Dashboard'), 'group/' . $group->gid);

  // Set Breadcrumbs
  drupal_set_breadcrumb($breadcrumb);

  // Add user
  $build['add_user'] = drupal_get_form('group_add_user_by_email_form', $group);

  // List members
  $members = $group->getGroupMembers();

  $build['members_table'] = array(
    '#theme' => 'table',
    '#header' => array('Username', 'Email', 'Group Role', array('data' => 'Operations', 'colspan' => 2)),
    '#rows' => array(),
    '#empty' => 'There are no users in this group',
  );

  foreach ($members as $member) {
    if (is_object($member->_user)) {
      $build['members_table']['#rows'][] = array(
        $member->_user->name,
        $member->_user->mail,
        $group->getUsersRole($member),
        l(t('Remove'), 'group/' . $group->gid . '/memberships/' . $member->_user->uid . '/remove', array('query' => array('destination' => $_GET['q']))),
        $member->createChangeRolesList(),
      );
    }
  }

  return $build;
}


function discussion_group_discussion_landing_page_view($group) {
  drupal_set_title(t($group->label() . ' Discussions'));

  // Build Breadcrumbs
  $breadcrumb = array();
  $breadcrumb[] = l(t('Dashboard'), '<front>');
  $group_uri = $group->uri();
  $breadcrumb[] = l($group->label(), $group_uri['path'], $group_uri['options']);

  // Set Breadcrumbs
  drupal_set_breadcrumb($breadcrumb);

  return _latest_feed_landing_page($group, 'discussion');
}

function discussion_group_event_landing_page_view($group) {
  drupal_set_title(t($group->label() . ' Events'));
  $build = array();

  // Build Breadcrumbs
  $breadcrumb = array();
  $breadcrumb[] = l(t('Dashboard'), '<front>');
  $group_uri = $group->uri();
  $breadcrumb[] = l($group->label(), $group_uri['path'], $group_uri['options']);

  // Set Breadcrumbs
  drupal_set_breadcrumb($breadcrumb);

  $items = $group->getGroupEvents();

  foreach ($items as $item) {
    $content = node_view($item, 'teaser', $item->language);

    $content += array(
      '#theme' => 'entity',
      '#entity_type' => $item->type,
      '#entity' => $item,
      '#view_mode' => 'teaser',
      '#language' => $item->language,
      '#page' => NULL,
    );

    $build['items'][] = $content;
  }

  return $build;
}

function discussion_group_gallery_landing_page_view($group) {
  drupal_set_title(t($group->label() . ' Galleries'));
  $build = array();

  // Build Breadcrumbs
  $breadcrumb = array();
  $breadcrumb[] = l(t('Dashboard'), '<front>');
  $group_uri = $group->uri();
  $breadcrumb[] = l($group->label(), $group_uri['path'], $group_uri['options']);

  $items = $group->getGroupGalleries();

  foreach ($items as $item) {
    $content = node_view($item, 'teaser', $item->language);

    $content += array(
      '#theme' => 'entity',
      '#entity_type' => $item->type,
      '#entity' => $item,
      '#view_mode' => 'teaser',
      '#language' => $item->language,
      '#page' => NULL,
    );

    $build['items'][] = $content;
  }

  return $build;
}

function _latest_feed_landing_page($group, $content_type = NULL) {

  // Add main content
  $build = array(
    '#type' => 'container',
    'items' => array(),
    '#prefix' => '<div id="latest-content-item-list">',
    '#suffix' => '</div>',
  );

  $page = isset($_GET['page'])? $_GET['page'] : 0;
  $page_count = 20;
  $offset = $page * $page_count;

  $count = _pull_latest_content($group, NULL, NULL, $content_type);

  $items = _pull_latest_content($group, $offset, $page_count, $content_type);

  foreach ($items as $item) {

    $entity_type = $item->__nc;

    if ($entity_type == 'node') {
      $content = node_view($item, 'latest_feed', $item->language);
    }
    else {
      $content = comment_view($item, node_load($item->nid), 'latest_feed', $item->language);
    }

    $content += array(
      '#theme' => 'entity',
      '#entity_type' => $entity_type,
      '#entity' => $item,
      '#view_mode' => 'latest_feed',
      '#language' => $item->language,
      '#page' => NULL,
    );

    $build['items'][] = $content;
  }

  $max = ceil($count/$page_count) - 1;
  if ($max < 0) $max = 0;

  $build['pager'] = array(
    '#theme' => 'latest_feed_pager',
    '#page' => $page,
    '#max' => $max,
  );

  return $build;
}



function discussion_group_discussion_add($group) {
  module_load_include('inc', 'node', 'node.pages');
  global $user;
  $build = array();

  $node = new stdClass();
  $node->uid = $user->uid;
  $node->name = isset($user->name) ? $user->name : '';
  $node->type = 'discussion';
  $node->language = LANGUAGE_NONE;

  $form = drupal_get_form('discussion_node_form', $node);
  $form['field_reply_to']['#access'] = FALSE;
  $form['menu']['#access'] = FALSE;
  $form['path']['#access'] = FALSE;
  $form['additional_settings']['#access'] = FALSE;
  $form['revision_information']['#access'] = FALSE;

  $build['reply'] = $form;

  return $build;
}

function discussion_group_discussion_page_view($group, $node) {
  global $user;
  module_load_include('inc', 'node', 'node.pages');

  drupal_set_title($node->title);

  // Build Breadcrumbs
  $breadcrumb = array();
  $breadcrumb[] = l(t('Dashboard'), '<front>');
  $breadcrumb[] = l(t($group->label() . ' Dashboard'), 'group/' . $group->gid);
  $breadcrumb[] = l(t('Discussions'), 'group/' . $group->gid . '/discussions');

  // Set Breadcrumbs
  drupal_set_breadcrumb($breadcrumb);


  $build = array();
  $view_mode = 'full';
  $langcode = NULL;

  // Populate $node->content with a render() array.
  node_build_content($node, $view_mode, $langcode);

  $build['node'] = $node->content;
  // We don't need duplicate rendering info in node->content.
  unset($node->content);

  $node->thread = _build_discussion_thread($node, 'discussion_replies', $langcode);

  $build['node'] += array(
    '#theme' => 'node',
    '#node' => $node,
    '#view_mode' => $view_mode,
    '#language' => $langcode,
  );

  // Allow modules to modify the structured node.
  $type = 'node';
  drupal_alter(array('node_view', 'entity_view'), $build['node'], $type);

  // Build thread...
  //$build['thread'] = _build_discussion_thread($node, 'discussion_replies', $langcode);

  return $build;
}



function discussion_group_event_add($group) {
  module_load_include('inc', 'node', 'node.pages');
  global $user;
  $build = array();

  $node = new stdClass();
  $node->uid = $user->uid;
  $node->name = isset($user->name) ? $user->name : '';
  $node->type = 'event';
  $node->language = LANGUAGE_NONE;

  $form = drupal_get_form('event_node_form', $node);

  /*
  $form['field_reply_to']['#access'] = FALSE;
  $form['menu']['#access'] = FALSE;
  $form['path']['#access'] = FALSE;
  $form['additional_settings']['#access'] = FALSE;
  $form['revision_information']['#access'] = FALSE;
  */

  $build = $form;

  return $build;
}

function discussion_group_event_page_view($group, $node) {

  global $user;

  drupal_set_title($node->title);

  // Build Breadcrumbs
  $breadcrumb = array();
  $breadcrumb[] = l(t('Dashboard'), '<front>');
  $breadcrumb[] = l(t($group->label() . ' Dashboard'), 'group/' . $group->gid);
  $breadcrumb[] = l(t('Events'), 'group/' . $group->gid . '/events');

  // Set Breadcrumbs
  drupal_set_breadcrumb($breadcrumb);

  $build = array();
  $view_mode = 'full';
  $langcode = NULL;

  // Populate $node->content with a render() array.
  node_build_content($node, $view_mode, $langcode);

  $build['node'] = $node->content;
  // We don't need duplicate rendering info in node->content.
  unset($node->content);

  $build['node'] += array(
    '#theme' => 'node',
    '#node' => $node,
    '#view_mode' => $view_mode,
    '#language' => $langcode,
  );

  // Allow modules to modify the structured node.
  $type = 'node';
  drupal_alter(array('node_view', 'entity_view'), $build['node'], $type);

  return $build;
}



function discussion_group_gallery_add($group) {
  module_load_include('inc', 'node', 'node.pages');
  global $user;
  $build = array();

  $node = new stdClass();
  $node->uid = $user->uid;
  $node->name = isset($user->name) ? $user->name : '';
  $node->type = 'gallery';
  $node->language = LANGUAGE_NONE;

  $form = drupal_get_form('event_node_form', $node);

  $build = $form;

  return $build;
}

function discussion_group_gallery_page_view($group, $node) {

  global $user;

  drupal_set_title($node->title);

  // Build Breadcrumbs
  $breadcrumb = array();
  $breadcrumb[] = l(t('Dashboard'), '<front>');
  $breadcrumb[] = l(t($group->label() . ' Dashboard'), 'group/' . $group->gid);
  $breadcrumb[] = l(t('Galleries'), 'group/' . $group->gid . '/galleries');

  // Set Breadcrumbs
  drupal_set_breadcrumb($breadcrumb);

  $build = array();
  $view_mode = 'full';
  $langcode = NULL;

  // Populate $node->content with a render() array.
  node_build_content($node, $view_mode, $langcode);

  $build['node'] = $node->content;
  // We don't need duplicate rendering info in node->content.
  unset($node->content);

  $build['node'] += array(
    '#theme' => 'node',
    '#node' => $node,
    '#view_mode' => $view_mode,
    '#language' => $langcode,
  );

  // Allow modules to modify the structured node.
  $type = 'node';
  drupal_alter(array('node_view', 'entity_view'), $build['node'], $type);

  return $build;
}
