/**
 * @file
 * JS for Fast Facts counting effect.
 */

(function ($, Drupal) {
  Drupal.behaviors.fastfacts = {
    attach: function (context, settings) {
      function count($this, number, context){
        $({ countNum: $this.html(), context:context }).animate({ countNum: number }, {
          duration: 3000,
          easing: 'linear',
          step: function () {
            $this.html(Math.floor(this.countNum).toLocaleString());
          },
          complete: function () {
            $this.html(this.countNum.toLocaleString());
          }
        });
      }

      $(".fastfact__value", context).each(function () {
        var number = parseInt($(this).html(), 10);
        $(this).html('0');
        count($(this), number, context);
      });
    }
  };

})(jQuery, Drupal);
