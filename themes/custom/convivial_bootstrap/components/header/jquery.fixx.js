/**
 * @file
 * Fixx | v1.0 | 05/10/2018 - https://github.com/fiasst - Marc Hudson.
 *
 * Instructions:
  * 1) For the element you want to be fixed, add:
    * "#example-element.fixed { position: fixed; }" to your CSS.
  * 2) Update #example-element with the element's ID/classname.
  * 3) Make sure the .fixed selector matches the "stateFixedClass"
    * value shown in this file.
  * 4) Add $('#example-element').fixx(); to your custom Javascript
    * with any override options you need.
*/

(function ($) {
  /*
  * Fix an element to the top of the screen on page scroll.
  * Included: responsive support on window scroll/resize,
  * placeholder element to prevent other elements resizing/repositioning
  * when element gets 'position: fixed;' style, offset and threshold and
  * callbacks.
  */
  $.fn.fixx = function (options) {
    if (this.length < 1) {
      return false;
    }

    var element = this,
        options = options || {},

        defaults = {
          // The pixel amount to offset the element from the top of the screen.
          offset: 0,

          /*
          * Provide a function() to update the offset dynamically,
          * called on window scroll/resize.
          */
          offsetCallback: false,

          /*
          * Set the window width in pixels that define each Media Query
          * breakpoint in your responsive CSS.
          */
          responsiveBreakpoints: {
            // Mobile to Tablet-portrait screens.
            sm: 768,
            // Tablet-landscape and small desktop screens.
            md: 992,
            // Larger desktop screens and above.
            lg: 1200
          },

          /*
          * list the breakpoints that this element should be Fixed for:
          * "xs" = Extra-small (mobile only) screens,
          * "sm" = Small (mobile and some tablets) screens,
          * "md" = Medium (tablet portrait) screens,
          * "lg" = Large (tablet landscape/desktop) screens.
          */
          fixedBreakpoints: ['xs', 'sm', 'md', 'lg'],

          /*
          * Start fixing the element when the screen reaches the top/bottom
          * of this element.
          */
          startElement: $('body'),

          /*
          * Define if the element should start being fixed when the top
          * of the screen reaches the "top" or "bottom" of the startElement.
          */
          startAt: 'top',

          /*
          * Set a pixel threshold from the top of the screen for when
          * the element should start being fixed.
          */
          startThreshold: 0,

          /*
          * Provide a function() to update the threshold dynamically,
          * called on window scroll/resize. For example, change the
          * threshold from 50 to 0 if the user is scrolling up.
          */
          startThresholdCallback: false,

          /*
          * If defined, the element will stop being fixed when the top
          * of the screen reaches this element.
          */
          endElement: null,

          /*
          * Define if the element should stop being fixed when the top
          * of the screen reaches the "top" or "bottom" of the endElement.
          */
          endAt: 'bottom',

          /*
          * Same as the startThreshold but for when the element should stop
          * being fixed.
          */
          endThreshold: 0,

          /*
          * Whether to let a placeholder element elmulate the fixed
          * element's position to stop other page content from repositioning
          * when the element's position changes from static to fixed.
          * The placeholder also adjusts the fixed element's width on window
          * resize for true responsive support.
          */
          placeholder: true,

          // Whether to add the placeholder before the fixed element in the
          // markup or after it. There's not many benefits to this but it can
          // be very useful in certain situations.
          placeholderPrepend: true,

          // Class to specifically target only this placeholder on the page.
          placeholderClass: '',
          // Class applied to make this element fixed in place.
          stateFixedClass: 'fixed',
          // Class applied when the element stops being fixed in place.
          stateStaticClass: 'static',
          /*
          * Class applied when the element reaches the endElement and should
          * freeze in current position.
          */
          stateFreezeClass: 'freeze',

          /*
          * There are ocassions where you might not want to assign a 'left'
          * CSS property to the fixed element. This gives you the option
          * to prevent it being set.
          */
          leftPositioning: true
        },

        optionsObj = $.extend(true, defaults, options),
        placeholder = $('<div class="fixx-placeholder ' + optionsObj.placeholderClass + '" />').css({
          height: 1,
          marginTop: -1
        }),
        startElement = $(optionsObj.startElement),
        endElement = $(optionsObj.endElement);

    // Add placeholder for position and size support.
    if (optionsObj.placeholderPrepend) {
      // Add placeholder before element.
      element.before(placeholder);
    }
    else {
      // Add placeholder after element.
      element.after(placeholder);
    }

    /*
    * Return a String depending on the window width and the defined
    * responsiveBreakpoints.
    */
    var viewportWidth = function () {
      var width = $(window).width();

      if (typeof(optionsObj.responsiveBreakpoints) !== 'undefined') {
        if (width < optionsObj.responsiveBreakpoints.sm) {
          return 'xs';
        }
        if (width >= optionsObj.responsiveBreakpoints.lg) {
          return 'lg';
        }
        if (width >= optionsObj.responsiveBreakpoints.md) {
          return 'md';
        }
        if (width >= optionsObj.responsiveBreakpoints.sm) {
          return 'sm';
        }
      }
    };

    var fix = function (element, placeholder) {
      var pos = placeholder.offset(),
          topPos,
          scrollTop = $(window).scrollTop(),

          // Get the current viewport width of the screen as a 2 digit String.
          vw = viewportWidth(),
          // Check if the element should be fixed for the current breakpoint.
          isAcceptedBreakpoint = ($.inArray(vw, optionsObj.fixedBreakpoints) > -1),

          elementHeight = element.outerHeight(true),

          startPointOffset = 0,
          endPointOffset = 9999999999;

      if ($.isFunction(optionsObj.offsetCallback)) {
        /*
        * Get a new offset value on window scroll/resize,
        * in case its changed since init.
        */
        optionsObj.offset = optionsObj.offsetCallback();
      }
      if ($.isFunction(optionsObj.startThresholdCallback)) {
        /*
        * Get a new threshold value on window scroll/resize,
        * in case its changed since init.
        */
        optionsObj.startThreshold = optionsObj.startThresholdCallback();
      }

      var startPoint = function () {
            // If startElement exists in DOM.
            if (startElement.length > 0) {
              startPointOffset = startElement.offset().top;

              // If the start point is the bottom of the startElement and not the top:
              if (optionsObj.startAt === 'bottom') {
                // Add the element height.
                startPointOffset += startElement.outerHeight() - optionsObj.offset;
              }
            }
            else {
              // Default to when user scrolls past the element.
              startPointOffset = pos.top - optionsObj.offset;
            }
            return startPointOffset + optionsObj.startThreshold;
          },
          endPoint = function () {
            // If endElement exists in DOM.
            if (endElement.length > 0) {
              endPointOffset = endElement.offset().top;

              // If the start point is the bottom of the endElement and not the top:
              if (optionsObj.endAt === 'bottom') {
                // Add the element height.
                endPointOffset += endElement.outerHeight();
              }
              endPointOffset -= (element.outerHeight(true) + (optionsObj.offset * 2));
            }
            return endPointOffset + optionsObj.endThreshold;
          };

      // Add or remove fixed/static class.
      if (isAcceptedBreakpoint && scrollTop > startPoint()) {
        element.removeClass(optionsObj.stateStaticClass).addClass(optionsObj.stateFixedClass);

        /*
        * If user has scrolled past where we want the element to
        * freeze in position:
        */
        if (scrollTop >= endPoint()) {
          // Add classname to freeze element.
          element.addClass(optionsObj.stateFreezeClass);
        }
        else {
          /*
          * Back within fixed startPoint => endPoint range so un-freeze
          * element be removing freeze classname.
          */
          element.removeClass(optionsObj.stateFreezeClass);
        }
      }
      else {
        // Scroll up past startPoint so remove fixed/freeze classes.
        element.addClass(optionsObj.stateStaticClass).removeClass(optionsObj.stateFixedClass + ' ' + optionsObj.stateFreezeClass);
      }

      if (element.hasClass(optionsObj.stateFixedClass)) {
        // Adjust placeholder height.
        if (optionsObj.placeholder) {
          placeholder.css('height', elementHeight);
        }

        // Set the top position of the element.
        if (element.hasClass(optionsObj.stateFreezeClass)) {
          // Sit frozen in place as user has scrolled past endPoint.
          topPos = endPoint() - (scrollTop - optionsObj.offset);
        }
        else {
          // Sit at top of screen + offset.
          topPos = optionsObj.offset;
        }

        // Adjust element width and position.
        element
            .css({
              width: placeholder.width(),
              top: topPos,
            });

        if (options.leftPositioning) {
          element.css({
            left: pos.left
          })
        }
      }
      else {
        element.removeAttr('style');

        // Remove placeholder height.
        if (optionsObj.placeholder) {
          placeholder.removeAttr('style');
        }
      }
    };

    $(window)
      .on('resize scroll', function () {
        fix(element, placeholder);
      });

    // Init.
    fix(element, placeholder);
  };
})(jQuery);
