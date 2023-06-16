<?php

/**
 * @file
 * Modifiers feature.
 *
 * @see convivial_bootstrap_preprocess_layout()
 */

use Drupal\Component\Utility\Html;
use Drupal\Core\Entity\FieldableEntityInterface;
use Drupal\Core\Field\EntityReferenceFieldItemListInterface;
use Drupal\Core\Field\FieldItemListInterface;
use Drupal\Core\Field\FieldStorageDefinitionInterface;
use Drupal\Core\Render\Element;

/**
 * Attach Layout Builder Modifiers feature.
 */
function _convivial_bootstrap_modifiers(&$variables) {
  $blocks = [];

  // Find all modifier blocks between regions.
  foreach (Element::children($variables['content']) as $region_key) {
    $block_keys = Element::children($variables['content'][$region_key]);

    // Search between all blocks inside specific region.
    foreach ($block_keys as $block_key) {
      $region_block = $variables['content'][$region_key][$block_key];

      // Only if block contains a custom block entity.
      if (isset($region_block['content']['#block_content'])) {
        /** @var \Drupal\block_content\Entity\BlockContent $entity */
        $entity = $region_block['content']['#block_content'];

        // Check if this block is of a modifier block type.
        if (substr($entity->bundle(), -9) === '_modifier') {
          // Apply to section if checkbox is checked.
          if ($entity->hasField('field_lb_modifiers_section')
            && !empty($entity->get('field_lb_modifiers_section')->value)
          ) {
            $blocks['section'][] = $entity;
          }
          else {
            // Apply this modifier to region.
            $blocks['region'][$region_key][] = $entity;
          }
        }
      }
    }
  }

  // Process all modifiers if found.
  if (!empty($blocks)) {
    // Get unique ID for every section with modifiers.
    $section_id = &drupal_static(__FUNCTION__, 0);
    $section_id++;

    // Process all section modifiers if available.
    if (!empty($blocks['section'])) {
      $entity_id = 'section-' . $section_id;
      _convivial_bootstrap_modifiers_apply($variables, $blocks['section'], $entity_id);
    }

    // Process all region modifiers if available.
    if (!empty($blocks['region'])) {
      foreach ($blocks['region'] as $region_key => $entities) {
        $entity_id = 'section-' . $section_id . '-region-' . $region_key;
        _convivial_bootstrap_modifiers_apply($variables, $entities, $entity_id, $region_key);
      }
    }
  }
}

/**
 * Apply modifications extracted from Layout Builder blocks.
 *
 * @param array $build
 *   The build array for rendering.
 * @param array|\Drupal\block_content\Entity\BlockContent[] $entities
 *   The set of modifier blocks.
 * @param string $entity_id
 *   The block entity ID.
 * @param string|null $region
 *   The region name or NULL for section.
 */
function _convivial_bootstrap_modifiers_apply(array &$build, $entities, $entity_id, $region = NULL) {
  // Initialize modifiers service.
  /** @var \Drupal\modifiers\Modifiers $modifiers */
  $modifiers = \Drupal::service('modifiers');

  // Compose selector and config to field mapping.
  $selector = '.modifiers.modifiers-id-' . $entity_id;
  $config = [];

  // Extract every entity configuration.
  foreach ($entities as $entity) {
    $entity_config = [];

    // Extract configuration from every field.
    foreach ($entity->getFields() as $field_name => $field) {
      /** @var \Drupal\Core\Field\FieldItemListInterface $field */
      $storage = $field->getFieldDefinition()->getFieldStorageDefinition();

      // Skip entity base fields.
      if (!$storage->isBaseField()) {

        // Get simple value from field with referenced entity.
        if ($field instanceof EntityReferenceFieldItemListInterface) {
          $value = _convivial_bootstrap_modifiers_referenced_value($field, $storage);
        }
        else {
          // Otherwise get value from simple field.
          $value = _convivial_bootstrap_modifiers_simple_value($field, $storage);
        }

        // Fill field value into referenced entity config array.
        $field_short = $modifiers->getShortField($field_name);
        $entity_config[$field_short] = $value;
      }
    }

    if (!empty($entity_config)) {
      $config[$entity->bundle()][] = $entity_config;
    }
  }

  $modifications = [];
  // Fill all modifications.
  $modifiers->process($modifications, $config, $selector);

  // Apply modifications if available.
  if (!empty($modifications)) {
    if (!empty($region)) {
      $build['region_attributes'][$region]->addClass([
        'modifiers',
        Html::getClass('modifiers-id-' . $entity_id),
      ]);
    }
    else {
      $build['attributes']['class'][] = 'modifiers';
      $build['attributes']['class'][] = Html::getClass('modifiers-id-' . $entity_id);
    }
    $modifiers->apply($modifications, $build, $entity_id);
  }
}

/**
 * Gets value from complex field with referenced entities.
 *
 * @param \Drupal\Core\Field\EntityReferenceFieldItemListInterface $field
 *   The field object referencing other entities.
 * @param \Drupal\Core\Field\FieldStorageDefinitionInterface $field_storage
 *   The field storage definition object.
 *
 * @return array|mixed|null
 *   The array of values, single value or null if empty.
 */
function _convivial_bootstrap_modifiers_referenced_value(EntityReferenceFieldItemListInterface $field, FieldStorageDefinitionInterface $field_storage) {

  // Only if some value exists.
  if (!$field->isEmpty()) {
    // Define mappings from entity type and bundle to field names.
    $mappings = [
      'media' => [
        'audio' => ['field_media_audio_file'],
        'file' => ['field_media_file'],
        'image' => ['field_media_image', 'image', 'field_file'],
        'remote_video' => [
          'field_media_video_embed_field',
          'field_media_oembed_video',
        ],
        'video' => [
          'field_media_video_file',
          'field_media_video_embed_field',
          'field_file',
        ],
        'video_embed' => ['field_media_video_embed_field', 'field_file'],
      ],
      'taxonomy_term' => [
        'modifiers_color' => ['field_mod_color'],
      ],
    ];
    // Allow other modules to alter these mappings.
    \Drupal::moduleHandler()->alter('modifiers_mappings', $mappings);

    $values = [];
    // Process all referenced entities.
    foreach ($field->referencedEntities() as $entity) {
      $type = $entity->getEntityTypeId();
      $bundle = $entity->bundle();

      // Only if this type has some mappings.
      if ($entity instanceof FieldableEntityInterface && isset($mappings[$type][$bundle])) {

        // Find first existing field by name.
        foreach ($mappings[$type][$bundle] as $field_name) {
          if ($entity->hasField($field_name)) {
            // Only if some value exists.
            if (!$entity->get($field_name)->isEmpty()) {
              /** @var \Drupal\Core\Field\EntityReferenceFieldItemListInterface $entity_field */
              $entity_field = $entity->get($field_name);
              break;
            }
          }
        }

        // Only for entities with mapped field.
        if (!empty($entity_field)) {
          $entity_field_storage = $entity_field->getFieldDefinition()
            ->getFieldStorageDefinition();

          // Get value by different field types.
          switch ($entity_field_storage->getType()) {

            case 'color_field_type':
              foreach ($entity_field->getValue() as $item) {
                $values[] = _convivial_bootstrap_modifiers_color_value($item['color'], $item['opacity']);
              }
              break;

            case 'file':
            case 'image':
              /** @var \Drupal\file\Entity\File $file */
              foreach ($entity_field->referencedEntities() as $file) {
                $values[] = $file->createFileUrl();
              }
              break;

            default:
              foreach ($entity_field->getValue() as $item) {
                $values[] = $item[$entity_field_storage->getMainPropertyName()];
              }
              break;
          }
        }
      }
    }
    // Return simple value or array of values.
    if (!empty($values)) {
      return $field_storage->isMultiple() ? $values : $values[0];
    }
  }
  // Return null if there is no value.
  return NULL;
}

/**
 * Gets value from simple field.
 *
 * @param \Drupal\Core\Field\FieldItemListInterface $field
 *   The field object containing values.
 * @param \Drupal\Core\Field\FieldStorageDefinitionInterface $field_storage
 *   The field storage definition object.
 *
 * @return array|mixed|null
 *   The array of values, single value or null if empty.
 */
function _convivial_bootstrap_modifiers_simple_value(FieldItemListInterface $field, FieldStorageDefinitionInterface $field_storage) {

  // Only if some value exists.
  if (!$field->isEmpty()) {
    $field_type = $field_storage->getType();
    $property_name = $field_storage->getMainPropertyName();
    $values = [];

    // Process all field values.
    foreach ($field->getValue() as $item) {

      // Specific handling for color field.
      if ($field_type === 'color_field_type') {
        $values[] = _convivial_bootstrap_modifiers_color_value($item['color'], $item['opacity']);
      }
      else {
        // Get value by main property.
        $values[] = $item[$property_name];
      }
    }
    // Return simple value or array of values.
    return $field_storage->isMultiple() ? $values : $values[0];
  }
  // Return null if there is no value.
  return NULL;
}

/**
 * Gets RGBA color string from color and opacity.
 *
 * @param string $color
 *   The hexadecimal color value.
 * @param string $opacity
 *   The decimal opacity value.
 *
 * @return string
 *   The RGBA value.
 */
function _convivial_bootstrap_modifiers_color_value($color, $opacity) {

  // Clean provided values.
  $hex = trim($color);
  $opacity = floatval($opacity);

  // Validate values to match hexadecimal format.
  if (substr($hex, 0, 1) === '#') {
    $hex = substr($hex, 1);
  }
  if (strlen($hex) === 3) {
    $hex = $hex[0] . $hex[0] . $hex[1] . $hex[1] . $hex[2] . $hex[2];
  }
  if (!preg_match('/[0-9A-F]{6}/i', $hex)) {
    return '';
  }
  // Convert hexadecimal string to decimal.
  [$red, $green, $blue] = sscanf($hex, "%02x%02x%02x");

  // Join provided values into single RGBA string.
  return 'rgba(' . $red . ',' . $green . ',' . $blue . ',' . $opacity . ')';
}
