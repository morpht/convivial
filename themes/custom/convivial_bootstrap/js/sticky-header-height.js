/*
* Get the height of the sticky header.
*/
(function ($, Drupal, drupalSettings) {
  Drupal.behaviors.stickyHeaderHeight = {
    attach: function (context, settings) {
      const header = $('header.bs-header')
      if (header.hasClass('bs-header--sticky')) {
        drupalSettings.stickyHeaderHeight = header.height()
      }
    }
  };
})(jQuery, Drupal, drupalSettings)
