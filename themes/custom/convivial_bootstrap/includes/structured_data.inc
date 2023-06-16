<?php

/**
 * @file
 * Structured data.
 *
 * @see convivial_bootstrap_preprocess_html()
 * @see convivial_bootstrap_preprocess_paragraph()
 */

use Drupal\Core\Entity\FieldableEntityInterface;

/**
 * Attach structured data.
 */
function _convivial_bootstrap_attach_structured_data(FieldableEntityInterface $entity, array &$build, $attributes_key) {
  // Add FAQPage structured data attributes if field is enabled.
  $html_item_types = &drupal_static('convivial_bootstrap_html_item_types', []);
  if ($entity->bundle() === 'accordion' && !empty($entity->get('field_accordion_make_faq')->value)) {
    $html_item_types['FAQPage'] = 'https://schema.org/FAQPage';
  }
}
