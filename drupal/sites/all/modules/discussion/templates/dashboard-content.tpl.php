<div class="row">
  <div class="col-md-3 sidebar">
    <?php
      if (isset($element['user'])) print render($element['user']);
      if (isset($element['groups'])) print render($element['groups']);
      if (isset($element['add_content'])) print render($element['add_content']);
    ?>
  </div>
  <div class="col-md-9">
    <?php
      print render($element['content']);
    ?>
  </div>
</div>
