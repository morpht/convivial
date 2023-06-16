<?php

/**
 * @file
 * Map feature.
 *
 * @see convivial_bootstrap_preprocess_paragraph()
 * @see convivial_bootstrap_preprocess_ds_entity_view()
 */

use Drupal\Component\Utility\Html;
use Drupal\Core\Entity\FieldableEntityInterface;

/**
 * Attach Map feature.
 *
 * @param \Drupal\Core\Entity\FieldableEntityInterface $entity
 *   The entity with fields.
 * @param array $build
 *   The build array for rendering.
 * @param string $attributes_key
 *   The key inside build array where attributes are stored.
 */
function _convivial_bootstrap_map(FieldableEntityInterface $entity, array &$build, $attributes_key) {
  $entity_type = $entity->getEntityTypeId();

  // Fill values if not empty.
  $values = [];
  if ($entity->hasField('field_map_geojson_text')) {
    foreach ($entity->get('field_map_geojson_text')->getValue() as $item) {
      $values['text'][] = $item['value'];
    }
  }
  if ($entity->hasField('field_map_geojson_url')) {
    foreach ($entity->get('field_map_geojson_url')->getValue() as $item) {
      $values['url'][] = $item['value'];
    }
  }
  if ($entity->hasField('field_map_ratio')) {
    foreach ($entity->get('field_map_ratio')->getValue() as $item) {
      $values['ratio'][] = $item['value'];
    }
  }

  if (!empty($values)) {
    // Create unique selector for current entity.
    $class = Html::getClass('map-' . $entity_type . '-' . $entity->id());
    $build[$attributes_key]['class'][] = $class;

    // Attach libraries.
    $build['#attached']['library'][] = 'convivial_bootstrap/leaflet-apply';

    // Attach all values for JS processing.
    $selector = '.' . $class . ' .map__container';
    $build['#attached']['drupalSettings']['map'][$selector] = $values;
  }
}
