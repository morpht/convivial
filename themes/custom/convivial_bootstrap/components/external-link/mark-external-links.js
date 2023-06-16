/**
 * @file
 * JS for marking the external links.
 */

(function ($) {

  Drupal.behaviors.markExternalLinks = {
    attach: function (context, settings) {
      $('.field--name-body a, .field--name-field-body a', context).filter(function () {
        return this.hostname && this.hostname !== location.hostname;
      }).attr('rel', 'external');
    }    
  }

})(jQuery);
