(function ($, Drupal) {
  Drupal.behaviors.convivial_claro_layoutParagraphsHotfix = {
    attach: function (context, settings) {

      // This is workaround for layout_paragraphs modal not showing up.
      //
      // The error can be seen as:
      // ```
      // dialog.position.js?v=9.2.9:37 Uncaught TypeError: Cannot read
      // properties of null (reading 'settings') at resetSize
      // (dialog.position.js?v=9.2.9:37) at later (debounce.js?v=9.2.9:22)
      // ```
      // in browser console after clicking the "pencil" edit button on a
      // paragraph.  @See: Redmine #25673.
      window.addEventListener('error', function (event) {
        var search = 'Cannot read prop';
        if (event.error.message.indexOf(search) === 0) {
          $(window).trigger('resize');
        }
      })

    }
  };
})(jQuery, Drupal);
