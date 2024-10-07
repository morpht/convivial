(function ($, Drupal, drupalSettings) {
  Drupal.behaviors.convivialAnimation = {
    attach: function (context) {
      if (typeof drupalSettings.animation !== 'undefined') {
        let animations = drupalSettings.animation;

        for (const paragraphId in animations) {
          let imagePaths = animations[paragraphId].imagePaths,
            delay = animations[paragraphId].delay,
            repeat = animations[paragraphId].repeat;

          // init controller
          let controller = new ScrollMagic.Controller();

          // TweenMax can tween any property of any object. We use this object
          // to cycle through the array
          let obj = { curImg: 0 };

          // create tween
          let tween = TweenMax.to(obj, 0.5,
            {
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
                $('img.field--paragraph-' + paragraphId).attr('src', imagePaths[obj.curImg]); // set the image source
              },
            },
          );

          // build scene
          new ScrollMagic.Scene({
            triggerElement: 'div.paragraph--type--animation.paragraph--id-' + paragraphId,
            duration: delay,
          })
            .setTween(tween)
            .addTo(controller);
        }
      }
    },
  };
})(jQuery, Drupal, drupalSettings);
