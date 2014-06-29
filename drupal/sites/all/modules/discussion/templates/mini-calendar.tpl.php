<div class="mini-calendar-wrapper">
  <script type="text/template" id="mini-calendar-template">

    <div class="row text-center">
      <div class="col-md-2 clndr-prev">&lsaquo;</div>
      <div class="col-md-10 month"><%= month %></div>
      <div class="col-md-2 clndr-next">&rsaquo;</div>
    </div>
    <div class="row text-center">
      <% _.each(daysOfTheWeek, function(day) { %><div class="col-md-2"><%= day %></div><% }); %>
    </div>
    <div class="row text-center">
      <% _.each(days, function(day) { %><div class="col-md-2 <%= day.classes %>" id="<%= day.id %>"><%= day.day %></div><% }); %>
    </div>



  </script>

  <h3>Calendar</h3>

  <div class="row text-center">
    <div class="col-md-12 col-md-offset-1">
      <div class="mini-calendar" <?php print drupal_attributes($element['#attributes']); ?>></div>
    </div>
  </div>
</div>
