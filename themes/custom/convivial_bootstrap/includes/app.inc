<?php

/**
 * @file
 * App feature.
 *
 * @see convivial_bootstrap_preprocess_paragraph()
 * @see convivial_bootstrap_preprocess_ds_entity_view()
 */

use Drupal\Component\Utility\Html;
use Drupal\Core\Entity\FieldableEntityInterface;

/**
 * Attach App feature.
 */
function _convivial_bootstrap_app(FieldableEntityInterface $entity, array &$build, $attributes_key) {
  $entity_type = $entity->getEntityTypeId();
  $args = [];

  // Create app class with entity ID.
  $class = Html::getClass('app-' . $entity_type . '-' . $entity->id());
  $build[$attributes_key]['class'][] = $class;

  // Attach app CSS if available.
  $url = _convivial_bootstrap_get_file_url($entity, 'field_app_css_file');
  if (!empty($url)) {
    $args['css'] = $url;
  }

  // Attach app JS if available.
  $url = _convivial_bootstrap_get_file_url($entity, 'field_app_js_file');
  if (!empty($url)) {
    $args['js'] = $url;

    // Specify namespace with init callback.
    if ($entity->hasField('field_app_js_namespace')) {
      $namespace = $entity->get('field_app_js_namespace')->value;
      if (!empty($namespace)) {
        $args['namespace'] = $namespace;

        // Attach app config.
        $args['config'] = [
          'title' => $entity->label(),
          'base_url' => $GLOBALS['base_url'],
        ];
      }
    }
  }

  // Attach resources if available.
  foreach ($entity->get('field_app_resources')->getValue() as $item) {
    $args['resources'][] = $item['value'];
  }

  if (isset($args['css']) || isset($args['js']) || isset($args['resources'])) {
    // Attach initialization script for hosted apps.
    $build['#attached']['library'][] = 'convivial_bootstrap/app';

    // Attach app config for specific selector.
    $selector = '.' . $class . ' .field--name-field-app-html';
    $build['#attached']['drupalSettings']['app'][$selector] = $args;
  }
}
