<div class="row">
  <div class="col-md-4 sidebar">
    <?php
      if (isset($element['user'])) print render($element['user']);
    ?>
  </div>
  <div class="col-md-10">
    <?php
      print render($element['content']);
    ?>
  </div>
</div>
