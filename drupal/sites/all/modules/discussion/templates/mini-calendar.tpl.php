<script type="text/template" id="mini-calendar-template">

<!--
            <div class="controls">
              <div class="clndr-previous-button">&lsaquo;</div><div class="month"><%= month %></div><div class="clndr-next-button">&rsaquo;</div>
            </div>

            <div class="days-container">
              <div class="days">
                <div class="headers">
                  <% _.each(daysOfTheWeek, function(day) { %><div class="day-header"><%= day %></div><% }); %>
                </div>
                <% _.each(days, function(day) { %><div class="<%= day.classes %>" id="<%= day.id %>"><%= day.day %></div><% }); %>
              </div>
              <div class="events">
                <div class="headers">
                  <div class="x-button">x</div>
                  <div class="event-header">EVENTS</div>
                </div>
                <div class="events-list">
                  <% _.each(eventsThisMonth, function(event) { %>
                    <div class="event">
                      <a href="<%= event.url %>"><%= moment(event.date).format('MMMM Do') %>: <%= event.title %></a>
                    </div>
                  <% }); %>
                </div>
              </div>
            </div>
-->

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
  <div class="col-md-10">
    <div class="mini-calendar" <?php print drupal_attributes($element['#attributes']); ?>></div>
  </div>
</div>
