<div class="row">
  <div class="col-md-4 sidebar">
    <?php
      if (isset($element['user'])) print render($element['user']);
      if (isset($element['groups'])) print render($element['groups']);
      if (isset($element['add_content'])) print render($element['add_content']);
      if (isset($element['mini_calendar'])) print render($element['mini_calendar']);
    ?>
  </div>
  <div class="col-md-10">
    <?php
      print render($element['content']);
    ?>
  </div>
</div>
