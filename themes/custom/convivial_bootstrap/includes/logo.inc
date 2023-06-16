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
  $selector = '.logo-block--' . $entity_id;
  // Create the variable for twig template.
  $variables['block_class'] = 'logo-block--' . $entity_id;

  // Get values from block fields.
  $width_large = $entity->get('field_logo_large_width')->value;
  $field_logo_breakpoint = $entity->get('field_logo_breakpoint');

  // Load the site name out of configuration. It's needed for default alt.
  $variables['site_name'] = \Drupal::config('system.site')->get('name');

  // Provide a default value or both logo sizes will appear together when the
  // breakpoint field is left empty.
  $width_breakpoint = !$field_logo_breakpoint->isEmpty() ?
    $field_logo_breakpoint->value : 768;

  $field_logo_small_default = $entity->get('field_logo_small_default');
  $field_logo_small_inverse = $entity->get('field_logo_small_inverse');

  $style = '';

  // We only include breakpoints if an alternative logo is available.
  $include_breakpoint = (
    !empty($width_breakpoint) &&
    (!$field_logo_small_default->isEmpty() || !$field_logo_small_inverse->isEmpty())
  );

  // Check if main value is available.
  if (!empty($width_large)) {
    // Get other field values.
    $width_small = $entity->get('field_logo_small_width')->value;

    // Create the style with both widths and breakpoint.
    if ($include_breakpoint && !empty($width_small)) {
      $style .= $selector . '{max-width:' . $width_small
        . 'px;}@media(min-width:' . $width_breakpoint . 'px){'
        . $selector . '{max-width:' . $width_large . 'px;}}';
    }
    else {
      // Create style just with the large width.
      $style .= $selector . '{max-width:' . $width_large . 'px;}';
    }
  }

  if ($include_breakpoint) {
    $style .= $selector . ' .logo-block__logo--small{display: block;}' . $selector
      . ' .logo-block__logo--large{display: none;}
          @media (min-width:' . $width_breakpoint . 'px){' . $selector
      . ' .logo-block__logo--small{display: none;}' . $selector
      . ' .logo-block__logo--large{display: block;}}';
  }

  // Print the style in head element.
  if (!empty($style)) {
    $variables['#attached']['html_head'][] = [
      [
        '#type' => 'html_tag',
        '#tag' => 'style',
        '#attributes' => [
          'media' => 'all',
          'data-logo-block' => $variables['block_class'],
        ],
        '#value' => Markup::create($style),
        '#weight' => 10,
      ],
      $variables['block_class'],
    ];
  }
}
