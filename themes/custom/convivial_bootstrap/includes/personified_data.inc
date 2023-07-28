<?php

/**
 * @file
 * Personified settings.
 *
 * @see _convivial_bootstrap_preprocess_block()
 */

use Drupal\paragraphs\Entity\Paragraph;

function _convivial_bootstrap_personified_data(&$variables, $content) {
  $personified_id = 'personified-data-' . $content->id();
  $endpoint = $content->get('field_personified_endpoint') ? $content->get('field_personified_endpoint')->value : NULL;
  $parameters = $content->get('field_personified_parameters') ? $content->get('field_personified_parameters')->getValue() : NULL;
  $template_name = $content->get('field_personified_template') ? $content->get('field_personified_template')->value : NULL;
  $template_path = $variables['theme_path'] . '/handlebars/' . $template_name . '.html.hbs';
  $template = file_get_contents($template_path) != FALSE ? file_get_contents($template_path) : NULL;
  $vertical_spacing = $content->get('field_vertical_spacing') ? $content->get('field_vertical_spacing')->value : NULL;
  $params = [];

  if ($parameters) {
    foreach ($parameters as $key => $parameter) {
      $parameter_paragraph = Paragraph::load($parameter['target_id']);
      if ($parameter_paragraph) {
        $params[$key] = [
          'default_value' => $parameter_paragraph->get('field_parameter_default_value') ? $parameter_paragraph->get('field_parameter_default_value')->value : NULL,
          'endpoint_key'  => $parameter_paragraph->get('field_parameter_endpoint_key') ? $parameter_paragraph->get('field_parameter_endpoint_key')->value : NULL,
          'source_key'    => $parameter_paragraph->get('field_parameter_source_key') ? $parameter_paragraph->get('field_parameter_source_key')->value : NULL,
          'source_type'   => $parameter_paragraph->get('field_parameter_source_type') ? $parameter_paragraph->get('field_parameter_source_type')->value : NULL,
        ];
      }
    }
  }

  // Construct variables.
  $personified_data = [
    'endpoint' => $endpoint,
    'template' => $template,
    'params'   => $params,
  ];

  // Attached variables.
  $variables['personified'] = [
    'id'               => $personified_id,
    'vertical_spacing' => $vertical_spacing,
  ];

  // Attached drupal settings.
  $variables['#attached']['drupalSettings']['personifiedData'][$personified_id] = $personified_data;

  // Attached library.
  $variables['#attached']['library'][] = 'convivial_bootstrap/personified_data';
}
