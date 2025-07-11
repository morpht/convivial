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

      // Simple vanilla JS sticky header
      makeStickyHeader = function (selector, stickyClass = 'sticky-fixed') {
        const element = document.querySelector(selector);
        if (!element) return;

        // Create placeholder
        const placeholder = document.createElement('div');
        placeholder.className = 'bs-header-placeholder';
        placeholder.style.display = 'none';
        element.parentNode.insertBefore(placeholder, element.nextSibling);

        // Get initial position
        const originalTop = element.offsetTop;
        let isSticky = false;

        function handleScroll() {
          const shouldStick = window.scrollY > originalTop;

          if (shouldStick && !isSticky) {
            // Make sticky
            isSticky = true;
            placeholder.style.height = element.offsetHeight + 'px';
            placeholder.style.display = 'block';

            element.style.position = 'fixed';
            element.style.top = '0';
            element.style.left = '0';
            element.style.width = '100%';
            element.classList.add(stickyClass);

          } else if (!shouldStick && isSticky) {
            // Remove sticky
            isSticky = false;
            placeholder.style.display = 'none';

            element.style.position = '';
            element.style.top = '';
            element.style.left = '';
            element.style.width = '';
            element.classList.remove(stickyClass);
          }
        }

        window.addEventListener('scroll', handleScroll);
      }

      $(once('convivial_bootstrap_Header--fixx', '.bs-header--sticky ' + stickyElementSelector, context)).each(function() {
        makeStickyHeader('.bs-header--sticky ' + stickyElementSelector, stickyClass);
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
