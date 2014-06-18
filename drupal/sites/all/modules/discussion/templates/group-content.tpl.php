<div class="row">
  <div class="col-md-3 sidebar">
    <?php
      if (isset($element['user'])) print render($element['user']);
    ?>
  </div>
  <div class="col-md-9">
    <?php
      print render($element['content']);
    ?>
  </div>
</div>
