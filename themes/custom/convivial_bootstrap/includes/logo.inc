<?php

/**
 * @file
 * Logo feature.
 *
 * @see convivial_bootstrap_preprocess_block()
 */

use Drupal\Core\Render\Markup;

/**
 * Attach Logo feature.
 */
function _convivial_bootstrap_logo(&$variables, $entity) {
  $entity_id = $entity->id();
  // Specify the Block ID selector.
  $selector = '.logo--' . $entity_id;
  // Create the variable for twig template.
  $variables['block_class'] = 'logo--' . $entity_id;

  // Get values from block fields.
  $width = $entity->get('field_logo_width')->value;
  $field_logo_breakpoint = $entity->get('field_logo_breakpoint');

  // Load the site name out of configuration. It's needed for default alt.
  $variables['site_name'] = \Drupal::config('system.site')->get('name');

  // Provide a default value or both logo sizes will appear together when the
  // breakpoint field is left empty.
  $width_breakpoint = !$field_logo_breakpoint->isEmpty()
    ? $field_logo_breakpoint->value : 768;

  $field_logo_default_small = $entity->get('field_logo_default_small');
  $style = '';

  // We only include breakpoints if an alternative logo is available.
  $include_breakpoint = (!empty($width_breakpoint) && !$field_logo_default_small->isEmpty());

  // Check if main value is available.
  if (!empty($width)) {
    // Get other field values.
    $width_small = $entity->get('field_logo_width_small')->value;

    // Create the style with both widths and breakpoint.
    if ($include_breakpoint && !empty($width_small)) {
      $style .= $selector . '{max-width:' . $width_small
        . 'px;}@media(min-width:' . $width_breakpoint . 'px){' . $selector
        . '{max-width:' . $width . 'px;}}';
    }
    else {
      // Create style just with the large width.
      $style .= $selector . '{max-width:' . $width . 'px;}';
    }
  }

  if ($include_breakpoint) {
    $style .= $selector . ' .logo__small{display: block;}'
      . $selector . ' .logo__large{display: none;}
          @media (min-width:' . $width_breakpoint . 'px){' . $selector
      . ' .logo__small{display: none;}' . $selector
      . ' .logo__large{display: block;}}';
  }

  // Print the style in head element.
  if (!empty($style)) {
    $variables['#attached']['html_head'][] = [
      [
        '#type' => 'html_tag',
        '#tag' => 'style',
        '#attributes' => [
          'media' => 'all',
          'data-logo' => $variables['block_class'],
        ],
        '#value' => Markup::create($style),
        '#weight' => 10,
      ],
      $variables['block_class'],
    ];
  }
}
