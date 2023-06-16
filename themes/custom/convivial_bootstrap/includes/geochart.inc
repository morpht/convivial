<?php

function _convivial_bootstrap_geochart(&$variables) {
  $paragraph = $variables['paragraph'];
  $paragraph_id = $paragraph->id();
  $field_api_key = $paragraph->get('field_geochart_api_key');
  $field_background_color = $paragraph->get('field_geochart_background_color');
  $field_color_axis = $paragraph->get('field_geochart_color_axis');
  $field_csv = $paragraph->get('field_geochart_csv');
  $field_url = $paragraph->get('field_geochart_url');
  $field_dataless_color = $paragraph->get('field_geochart_dataless_color');
  $field_default_color = $paragraph->get('field_geochart_default_color');
  $field_display_mode = $paragraph->get('field_geochart_display_mode');
  $field_hide_legend = $paragraph->get('field_geochart_hide_legend');
  $field_region = $paragraph->get('field_geochart_region');
  $field_resolution = $paragraph->get('field_geochart_resolution');

  // Provide variables.
  $chart = [];
  $options = [];
  $csv_data = [];
  $url = !$field_url->isEmpty() ? $field_url->uri : NULL;
  $api_key = !$field_api_key->isEmpty() ? $field_api_key->value : NULL;

  if (!$field_csv->isEmpty()) {
    $csv_data = str_getcsv($field_csv->value, "\n");

    // Trim and convert string to array.
    for ($i = 0; $i < count($csv_data); ++$i) {
      $csv_data[$i] = array_map('trim', explode(',', $csv_data[$i]));

      // Convert string to numeric.
      for ($j = 0; $j < count($csv_data[$i]); ++$j) {
        if (is_numeric($csv_data[$i][$j])) {
          $csv_data[$i][$j] = intval($csv_data[$i][$j]);
        }
      }
    }
  }

  if (!$field_background_color->isEmpty()) {
    $options['backgroundColor'] = $field_background_color->value;
  }
  if (!$field_color_axis->isEmpty()) {

    // Trim and convert string to array.
    $options['colorAxis']['colors'] = array_map('trim', explode(',', $field_color_axis->value));
  }
  if (!$field_dataless_color->isEmpty()) {
    $options['datalessRegionColor'] = $field_dataless_color->value;
  }
  if (!$field_default_color->isEmpty()) {
    $options['defaultColor'] = $field_default_color->value;
  }
  if (!$field_display_mode->isEmpty()) {
    $options['displayMode'] = $field_display_mode->value;
  }
  if (!$field_hide_legend->isEmpty()) {
    if ($field_hide_legend->value == 1) {
      $options['legend'] = 'none';
    }
  }
  if (!$field_region->isEmpty()) {
    $options['region'] = $field_region->value;
  }
  if (!$field_resolution->isEmpty()) {
    $options['resolution'] = $field_resolution->value;
  }

  // Set chart data.
  $chart[$paragraph_id]['paragraphId'] = $paragraph_id;
  if ($api_key) {
    $chart[$paragraph_id]['apiKey'] = $api_key;
  }
  if ($csv_data) {
    $chart[$paragraph_id]['csvData'] = $csv_data;
  }
  if ($url) {
    $chart[$paragraph_id]['url'] = $url;
  }
  if ($options) {
    $chart[$paragraph_id]['options'] = $options;
  }

  // Attach drupal settings.
  $variables['#attached']['drupalSettings']['geoChart'] = $chart;

  // Attach library.
  $variables['#attached']['library'][] = 'convivial_bootstrap/geochart';
}
