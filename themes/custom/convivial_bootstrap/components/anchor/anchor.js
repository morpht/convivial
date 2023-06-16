/**
 * @file
 * Configuration JS for Anchor component.
 */

(function ($, Drupal) {

  Drupal.behaviors.anchor = {
    attach: function (context, settings) {
      $('.main-content', context).anchorific({
        navigation: '.toc', // Position of navigation.
        headers: 'h2', // headers that you wish to target
        anchorClass: 'anchor', // class of anchor links
        anchorText: '#', // prepended or appended to anchor headings
        spy: true, // scroll spy
        position: 'append', // position of anchor text
        spyOffset: 150 // specify heading offset for spy scrolling
      });

      // Flag headings with anchors.
      $('.anchor').each(function () {
        $(this).parent().addClass('has-anchor');
      });

      // Use smooth-scroll for better scrolling effect.
      var scroll = new SmoothScroll('.toc a[href*="#"]');
    }
  };

})(jQuery, Drupal);
