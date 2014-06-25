(function ($) {

  Drupal.behaviors.dMiniCal = {
    attach: function (context) {
      $('.mini-calendar', context).each(function() {
console.log('HERE');

        var self = this

        // @todo preload events
        var url = $(this).data('href');
        var calendar = $(this).clndr({

          template: $('#mini-calendar-template').html(),
          daysOfTheWeek: ['S', 'M', 'T', 'W', 'T', 'F', 'S'],
          startWithMonth: $(this).data('date') || moment(),
          events: [],
          clickEvents: {
            onMonthChange: function (month) {
console.log($(self).data('href'));
              // get data
              $.get($(self).data('href'), {date: month.format('YYYY-MM-DD')}, function (response) {
console.log(response);
                calendar.setEvents(response);
              });
            }
          },
          targets: {
            nextButton: 'clndr-next',
            previousButton: 'clndr-prev',
          },
          doneRendering: function(){
            Drupal.attachBehaviors(self);
          },
          multiDayEvents: {
            startDate: 'start',
            endDate: 'end'
          },
        });

        $.get(url, function (response) {
          calendar.setEvents(response);
        });

      });
    }
  }

})(jQuery);
