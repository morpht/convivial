/**
 * @file
 * Expand accordions with checked facets.
 *
 * @see: templates/facets/facets-item-list.html.twig
 * @see: components/facets/_facets.scss
 * @see: components/facets/facets.js <-- YOU ARE HERE
 */

(function ($, Drupal) {

  'use strict';

  $(document).ready(function() {

    // Process accordions.
    $(".facets-widget-checkbox").each(function() {

      // Find checked facets.
      var checked = $(this).find("input[checked=checked]");
      if (checked.length) {
        // Expand accordion.
        if ($(this).find("button").is(":visible")) {
          $(this).find("button").click();
        }
        // Set state.
        $(this).addClass("filtered");
      }

    });
  });

})(jQuery, Drupal);
