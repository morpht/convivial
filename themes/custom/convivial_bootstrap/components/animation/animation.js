(function ($, Drupal, drupalSettings, once) {
  Drupal.behaviors.convivialAnimation = {
    attach: function (context) {
      if (typeof drupalSettings.animation !== 'undefined') {
        let animations = drupalSettings.animation;

        for (const paragraphId in animations) {
          let imagePaths = animations[paragraphId].imagePaths,
            delay = animations[paragraphId].delay,
            repeat = animations[paragraphId].repeat;

          // Ensure this behavior runs only once per element
          $(once('convivial-animation', 'div.paragraph--type--animation.paragraph--id-' + paragraphId, context)).each(function () {
            // init controller
            let controller = new ScrollMagic.Controller();

            // TweenMax can tween any property of any object. We use this object
            // to cycle through the array
            let obj = { curImg: 0 };

            // create tween
            let tween = TweenMax.to(obj, 0.5, {
              // animate property curImg to number of images
              curImg: imagePaths.length - 1,
              // only integers so it can be used as an array index
              roundProps: 'curImg',
              // repeat n times
              repeat: repeat,
              // load first image automatically
              immediateRender: true,
              // show every image the same amount of time
              ease: Linear.easeNone,
              onUpdate: function () {
                // Update the image source
                $('img.field--paragraph-' + paragraphId, context).attr('src', imagePaths[obj.curImg]);
              }
            });

            // build scene
            new ScrollMagic.Scene({
              triggerElement: this,
              duration: delay
            })
              .setTween(tween)
              .addTo(controller);
          });
        }
      }
    }
  };
})(jQuery, Drupal, drupalSettings, once);
