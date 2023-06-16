<?php

/**
 * @file
 * Block feature.
 *
 * @see convivial_bootstrap_preprocess_field()
 */

use Drupal\block\Entity\Block;
use Drupal\Component\Utility\Html;
use Drupal\system\Plugin\Block\SystemMenuBlock;

/**
 * Attach Content Block feature.
 */
function _convivial_bootstrap_block_content(&$variables) {
  /** @var \Drupal\Core\Entity\FieldableEntityInterface $entity */
  $entity = $variables['element']['#object'];
  $referenced_entities = $entity->get('field_cb_block')->referencedEntities();

  if (!empty($referenced_entities)) {
    foreach ($referenced_entities as $entity) {
      $result = _convivial_bootstrap_block_build('block_content:' . $entity->uuid());
      if (!is_array($result)) {
        $result = t('Block ID is not valid.');
      }
    }
  }
  else {
    $result = t('No block found.');
  }

  $variables['content_block'] = $result;
}

/**
 * Attach Plugin Block feature.
 */
function _convivial_bootstrap_block_plugin(&$variables) {
  /** @var \Drupal\Core\Entity\FieldableEntityInterface $entity */
  $entity = $variables['element']['#object'];
  // Get the value of the field.
  $field_value = trim($entity->get('field_pb_definition')->value);

  // Only if field has value.
  if (!empty($field_value)) {
    $block = Block::load($field_value);

    // Only if block is available.
    if (!empty($block)) {
      // Build block render array.
      $result = _convivial_bootstrap_block_build($block->getPluginId(), $block->get('settings'));

      if (!is_array($result)) {
        $result = t('Block plugin ID is not valid.');
      }
    }
    else {
      $result = t('Block plugin ID is not valid.');
    }
  }
  else {
    $result = t('No definition found.');
  }

  $variables['plugin_block'] = $result;
}

/**
 * Builds the render array for a block.
 *
 * @param mixed $id
 *   The string of block plugin to render.
 * @param array $configuration
 *   (Optional) Pass on any configuration to the plugin block.
 * @param bool $wrapper
 *   (Optional) Whether or not use block template for rendering.
 *
 * @return null|array
 *   A render array for the block or NULL if the block cannot be rendered.
 */
function _convivial_bootstrap_block_build($id, array $configuration = [], $wrapper = TRUE) {
  $block_manager = \Drupal::service('plugin.manager.block');

  // Only if block is available.
  if (!$block_manager->hasDefinition($id)) {
    return NULL;
  }
  /** @var \Drupal\Core\Block\BlockPluginInterface $block_plugin */
  $block_plugin = $block_manager->createInstance($id, $configuration);

  // Empty result if user has no access to this block.
  if (!$block_plugin->access(\Drupal::currentUser())) {
    return [];
  }
  // Prepare content block render array.
  $content = $block_plugin->build();

  // Add block theme wrapper if needed.
  if ($wrapper) {
    $result = [
      '#theme' => 'block',
      '#attributes' => !empty($content['#attributes']) ? $content['#attributes'] : [],
      '#contextual_links' => !empty($content['#contextual_links']) ? $content['#contextual_links'] : [],
      '#configuration' => $block_plugin->getConfiguration(),
      '#plugin_id' => $block_plugin->getPluginId(),
      '#base_plugin_id' => $block_plugin->getBaseId(),
      '#derivative_plugin_id' => $block_plugin->getDerivativeId(),
      'content' => $content,
    ];
    // Add necessary menu block parameters.
    if ($block_plugin instanceof SystemMenuBlock) {
      $result['#id'] = Html::getUniqueId($id);
      $definition = $block_plugin->getPluginDefinition();
      $result['#configuration']['label'] = $definition['admin_label'];
      $result['#configuration']['label_display'] = FALSE;
    }
    return $result;
  }
  else {
    return $content;
  }
}
