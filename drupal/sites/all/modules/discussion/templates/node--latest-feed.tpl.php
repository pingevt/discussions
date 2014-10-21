<?php

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

$headers = array(
  'discussion' => $name . ' started a new ' . l('discussion', $node_url) . '.',
  'event' => $name . ' created a new ' . l('event', $node_url) . '.',
  'gallery' => $name . ' created a new ' . l('gallery', $node_url) . '.',
  'image' => $name . ' uploaded a new ' . l('image', $node_url) . '.',
);

?>
<article id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

  <header>
    <button class="btn btn-default btn-xs btn-expand pull-right" data-toggle="collapse" data-target="#node-<?php print $node->nid; ?>-collapse" ><i class="glyphicon glyphicon-align-justify"></i> <span class="label">Expand Content</span></button>
    <?php print $user_picture ?>
    <h2><?php print $headers[$type]; ?></h2>

  </header>


  <div class="content panel-collapse collapse"<?php print $content_attributes; ?> id="node-<?php print $node->nid; ?>-collapse">
    <hr />

    <?php print render($title_prefix); ?>
    <?php if (!$page && $type != 'discussion_reply'): ?>
      <h2<?php print $title_attributes; ?>><a href="/<?php print $node_url; ?>"><?php print $title; ?></a></h2>
    <?php elseif($type == 'discussion_reply'): ?>
      <h2<?php print $title_attributes; ?>><a href="/<?php print $node_url; ?>"><?php print $parent->title; ?></a></h2>
    <?php endif; ?>
    <?php print render($title_suffix); ?>

    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <hr />

  <footer>

    <p class="pull-right"><?php print format_interval((time() - $created)); ?></p>
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
  </footer>

</article>
