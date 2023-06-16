(function ($, Drupal, drupalSettings) {
  Drupal.behaviors.convivialGeoChart = {
    attach: function (context, settings) {
      const geoChart = drupalSettings.geoChart

      if (!geoChart) {
        return
      }

      // Define charts.
      google.charts.load('current', {
        'packages': ['geochart'],
      })

      // Loop each paragraph.
      Object.entries(geoChart).forEach(([chartKey, chartItem]) => {
        const paragraphId = chartItem['paragraphId']
        const chartId = 'paragraph-geochart-' + paragraphId
        const csvData = chartItem['csvData'] ?? null
        const url = chartItem['url'] ?? null
        const options = chartItem['options'] ?? {}

        // Draw chart.
        google.charts.setOnLoadCallback(drawChart)

        function drawChart () {
          if (url) {
            drawChartFromUrl()
          }
          else {
            drawChartFromCSV()
          }
        }

        function drawChartFromCSV () {
          const chartData = new google.visualization.arrayToDataTable(csvData)
          const chart = new google.visualization.GeoChart(document.getElementById(chartId))
          chart.draw(chartData, options)
        }

        async function drawChartFromUrl () {
          const csvData = await getCsvDataFromUrl(url)
          const chartData = new google.visualization.arrayToDataTable(csvData)
          const chart = new google.visualization.GeoChart(document.getElementById(chartId))
          chart.draw(chartData, options)
        }

        async function getCsvDataFromUrl (url) {
          let csvData
          await d3.text(url).then(function (data) {
            csvData = d3.csvParseRows(data)
          })

          // Convert string to numeric.
          for (let i = 0; i < csvData.length; ++i) {
            for (let j = 0; j < csvData[i].length; ++j) {
              if ($.isNumeric(csvData[i][j])) {
                csvData[i][j] = parseFloat(csvData[i][j])
              }
            }
          }
          return csvData
        }

        // Create trigger resizeTO to resizeEnd event.
        $(window).resize(function () {
          if (this.resizeTO) {
            clearTimeout(this.resizeTO)
          }
          this.resizeTO = setTimeout(function () {
            $(this).trigger('resizeEnd')
          }, 300)
        })

        // Redraw graph when window resize is completed.
        $(window).on('resizeEnd', function () {
          drawChart()
        })
      })
    }
  }
})(jQuery, Drupal, drupalSettings)
