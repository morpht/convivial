/*
* Global smooth scroll element to the destination anchor.
*/
(function ($, Drupal, drupalSettings, once) {
  Drupal.behaviors.scrollAnchor = {
    attach: function (context, settings) {
      const stickyHeaderHeight = drupalSettings.stickyHeaderHeight || 0
      const scrollTopPadding = 16 + stickyHeaderHeight

      $(once('scrollAnchorOnce', context)).on('click', 'a[href^="#"]', function (event) {
        event.preventDefault();

        $('html, body').animate({
          scrollTop: $($.attr(this, 'href')).offset().top - scrollTopPadding
        }, 500);
      });
    }
  };
})(jQuery, Drupal, drupalSettings, once)
