/**
 * @file
 * JS for Back To Top link.
 */

(function ($) {

  Drupal.behaviors.backToTop = {
    attach: function (context, settings) {
      var backToTop = $('.back-to-top__link', context);

      // Toggle class on backToTop.
      $(window).scroll(function () {
        if ($(this).scrollTop() > 250) {
          backToTop.addClass('is-visible');
        } else {
          backToTop.removeClass('is-visible');
        }
      });

      // Scroll smoothly to top on click.
      backToTop.click(function (event) {
        $('body, html', context).animate({
          scrollTop: 0
        }, 0);
        event.preventDefault();
      });

    }
  };

})(jQuery);
