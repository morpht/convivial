/*
* Global smooth scroll element to the destination anchor.
*/

(function (Drupal, $) {
  Drupal.behaviors.scrollAnchor = {
    attach: function (context) {
      const anchors = context.querySelectorAll('a[href^="#"]');

      if (!anchors) {
        return;
      }

      anchors.forEach(function (anchor) {

        // Make sure anchor.hash has a value before overriding default behavior
        if (anchor.hash) {
          anchor.addEventListener('click', function (e) {
            e.preventDefault();

            // Store hash
            const hash = this.hash;

            // Using jQuery's animate() method to add smooth page scroll
            // The optional number (800) specifies the number of milliseconds
            // it takes to scroll to the specified area
            $('html, body').animate({
              scrollTop: $(hash).offset().top,
            }, 800, function () {

              // Add hash (#) to URL when done scrolling
              // (default click behavior)
              window.location.hash = hash;
            });
          });
        }
      });
    },
  };
})(Drupal, jQuery);
