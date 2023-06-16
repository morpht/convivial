(function ($, Drupal) {
  Drupal.behaviors.convivialDemoSlider = {
    attach: function (context, settings) {

      let defaultSettings = {
        adaptiveHeight: true,
        centerMode: true,
      }

      let halvesSettings = {
        ...defaultSettings,
        responsive: [
          {
            breakpoint: 576,
            settings: {
              slidesToShow: 1,
            }
          },
        ]
      }

      let thirdsSettings = {
        ...defaultSettings,
        responsive: [
          {
            breakpoint: 576,
            settings: {
              slidesToShow: 1,
            }
          },
          {
            breakpoint: 768,
            settings: {
              slidesToShow: 2,
            }
          }
        ]
      }

      let quartersSettings = {
        ...defaultSettings,
        responsive: [
          {
            breakpoint: 576,
            settings: {
              slidesToShow: 1,
            }
          },
          {
            breakpoint: 768,
            settings: {
              slidesToShow: 2,
            }
          },
          {
            breakpoint: 992,
            settings: {
              slidesToShow: 3,
            }
          }
        ]
      }

      $('.list-layout--slider-full-width .list', context).slick({
        ...defaultSettings,
        slidesToShow: 1,
      })

      $('.list-layout--slider-halves .list', context).slick({
        ...halvesSettings,
        slidesToShow: 2,
      })

      $('.list-layout--slider-thirds .list', context).slick({
        ...thirdsSettings,
        slidesToShow: 3,
      })

      $('.list-layout--slider-quarters .list', context).slick({
        ...quartersSettings,
        slidesToShow: 4,
      })

      $('.views-layout--slider-full-width .view-content', context).slick({
        ...defaultSettings,
        slidesToShow: 1,
      })

      $('.views-layout--slider-halves .view-content', context).slick({
        ...halvesSettings,
        slidesToShow: 2,
      })

      $('.views-layout--slider-thirds .view-content', context).slick({
        ...thirdsSettings,
        slidesToShow: 3,
      })

      $('.views-layout--slider-quarters .view-content', context).slick({
        ...quartersSettings,
        slidesToShow: 4,
      })
    }
  };
})(jQuery, Drupal);
