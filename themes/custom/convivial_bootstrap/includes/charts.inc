<?php

/**
 * @file
 * Charts feature.
 *
 * @see convivial_bootstrap_preprocess_paragraph()
 * @see convivial_bootstrap_preprocess_ds_entity_view()
 */

use Drupal\Component\Utility\Html;
use Drupal\Core\Entity\FieldableEntityInterface;
use Drupal\Core\Url;

/**
 * Attach Charts feature.
 *
 * @param \Drupal\Core\Entity\FieldableEntityInterface $entity
 *   The entity with fields.
 * @param array $build
 *   The build array for rendering.
 * @param string $attributes_key
 *   The key inside build array where attributes are stored.
 */
function _convivial_bootstrap_charts(FieldableEntityInterface $entity, array &$build, $attributes_key) {
  $entity_type = $entity->getEntityTypeId();

  $charts_fields = [
    'type' => 'field_chrt_type',
    'stacking' => 'field_chrt_stacking',
    'stacking_order' => 'field_chrt_stack_order',
    'title' => 'field_chrt_title',
    'height' => 'field_chrt_height',
    'csv' => 'field_chrt_csv',
    'csv_url' => 'field_chrt_csv_url',
    'x_axis_type' => 'field_chrt_x_axis_type',
    'x_axis_label' => 'field_chrt_x_axis_label',
    'y_axis' => 'field_chrt_y_axis',
    'x_y_axis_switch' => 'field_chrt_x_y_axis_switch',
    'colors' => 'field_chrt_colors',
    'input_date_format' => 'field_chrt_date_time_format',
    'tick_date_format' => 'field_chrt_tick_date_time_format',
    'legend_position' => 'field_chrt_legend',
    'grid' => 'field_chrt_grid',
  ];

  // Get settings from chart fields.
  $field_values = [];
  foreach ($charts_fields as $key => $charts_field) {
    if (!$entity->hasField($charts_field)) {
      continue;
    }
    $field = $entity->get($charts_field);

    if (!$field->isEmpty()) {
      if ($field->count() === 1) {
        if ($key == 'csv_url') {
          $field_values[$key] = Url::fromUri($field->uri, ['absolute' => TRUE])->toString();
        }
        else {
          $field_values[$key] = $field->value;
        }
      }
      else {
        $values = [];
        foreach ($field->getValue() as $item) {
          if ($key == 'csv_url') {
            $values[] = Url::fromUri($field->uri, ['absolute' => TRUE])->toString();
          }
          else {
            $values[] = $item['value'];
          }
        }
        $field_values[$key] = $values;
      }
    }
    else {
      $field_values[$key] = NULL;

      if ($field->getName() === 'field_chrt_colors') {
        $default_colors = theme_get_setting('chart_colors');
        if (!empty($default_colors)) {
          $colors = explode(' ', $default_colors);
        }
        else {
          // If $colors is empty array, default colors are applied.
          $colors = [];
        }

        $field_values[$key] = $colors;
      }
    }
  }

  if (isset($field_values['csv']) || isset($field_values['csv_url'])) {
    // If there's only single color defined, convert it to array.
    if (is_string($field_values['colors'])) {
      $field_values['colors'] = [$field_values['colors']];
    }

    $chart_properties = [];
    foreach ($field_values as $name => $value) {
      $chart_properties[$name] = $value;
    }

    $settings = [
      'chart' => $chart_properties,
    ];
    // Create unique selector for current entity.
    $class = Html::getClass('billboard-' . $entity_type . '-' . $entity->id());
    $build[$attributes_key]['class'][] = $class;
    $selector = '.' . $class . ' .chart-billboard';

    // Attach libraries.
    $build['#attached']['library'][] = 'convivial_bootstrap/billboard-apply';

    // Attach all values for JS processing.
    $build['#attached']['drupalSettings']['billboard'][$selector] = $settings;
  }
}
