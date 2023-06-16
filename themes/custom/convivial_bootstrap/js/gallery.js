(function ($, Drupal, GLightbox) {
  Drupal.behaviors.gallery = {
    attach: function (context, settings) {
      const collection = document.getElementsByClassName('gallery');
      for (let item of collection) {
        let showControls = !item.classList.contains('gallery--hide-controls');
        GLightbox({
          selector: '#' + item.id + ' a.img',
          touchNavigation: showControls,
          loop: showControls,
        });
      }
    }
  };
})(jQuery, Drupal, GLightbox)
