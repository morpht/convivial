/**
 * @file
 * JS for FlipClock.
 */

(function ($) {

  Drupal.behaviors.convivial_bootstrap_FlipClock = {
    attach: function (context, settings) {
      $(".clock", context).each(function () {
        var currentDate = new Date();
        var targetDate = new Date($(this, context).html().toString());
        var countDown = true;
        var diff = targetDate.getTime() / 1000 - currentDate.getTime() / 1000;
        if (targetDate.getTime() > currentDate.getTime()) {

          countDown = true;

        }
        else {
          diff = 0 - diff;
          countDown = false;
        }
        var clock;
        clock = $(this, context).FlipClock(diff, {
          clockFace: 'DailyCounter',
          autoStart: false,
          callbacks: {
            stop: function () {
              console.log('The clock has stopped!')
            }
          }
        });
        clock.setTime(diff);
        clock.setCountdown(countDown);
        clock.start();

      });
    }
  };

})(jQuery);
