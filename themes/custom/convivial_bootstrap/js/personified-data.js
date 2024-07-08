/**
 * @file
 * Initializes all Personified Data blocks.
 */
(function ($, cookies, Drupal, Handlebars) {

  'use strict'

  Drupal.behaviors.CBpersonifiedData = {
    attach: function attach(context, settings) {

      function isJsonString (str) {
        try {
          JSON.parse(str);
        }
        catch (e) {
          return false;
        }
        return true;
      }
      if (typeof settings.personifiedData === 'undefined') {
        return
      }

      $.each(settings.personifiedData, function (id, args) {
        const element = $('.' + id, context)

        if (element.length) {
          const urlParams = {}
          $.each(args.params, function (id, param) {
            let value
            switch (param.source_type) {
              case 'query':
                value = getQueryParam(param.source_key)
                break
              case 'cookie':
                value = cookies.get(param.source_key)
                break
              case 'local_storage':
                value = localStorage.getItem(param.source_key)
                break
              case 'data_layer':
                value = getDataLayerParam(param.source_key)
                break
              case 'window':
                value = window[param.source_key]
                break
            }
            if (typeof value !== 'undefined' && value !== null) {
              if (isJsonString(value)) {
                var jsonValue = JSON.parse(value);
                if (Array.isArray(jsonValue)) {
                  urlParams[param.endpoint_key] = jsonValue.join('+');
                }
                else if (typeof jsonValue === 'object' && !Array.isArray(jsonValue)) {
                  urlParams[param.endpoint_key] = Object.keys(jsonValue).join('+');
                }
                else {
                  urlParams[param.endpoint_key] = value
                }
              }
              else {
                urlParams[param.endpoint_key] = value
              }
            }
            else if (param.default_value !== '') {
              urlParams[param.endpoint_key] = param.default_value
            }
          })

          $.ajax({
            type: 'GET',
            url: args.endpoint,
            data: urlParams,
            success: function (data, textStatus, jqXHR) {

              // Try to parse JSON data if not parsed yet.
              if (typeof data !== 'object') {
                try {
                  data = JSON.parse(data)
                }
                catch (e) {
                  console.debug('Personified: Unable to parse JSON data for endpoint "' + this.url + '".')
                  return
                }
              }

              // If data exists.
              if (data.length) {
                const handlebarsComplied = Handlebars.compile(args.template)
                const renderHandlebars = handlebarsComplied(data)

                // Render.
                element.html(renderHandlebars)
              }
            },
            error: function (jqXHR, textStatus, errorThrown) {
              console.debug('Personified: JSON data not available for endpoint "' + this.url + '".')
            }
          })
        }
      })

      function getQueryParam(key) {
        const params = window.location.search.substring(1).split('&')
        for (let i = 0; i < params.length; i++) {
          const param = params[i].split('=')
          if (param[0] === key) {
            return param[1] === undefined ? true : decodeURIComponent(param[1])
          }
        }
      }

      function getDataLayerParam(key) {
        if (typeof window.google_tag_manager !== 'undefined') {
          const gtm = window.google_tag_manager
          for (const name in gtm) {
            if (gtm.hasOwnProperty(name) && typeof gtm[name] === 'object') {
              if (typeof gtm[name].dataLayer !== 'undefined') {
                return gtm[name].dataLayer.get(key)
              }
            }
          }
        }
      }
    }
  }
})(jQuery, window.Cookies, Drupal, Handlebars)
