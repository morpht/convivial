/**
 * @file
 * Add current or past class to calendar item date.
 */

(function ($, Drupal, once) {

  Drupal.behaviors.calendarItemCurrentDate = {
    attach: function attach(context) {
      $(once('calendar-date', 'time.calendar-date', context)).each(function () {
        var $calendarDate = $(this);
        var dateTime = $calendarDate.attr('datetime');
        if (dateTime !== null) {
          // Set the time to 0, we only want to compare.
          var today = new Date().setHours(0, 0, 0, 0);
          var eventDate = new Date(dateTime).setHours(0, 0, 0, 0);

          // If event date is less than today.
          if (eventDate < today) {
            $calendarDate.closest('.calendar-item').addClass('calendar-item--past');
          }
          // If event date is today.
          if (eventDate === today) {
            $calendarDate.closest('.calendar-item').addClass('calendar-item--current');
          }
        }
      });
    }
  };

})(jQuery, Drupal, once);
