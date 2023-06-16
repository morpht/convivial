/**
 * @file
 * JS for the Header.
 */

(function ($, Drupal, once) {
  Drupal.behaviors.convivial_bootstrap_Header = {
    attach: function (context, settings) {
      /*
      * Sticky header.
      */
      var stickyElementSelector = '.bs-header__content',
          $bsHeader = $('.bs-header'),
          $bsHeaderStickyWrap = $(stickyElementSelector),
          stickyClass = 'sticky',
          navOpenClass = 'header_navbar--open',
          bsClasses = ['bg-light', 'bg-dark', 'bg-primary', 'bg-primary-dark'],

      headerStyleClasses = (function () {
        // Build a list of BS colour classes that are set on the Header.
        var classes = '';
        $.each(bsClasses, function(i, className) {
          if ($bsHeader.hasClass(className)) {
            // Header has this class so add it to our variable for referencing later.
            classes += ' ' + className;
          }
        });
        return classes;
      }()),

      switchHeaderStyles = function(useThemeStyle) {
        var themeHeaderStyle = $bsHeader.attr('data-header-style');

        if (useThemeStyle === true) {

          // Invert to non-transparent header styles.
          $bsHeader.removeClass('bg-light bg-dark bg-primary bg-primary-dark');
          $bsHeaderStickyWrap.removeClass('bg-light bg-dark bg-primary bg-primary-dark');

          $bsHeaderStickyWrap.addClass(themeHeaderStyle);
        }
        else {// FALSE; so revert to original classes.
          $bsHeaderStickyWrap.removeClass(themeHeaderStyle);

          // Restore the original classes to the Header.
          $bsHeader.addClass(headerStyleClasses);
          $bsHeaderStickyWrap.addClass(headerStyleClasses);
        }

        // Fix for scroll bug.
        if ($(stickyElementSelector).hasClass('static')) {
          $bsHeaderStickyWrap.removeClass(themeHeaderStyle);

          // Restore the original classes to the Header.
          $bsHeader.addClass(headerStyleClasses);
          $bsHeaderStickyWrap.addClass(headerStyleClasses);
        }
      };

      // Initial the jQuery Fixx plugin.
        // once() prevents this listener being attached multiple times...
      $(once('convivial_bootstrap_Header--fixx', '.bs-header--sticky ' + stickyElementSelector, context))
        .fixx({
          // Give the placeholder a class so we can specifically target only this one on the page.
          placeholderClass: 'bs-header-placeholder',

          // Don't prepend the placeholder before the element, append it after.
          placeholderPrepend: false,

          // Keep the terminology the same.
          stateFixedClass: stickyClass,

          // Pixel offset from screen-top of when the element becomes sticky.
          startThreshold: 0
        });

      $(once('convivial_bootstrap_Header--window-change', context))
        .on('resize scroll', function () {
          // Check if the Header is Sticky or not and alter the Header.
          if ($(stickyElementSelector).hasClass(stickyClass)) {
            // Header is Sticky so change its colour classes so text is
            // accessible when the background changes to the default colour.
            switchHeaderStyles(true);
          }
          else {// Header is not Sticky.
            // Prevent the Header from reinverting on "scroll top" when
            // the Mobile Nav is open.
            if (!$('body').hasClass(navOpenClass)) {
              switchHeaderStyles(false);
            }
          }
        });

      /*
      * Primary Nav toggle.
      */
      // Add class to body when the nav toggler is clicked and nav is visible.
      $(once('convivial_bootstrap_header--navbar', '.navbar', context))
        .on('show.bs.collapse', function () {
          $('body').addClass(navOpenClass);
          // Switch to theme header styling.
          switchHeaderStyles(true);
        })
        .on('hidden.bs.collapse', function () {
          $('body').removeClass(navOpenClass);
          // Otherwise, check if header is Sticky (if header is Sticky we
          // want to maintain theme styling).
          if (!$(stickyElementSelector).hasClass(stickyClass)) {
            // Remove the theme styling.
            switchHeaderStyles(false);
          }
        });
      }
  };
})(jQuery, Drupal, once);
