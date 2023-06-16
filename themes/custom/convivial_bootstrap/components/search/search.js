/**
 * @file
 * JS for Search Form.
 */

(function ($, Drupal, once) {

  'use strict';

  Drupal.behaviors.convivial_bootstrap_Search = {
    attach: function (context, settings) {

      /*
      * Inline reveal Searchform.
      */
      if ($('body').hasClass('search-form--inline-reveal')) {
        var $form = $('.searchform', context);
        var $input = $('.searchform__query', $form);
        var $button = $('.searchform__button', $form);

        // Disable search form submit if text is empty.
        $form.submit(function (event) {
          if ($('.searchform__query', this).val().length === 0) {
            // Prevent the form from submitting.
            event.preventDefault();
          }
        });

        var buttonClicked = function () {
          // Show Search if block is going to active.
          if (!$form.hasClass('searchform--open')) {
            $form.addClass('searchform--open');
            $input.focus();
          }
          // Hide Search if not active and input is empty.
          else if ($input.val().length === 0) {
            $form.removeClass('searchform--open');
          }
        };

        // Hide form input if clicked button or anywhere else than search form.
        $input.focusout(function () {
          if ($input.val().length === 0) {
            $form.removeClass('searchform--open');
            $button.off('click');

            // Allow to click the button again after 500ms.
            setTimeout(function () {
              $button.click(buttonClicked);
            }, 500);
          }
        });

        // Toggle form input when needed.
        $button.click(buttonClicked);
      }
      else {

        /*
        * Searchform Toggle.
        */
        var searchform_toggler = $('.searchform-toggler'),
            searchform_block = $('.block-bundle-search'),
            searchQueryInputSelector = '.searchform__query';

        // Search Form is visible on toggler button click.
        // once() prevents this listener being attached mutliple times...
        $(once('convivial_bootstrap_Search--toggler', '.searchform-toggler', context))
          .on('click', function () {
            // Toggle class on search form.
            searchform_block.toggleClass('active');

            // Create flag for whether search block is visible.
            var searchVisible = $(searchform_block).hasClass('active');

            // Toggle class on toggler button.
            searchform_toggler.toggleClass('active', searchVisible);

            // Toggle body class to assist in CSS styling when search form is visible.
            // basically, remove the animated fade-in when Header gets Sticky.
            $('body').toggleClass('searchform--open', searchVisible);

            // Focus on search input field if the form block is visible "active".
            if (searchVisible) {
              searchform_block.find(searchQueryInputSelector).focus();
            }
          });

        // Disable search form submit if text is empty.
        $(once('convivial_bootstrap_Search--submit', '.block-bundle-search', context))
          .on('submit', 'form', function (e) {
            if (!$(searchQueryInputSelector, this).val()) {
              // Prevent the form from submitting.
              e.preventDefault();
            }
          });
      }

    }
  };
})(jQuery, Drupal, once);
