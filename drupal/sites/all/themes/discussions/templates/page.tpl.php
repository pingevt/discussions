
  <div id="branding" class="clearfix">
    <div class="container">
      <?php print render($title_prefix); ?>
      <?php if ($title): ?>
        <h1 class="page-title"><?php print $title; ?></h1>
      <?php endif; ?>
      <?php print render($title_suffix); ?>
      <?php //print render($primary_local_tasks); ?>
    </div>
  </div>

  <div id="breadcrumb-container">
    <div class="container">
      <?php print $breadcrumb; ?>
    </div>
  </div>

  <div id="page">
    <div class="container">

      <?php print render($primary_local_tasks); ?>
      <?php if ($secondary_local_tasks): ?>
        <div class="tabs-secondary clearfix"><?php print render($secondary_local_tasks); ?></div>
      <?php endif; ?>
    </div>

    <div id="content" class="clearfix container">
      <div class="element-invisible"><a id="main-content"></a></div>
      <?php if ($messages): ?>
        <div id="console" class="clearfix"><?php print $messages; ?></div>
      <?php endif; ?>
      <?php if ($page['help']): ?>
        <div id="help">
          <?php print render($page['help']); ?>
        </div>
      <?php endif; ?>
      <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>




      <?php //print render($page['content']); ?>
      <div class="row">
        <?php if (isset($page['sidebar'])) : ?>
        <div class="col-md-3 sidebar" id="sidebar">
          <?php  print render($page['sidebar']); ?>
        </div>
        <div class="col-md-11 main-content">
          <?php
            print render($page['content']);
          ?>
        </div>

        <?php else: ?>

        <div class="col-md-14">
          <?php
            print render($page['content']);
          ?>
        </div>

        <?php endif; ?>

      </div>



    </div>

    <div id="footer" class="container">
      <?php print $feed_icons; ?>
    </div>

  </div>
