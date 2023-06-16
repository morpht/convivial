/**
 * @file
 * Remove image alt for icon, stack, teasers and tile.
 */

(function ($, Drupal) {

  'use strict';

  Drupal.behaviors.accessibleImageAlt = {
    attach: function (context, settings) {

      // Remove image alt text.
      this.remove_alt('.icon__icon--decorative img', context);
      this.remove_alt('.stack__image--decorative img', context);
      this.remove_alt('.teaser__image--decorative img', context);
      this.remove_alt('.tile__image--decorative img', context);

    },

    remove_alt: function (imageSection, context) {

      $(imageSection, context).each(function (index) {
        $(this).attr('alt', '');
      });

    }
  };

})(jQuery, Drupal);
