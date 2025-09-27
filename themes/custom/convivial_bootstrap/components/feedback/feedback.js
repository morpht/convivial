(function (Drupal, once) {
  Drupal.behaviors.feedbackForm = {
    attach: function (context, settings) {
      once('feedbackForm', '.feedback', context).forEach(function (element) {
        const currentPageURL = window.location.href;
        const encodeURL = encodeURIComponent(currentPageURL);
        const targetElement = element.querySelector('.feedback__action');

        // Set target url
        targetElement.href = targetElement.href + '?referrer=' + encodeURL;
      });
    },
  };
})(Drupal, once);
