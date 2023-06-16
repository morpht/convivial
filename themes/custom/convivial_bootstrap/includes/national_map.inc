<?php

/**
 * @file
 * National Map feature.
 *
 * @see convivial_bootstrap_preprocess_paragraph()
 * @see convivial_bootstrap_preprocess_ds_entity_view()
 */

use Drupal\Component\Utility\Html;
use Drupal\Core\Entity\FieldableEntityInterface;
use Drupal\paragraphs\Entity\Paragraph;

/**
 * Attach National Map feature.
 *
 * @param \Drupal\Core\Entity\FieldableEntityInterface $entity
 *   The entity with fields.
 * @param array $build
 *   The build array for rendering.
 * @param string $attributes_key
 *   The key inside build array where attributes are stored.
 */
function _convivial_bootstrap_national_map(FieldableEntityInterface $entity, array &$build, $attributes_key) {
  $entity_type = $entity->getEntityTypeId();

  // Build catalogs.
  $catalogs = [];
  foreach ($entity->field_nm_catalogs->getValue() as $catalog_ref) {
    // Get catalog.
    $catalog_id = $catalog_ref['target_id'];
    $catalog = Paragraph::load($catalog_id);
    // Build catalog.
    $catalog_build = [
      'type' => 'group',
      'name' => _convivial_bootstrap_national_map_get_value($catalog->field_nmc_name),
      'open' => (boolean) _convivial_bootstrap_national_map_get_value($catalog->field_nmc_open),
      'promoted' => (boolean) _convivial_bootstrap_national_map_get_value($catalog->field_nmc_promoted),
    ];
    // Go through catalog items.
    foreach ($catalog->field_nmc_items->getValue() as $item_ref) {

      // Get catalog item.
      $item_id = $item_ref['target_id'];
      $item = Paragraph::load($item_id);
      // Get catalog item values.
      $url = _convivial_bootstrap_national_map_get_url($item->field_nmci_url);
      $data = _convivial_bootstrap_national_map_get_value($item->field_nmci_data);

      // Don't add catalog item if both url and data empty.
      if (!empty($url) || !empty($data)) {
        // Build catalog item.
        $catalog_build['items'][] = [
          'name' => _convivial_bootstrap_national_map_get_value($item->field_nmci_name),
          'type' => _convivial_bootstrap_national_map_get_value($item->field_nmci_type),
          'data' => $data,
          'url' => $url,
          'isEnabled' => TRUE,
          'zoomOnEnable' => (boolean) _convivial_bootstrap_national_map_get_value($item->field_nmci_zoom),
        ];
      }

      // Add catalog item to parent catalog's items.
      $catalogs[] = $catalog_build;
    }
  }

  // Add catalogs to drupalSettings for acccess via JS.
  if (!empty($catalogs)) {

    // Create unique selector for current entity.
    $id = Html::getId('national-map-' . $entity_type . '-' . $entity->id());
    $build[$attributes_key]['id'][] = $id;

    // Pass map height to theme.
    $build['content']['nm_url'] = _convivial_bootstrap_national_map_get_url($entity->field_nm_url);
    $build['content']['nm_height'] = _convivial_bootstrap_national_map_get_value($entity->field_nm_height);

    // Attach client side library.
    $build['#attached']['library'][] = 'convivial_bootstrap/national-map';

    // Make catalog data available client side. Added by id to prevent overwriting values.
    $build['#attached']['drupalSettings']['national_map'][$id] = [
      'id' => $id,
      'catalogs' => $catalogs,
    ];
  }
}

/**
 * Get a field's value.
 */
function _convivial_bootstrap_national_map_get_value($field) {
  // Check that field has an item.
  if ($field->first() != NULL) {
    // Check that item has a value.
    $item = $field->first()->getValue();
    if (!empty($item)) {
      // Get value.
      return $item['value'];
    }
  }
  return NULL;
}

/**
 * Get a field's url.
 */
function _convivial_bootstrap_national_map_get_url($field) {
  // Check that field has an item.
  if ($field->first() != NULL) {
    // Check that item has a value.
    $item = $field->first()->getValue();
    if (!empty($item)) {
      // Get value.
      return $item['uri'];
    }
  }
  return NULL;
}
