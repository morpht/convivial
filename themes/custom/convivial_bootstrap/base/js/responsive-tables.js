/**
 * @file
 * JS for wrapping tables in a div with .table-responsive class.
 */

(function ($, Drupal) {

  'use strict';

  Drupal.behaviors.convivial_bootstrap_ResponsiveTables = {
    attach: function (context, settings) {

      var table = $('table', context);

      // Loop through tables on page.
      $(table).each(function () {
        // If table has not class table add that class and
        // wrap table with table-responsive div.
        if (!$(this).hasClass('table')) {
          $(this).addClass('table').wrap('<div class="table-responsive"></div>');
        }
        // Else only wrap table with table-responsive div.
        else {
          $(this).wrap('<div class="table-responsive"></div>');
        }
      });

    }
  };

})(jQuery, Drupal);
