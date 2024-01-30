(function ($, Drupal, GLightbox) {
  Drupal.behaviors.gallery = {
    attach: function (context, settings) {
      const collection = document.getElementsByClassName('gallery');
      for (let item of collection) {
        let showControls = !item.classList.contains('gallery--hide-controls');
        const classes = item.className;
        // Replace whitespace for a dot.
        const selector = classes.replace(/\s/g, '.')

        GLightbox({
          selector: '.' + selector + ' a.img',
          touchNavigation: showControls,
          loop: showControls,
        });
      }
    }
  };
})(jQuery, Drupal, GLightbox)
