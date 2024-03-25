(function (Drupal) {
  Drupal.behaviors.convivialSectionWipe = {
    attach: function (context) {
      const slides = document.querySelectorAll('.main-content .section-layout--wipe .section-components__item');
      let screenWidth = window.innerWidth;
      let controller = null

      // Initial for the first time
      if (screenWidth >= 768) {
        initScrollMagic()
      }

      // On resize
      window.addEventListener('resize', function () {
        screenWidth = window.innerWidth;

        if (screenWidth < 768 && controller) {
          controller = controller.destroy(true)
        }

        if (screenWidth >= 768 && !controller) {
          initScrollMagic()
        }
      })

      // Setup scroll magic
      function initScrollMagic () {
        controller = new ScrollMagic.Controller({
          globalSceneOptions: {
            triggerHook: 'onLeave',
            duration: 0
          }
        });

        for (let i = 0; i < slides.length; i++) {
          new ScrollMagic.Scene({
            triggerElement: slides[i]
          })
            .setPin(slides[i])
            .addTo(controller);
        }
      }
    }

  };
})(Drupal);
