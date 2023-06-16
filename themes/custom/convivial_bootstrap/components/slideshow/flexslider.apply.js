/**
 * @file
 * Flexslider apply script.
 */

(function ($, Drupal, drupalSettings) {

  Drupal.behaviors.flexslider = {
    attach: function (context, settings) {

      $('.slideshow', context).flexslider({
        selector: '.slides > .field__item',
        animation: 'fade',
        pausePlay: true,
      });

      // Wait for the flexslider script being loaded.
      setTimeout(function () {
        // Run the function immediately.
        fixFlexsliderHeight();

        // Run the function on resize.
        $(window).resize(function () {
          fixFlexsliderHeight();
        });

        function fixFlexsliderHeight() {
          // Set fixed height based on the tallest slide.
          $('.slideshow', context).each(function () {
            var slides = $(this).find('.slides > .field__item');

            // Reset min-height if it's already set.
            slides.css('min-height', 0);

            var sliderHeight = 0;

            // Find the tallest slide.
            var slideHeight;
            slides.each(function () {
              slideHeight = $(this).height();
              if (sliderHeight < slideHeight) {
                sliderHeight = slideHeight;
              }
            });

            // Set min-height of all slides to the tallest slide.
            slides.css('min-height', sliderHeight);
          });

          // Process all slideshows where ratio is set.
          if (typeof drupalSettings.slideshow !== 'undefined') {
            $.each(drupalSettings.slideshow, function (selector, ratio) {
              var element = $(selector, context);
              if (element.length) {
                var limit = 1200;
                var slideHeight = element.height();
                var slideWidth = element.width();
                var ratioHeight;
                var slides = element.find('.slides > .field__item');

                // Set the ratio 16:9 if the slides width is smaller then limit.
                if (slideWidth < limit) {
                  ratioHeight = slideWidth / ratio;
                }
                else {
                  // If not calculate the min-height from the limit.
                  ratioHeight = limit / ratio;
                }

                // Set ratioHeight only if it's bigger then sliderHeight.
                if (ratioHeight > slideHeight) {
                  slideHeight = ratioHeight;
                }

                // Set min-height of all slides to the tallest slide.
                slides.css('min-height', slideHeight);
              }

            });
          }
        }
      }, 200);

    }
  };

})(jQuery, Drupal, drupalSettings);
