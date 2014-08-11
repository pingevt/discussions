<?php

$headers = array(
  'discussion' => $name . ' started a new discussion.',
  'discussion_reply' => $name . ' commented on a discussion.',
  'event' => $name . ' created a new event.',
  'gallery' => $name . ' created a new gallery.',
  'image' => $name . ' uploaded a new image.',
);

//$node_url = '';

// get groups info
$node_group_info = GroupContent::getNodeGroupOwnersByUser($node);
$current_group = discussion_get_current_group();

if ($type != 'discussion') {
  if ($current_group != NULL) {
    $node_url = 'group/' . $current_group->gid . '/' . $type . '/' . $node->nid;
  }
  else {
    $node_url = 'group/' . current($node_group_info)->gid . '/' . $type . '/' . $node->nid;
  }
}
else {
  $parent = _get_discussion_parent($node);

  if ($current_group != NULL) {
    $node_url = 'group/' . $current_group->gid . '/' . $type . '/' . $parent->nid;
  }
  else {
    $node_url = 'group/' . current($node_group_info)->gid . '/' . $type . '/' . $parent->nid;
  }

  if ($parent !== $node) {
    $type = 'discussion_reply';
  }
}

?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <header>
    <?php print $user_picture ?>

    <h2><?php print $headers[$type]; ?></h2>
    <p><?php print format_interval((time() - $created)); ?></p>

  </header>

  <hr />

  <?php print render($title_prefix); ?>
  <?php if (!$page && $type != 'discussion_reply'): ?>
    <h2<?php print $title_attributes; ?>><a href="/<?php print $node_url; ?>"><?php print $title; ?></a></h2>
  <?php elseif($type == 'discussion_reply'): ?>
    <h2<?php print $title_attributes; ?>><a href="/<?php print $node_url; ?>"><?php print $parent->title; ?></a></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <div class="content"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <hr />

  <?php
    $group_list = array(
      '#theme' => 'item_list',
      '#items' => array(),
      '#title' => t('Groups'),
      '#prefix' => '<div class="node-groups-list">',
      '#suffix' => '</div>',
    );

    foreach ($node_group_info as $group) {
      $uri = entity_uri('group', $group);
      $group_list['#items'][] = l($group->label(), $uri['path'], $uri['options']);
    }

    print render($group_list);
  ?>

</article>
