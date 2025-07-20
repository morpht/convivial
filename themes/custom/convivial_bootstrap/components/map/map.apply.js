/**
 * @file
 * Map.
 */
(function (Drupal, L) {

  Drupal.behaviors.map = {
    attach: function (context, settings) {

      if (typeof settings.map !== 'undefined') {
        for (const selector in settings.map) {

          // Skip unexpected properties.
          if (!settings.map.hasOwnProperty(selector)) {
            return;
          }

          const element = context.querySelector(selector);
          if (!element) {
            return;
          }

          const config = settings.map[selector];

          async function initMap() {
            const features = [];

            // Load and parse inline text features.
            if (Array.isArray(config.text)) {
              config.text.forEach((value) => {
                try {
                  const parsedFeature = JSON.parse(value);
                  features.push(parsedFeature);
                } catch (error) {
                  console.error('Error parsing JSON feature:', error, 'Value:', value);
                }
              });
            }

            // Load remote features asynchronously.
            if (Array.isArray(config.url)) {
              for (const value of config.url) {
                try {
                  const response = await fetch(value);
                  if (response.ok) {
                    const data = await response.json();
                    features.push(data);
                  } else {
                    console.warn(`Request to ${value} failed with status ${response.status}`);
                  }
                } catch (error) {
                  console.error(`Error fetching ${value}:`, error);
                }
              }
            }

            // Only proceed if we have features.
            if (!features.length) {
              return;
            }

            // Set map height relatively to width using the ratio.
            element.style.height = (element.offsetWidth / config.ratio) + 'px';

            // Initialize map container.
            const map = L.map(element, {
              dragging: !L.Browser.mobile
            });

            // Set OpenStreetMap base layer.
            const osm = new L.TileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
              attribution: 'Map data © <a href="https://openstreetmap.org">OpenStreetMap</a> contributors'
            });
            map.addLayer(osm);

            // Set all features to map.
            const layer = L.geoJSON(features, {
              onEachFeature: function (feature, layer) {
                if (typeof feature.properties !== 'undefined') {
                  let popup = '';

                  // Add title if not empty.
                  if (feature.properties.title) {
                    let title = feature.properties.title;

                    // Wrap title by URL if not empty.
                    if (feature.properties.url) {
                      title = `<a href="${feature.properties.url}">${title}</a>`;
                    }
                    popup += `<h3>${title}</h3>`;
                  }

                  // Add description if not empty.
                  if (feature.properties.description) {
                    popup += `<p>${feature.properties.description}</p>`;
                  }

                  if (popup) {
                    layer.bindPopup(popup);
                  }
                }
              }
            });
            layer.addTo(map);

            // Set boundaries of the map container.
            map.fitBounds(layer.getBounds());
          }

          // Fire the map setup
          initMap();
        }
      }

    }
  };

})(Drupal, L);
