"use strict";

function _toArray(arr) {
  return _arrayWithHoles(arr) || _iterableToArray(arr) || _nonIterableRest();
}

function _nonIterableRest() {
  throw new TypeError("Invalid attempt to destructure non-iterable instance");
}

function _iterableToArray(iter) {
  if (Symbol.iterator in Object(iter) || Object.prototype.toString.call(iter) === "[object Arguments]") return Array.from(iter);
}

function _arrayWithHoles(arr) {
  if (Array.isArray(arr)) return arr;
}

(function ($, Drupal, drupalSettings) {
  Drupal.behaviors.charts = {
    attach: function attach(context, settings) {
      if (typeof drupalSettings.billboard !== 'undefined') {
        $.each(drupalSettings.billboard, function (selector, config) {
          var chart;
          var data;
          var inputDateFormat = null;
          var tickDateFormat = null;
          var _config$chart = config.chart,
            title = _config$chart.title,
            type = _config$chart.type,
            height = _config$chart.height,
            x_axis_type = _config$chart.x_axis_type,
            x_axis_label = _config$chart.x_axis_label,
            y_axis = _config$chart.y_axis,
            x_y_axis_switch = _config$chart.x_y_axis_switch,
            csv = _config$chart.csv,
            csv_url = _config$chart.csv_url,
            stacking = _config$chart.stacking,
            stacking_order = _config$chart.stacking_order,
            input_date_format = _config$chart.input_date_format,
            tick_date_format = _config$chart.tick_date_format,
            colors = _config$chart.colors,
            legend_position = _config$chart.legend_position,
            grid = _config$chart.grid; // If there's URL entered, replace CSV entered in the paragraph.

          function processCsv(csvData) {
            data = d3.csvParseRows(csvData);

            var _data = data,
              _data2 = _toArray(_data),
              header = _data2[0],
              dataSeries = _data2.slice(1); // dataSeriesLabels is used only when stacking is enabled.

            if (stacking) {
              var dataSeriesLabels = [];
              dataSeries.forEach(function (item) {
                dataSeriesLabels.push(item[0]);
              });
            }

            var x_axis_key = header[0];

            if (x_axis_type === 'timeseries') {
              inputDateFormat = input_date_format;
              tickDateFormat = tick_date_format;
            }
            else if (x_axis_type === null) {
              x_axis_type = 'indexed';
            }

            if (type === 'pie' || type === 'donut') {
              // For pie or donut chart, we need data without header.
              data = dataSeries;
              x_axis_key = undefined;
            }

            var chart = bb.generate({
              bindto: selector,
              transition: {
                duration: null
              },
              title: {
                show: true,
                text: title,
                position: 'top-center',
                // top-left, top-center and top-right
                padding: {
                  top: 20,
                  right: 20,
                  bottom: 40,
                  left: 50
                }
              },
              size: {
                height: height
              },
              data: {
                order: stacking_order,
                type: type,
                columns: data,
                x: x_axis_key,
                xFormat: inputDateFormat,
                groups: stacking ? [dataSeriesLabels] : []
              },
              color: {
                pattern: colors
              },
              axis: {
                rotated: x_y_axis_switch === '1',
                x: {
                  type: x_axis_type,
                  label: {
                    text: x_axis_label,
                    position: "outer-center"
                  },
                  tick: {
                    format: tickDateFormat
                  }
                },
                y: {
                  label: {
                    text: y_axis,
                    position: 'outer-middle'
                  }
                }
              },
              grid: {
                x: {
                  show: grid !== null && inArray(grid, 'x')
                },
                y: {
                  show: grid !== null && inArray(grid, 'y')
                }
              },
              legend: {
                show: legend_position !== 'hidden',
                position: legend_position
              }
            });
          }

          if (csv_url) {
            d3.text(csv_url).then(function (data) {
              processCsv(data);
            });
          } else { // csvParseRows method parses data directly to the arrays
            processCsv(csv);
          }
        });
      } // Auxiliary funtion to test if array contains the item.

      function inArray(arr, obj) {
        return arr.indexOf(obj) !== -1;
      }
    }
  };
})(jQuery, Drupal, drupalSettings);
