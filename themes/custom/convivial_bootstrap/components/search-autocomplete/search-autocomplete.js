(function ($, Drupal, once) {

  'use strict';

  Drupal.behaviors.searchAutocomplete = {
    attach: function (context, settings) {

      $(once('searchFormInputFocusIn', '.block-bundle-search .searchform .searchform__query', context)).focusin(function () {
        $(this).autocomplete({
          source: function (request, response) {
            $.getJSON("/data/autocomplete?v=" + request.term, function (data) {
              response($.map(data, function (value, key) {
                return {
                  id: value.nid,
                  title: value.title,
                  html: value.html,
                };
              }));
            });
          },
          minLength: 3,
          delay: 100,
          classes: {
            "ui-autocomplete": "global-search"
          }
        }).data("ui-autocomplete")._renderItem = function (ul, item) {
          return $("<li>")
            .attr("data-id", item.id)
            .append(item.html)
            .appendTo(ul)
        };
      })

      $(once('searchFormInputFocusOut', '.block-bundle-search .searchform .searchform__query', context)).focusout(function () {
        $(this).autocomplete('destroy')
      })
    }
  };
})(jQuery, Drupal, once);
