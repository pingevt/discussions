<?php

?>

<?php

?>
<article class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <header>
    <button class="btn btn-default btn-xs btn-expand pull-right" data-toggle="collapse" data-target="#comment-<?php print $comment->cid; ?>-collapse" ><i class="glyphicon glyphicon-align-justify"></i> <span class="label">Expand Content</span></button>
    <?php print $picture ?>
    <h2><?php print $author; ?> commented on <?php print l($node->title, 'node/' . $node->nid); ?></h2>
  </header>


  <div class="content panel-collapse collapse"<?php print $content_attributes; ?> id="comment-<?php print $comment->cid; ?>-collapse">
    <hr />

    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['links']);
      print render($content);
    ?>

  </div>

  <hr />
  <footer>
    <p class="pull-right"><?php print format_interval((time() - $comment->created)); ?></p>
    <?php

      $node_group_info = GroupContent::getNodeGroupOwnersByUser($node);
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
