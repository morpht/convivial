<?php

/**
 * @file
 * Slideshow feature.
 *
 * @see convivial_bootstrap_preprocess_block()
 * @see convivial_bootstrap_preprocess_paragraph()
 * @see convivial_bootstrap_preprocess_ds_entity_view()
 */

use Drupal\Component\Utility\Html;
use Drupal\Core\Entity\FieldableEntityInterface;

/**
 * Attach Slideshow feature.
 */
function _convivial_bootstrap_slideshow(FieldableEntityInterface $entity, array &$build, $attributes_key) {
  $entity_type = $entity->getEntityTypeId();

  // Get the ratio field value.
  if (!$entity->get('field_slideshow_ratio')->isEmpty()) {

    // Create unique selector for current entity.
    $class = Html::getClass('slideshow-' . $entity_type . '-' . $entity->id());
    $build[$attributes_key]['class'][] = $class;

    // Attach value of field ratio for JS processing.
    $selector = '.' . $class;
    $ratio = floatval($entity->get('field_slideshow_ratio')->value);
    $build['#attached']['drupalSettings']['slideshow'][$selector] = $ratio;
  }
}
