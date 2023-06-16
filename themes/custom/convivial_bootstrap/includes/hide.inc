<?php

/**
 * @file
 * Hide feature.
 *
 * @see convivial_bootstrap_preprocess_block()
 * @see convivial_bootstrap_preprocess_paragraph()
 */

use Drupal\Core\Entity\FieldableEntityInterface;

/**
 * Attach Hide Heading feature.
 *
 * Add class to hide entity heading if enabled.
 */
function _convivial_bootstrap_hide_heading(FieldableEntityInterface $entity, array &$build, $attributes_key) {
  // Add class to entity if the field_hide_heading is enabled.
  if ($entity->hasField('field_hide_heading') &&
    !empty($entity->get('field_hide_heading')->value)) {
    $build[$attributes_key]['class'][] = 'hide-heading';
  }
}
