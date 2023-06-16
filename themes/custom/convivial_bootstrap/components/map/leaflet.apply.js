/**
 * @file
 * Map.
 */
(function (Drupal, L) {

  Drupal.behaviors.map = {
    attach: function (context, settings) {

      if (typeof settings.map !== 'undefined') {
        for (var selector in settings.map) {

          // Skip unexpected properties.
          if (!settings.map.hasOwnProperty(selector)) {
            return;
          }

          var element = context.querySelector(selector);
          if (!element) {
            return;
          }

          var config = settings.map[selector];
          var features = [];

          // Text field.
          if (typeof config.text !== 'undefined') {
            config.text.forEach(function (value) {
              features.push(JSON.parse(value));
            });
          }

          // URL field.
          if (typeof config.url !== 'undefined') {
            config.url.forEach(function (value) {
              // Send AJAX GET request and parse to object.
              var xmlhttp = new XMLHttpRequest();
              xmlhttp.open('GET', value, false);
              xmlhttp.onreadystatechange = function() {
                if (this.readyState === 4 && this.status === 200) {
                  features.push(JSON.parse(this.responseText));
                }
              };
              xmlhttp.send();
            });
          }

          if (features.length) {
            // Set map height relatively to width using 4:3 ratio.
            element.style.height = (element.offsetWidth / config.ratio) + 'px';

            // Initialize map container.
            var map = L.map(element, {
              // Enable two fingers dragging only on touch devices.
              dragging: !L.Browser.mobile
            });

            // Define OpenStreetMap base layer.
            var osmUrl = 'https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png';

            // Set OpenStreetMap base layer.
            var osm = new L.TileLayer(osmUrl, {
              attribution: 'Map data © <a href="https://openstreetmap.org">'
                + 'OpenStreetMap</a> contributors'
            });
            map.addLayer(osm);

            // Set all features to map.
            var layer = L.geoJSON(features, {
              onEachFeature: function (feature, layer) {
                if (typeof feature.properties !== 'undefined') {
                  var popup = '';

                  // Add title if not empty.
                  if (typeof feature.properties.title !== 'undefined' && feature.properties.title !== '') {
                    var title = feature.properties.title;

                    // Wrap title by URL if not empty.
                    if (typeof feature.properties.url !== 'undefined' && feature.properties.url !== '') {
                      title = '<a href="' + feature.properties.url + '">' + title + '</a>';
                    }
                    popup += '<h3>' + title + '</h3>';
                  }

                  // Add description if not empty.
                  if (typeof feature.properties.description !== 'undefined' && feature.properties.description !== '') {
                    popup += '<p>' + feature.properties.description + '</p>';
                  }

                  if (popup !== '') {
                    layer.bindPopup(popup);
                  }
                }
              }
            });
            layer.addTo(map);

            // Set boundaries of the map container.
            map.fitBounds(layer.getBounds());
          }

        }
      }

    }
  };

})(Drupal, L);
