(function ($, Drupal, GLightbox) {
  Drupal.behaviors.lightbox = {
    attach: function (context, settings) {
      const collection = document.getElementsByClassName('lightbox');
      for (let item of collection) {
        GLightbox({
          touchNavigation: true,
          loop: true,
          autoplayVideos: true,
          selector: '#' + item.id + ' a.img',
        });
      }
    }
  };
})(jQuery, Drupal, GLightbox)
