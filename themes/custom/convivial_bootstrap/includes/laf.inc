<?php

/**
 * @file
 * Look and Feel feature.
 *
 * @see convivial_bootstrap_preprocess_html()
 */

use Drupal\Component\Utility\Html;
use Drupal\Core\Cache\Cache;
use Drupal\Core\Entity\FieldableEntityInterface;
use Drupal\Core\Field\EntityReferenceFieldItemListInterface;

/**
 * Attach Look and Feel feature.
 */
function _convivial_bootstrap_laf(&$variables) {
  // Get configured selectors and skip if empty.
  $selectors = _convivial_bootstrap_laf_selectors();
  if (empty($selectors)) {
    return;
  }
  $looks = [];

  // Get configured strategies.
  $completes = _convivial_bootstrap_laf_strategies('laf_completes');
  $partials = _convivial_bootstrap_laf_strategies('laf_partials');

  // Search for first matching look.
  foreach ($completes as $strategy) {
    $look = _convivial_bootstrap_laf_look($strategy['id'], $strategy['params']);
    // Add only if found and already not there.
    if (!empty($look) && empty($looks[$look['id']])) {
      $looks[$look['id']] = $look;
      // Skip other strategies.
      break;
    }
  }

  // Search for all matching looks.
  foreach ($partials as $strategy) {
    $look = _convivial_bootstrap_laf_look($strategy['id'], $strategy['params']);
    // Add only if found and already not there.
    if (!empty($look) && empty($looks[$look['id']])) {
      $looks[$look['id']] = $look;
    }
  }

  // Initialize modifiers service.
  /** @var \Drupal\modifiers\Modifiers $modifiers */
  $modifiers = \Drupal::service('modifiers');
  // Attach class if some look applied.
  if (!empty($looks)) {
    $variables['attributes']['class'][] = 'laf';
  }

  // Process all available looks.
  foreach ($looks as $look) {
    $modifications = [];

    // Collect all modifications.
    foreach ($selectors as $field => $selector) {
      $namespace = $modifiers->getShortField($field);
      // Only if namespace is available.
      if (isset($look['config'][$namespace])) {
        // Fill current namespace modifications.
        $modifiers->process($modifications, $look['config'][$namespace], $selector);
      }
    }
    // Apply all modifications.
    if (!empty($modifications)) {
      $modifiers->apply($modifications, $variables, 'laf-' . $look['id']);
    }
    // Attach applied look class.
    $variables['attributes']['class'][] = Html::getClass('laf--' . $look['id']);
  }
}

/**
 * Gets configured selector mappings.
 *
 * @return array
 *   The array of selectors keyed by field.
 */
function _convivial_bootstrap_laf_selectors() {
  $result = [];
  // Get configured value and split by lines.
  $value = theme_get_setting('laf_selectors');
  $lines = explode(PHP_EOL, $value);

  // Get mappings from every line.
  foreach ($lines as $line) {
    $mapping = _convivial_bootstrap_laf_explode_trim('|', $line);
    // Only if both parts are not empty.
    if (!empty($mapping[0]) && !empty($mapping[1])) {
      $result[$mapping[0]] = $mapping[1];
    }
  }
  return $result;
}

/**
 * Gets configured strategy mappings.
 *
 * @param string $type
 *   The strategy group variable.
 *
 * @return array
 *   The array of strategies with ID and parameters.
 */
function _convivial_bootstrap_laf_strategies($type) {
  $result = [];
  // Get configured value and split by lines.
  $value = theme_get_setting($type);
  $lines = explode(PHP_EOL, $value);

  // Get mappings from every line.
  foreach ($lines as $line) {
    $mapping = explode('|', $line);
    // Only if both parts are not empty.
    if (!empty($mapping[0]) && !empty($mapping[1])) {
      $result[] = [
        'id' => trim($mapping[0]),
        'params' => _convivial_bootstrap_laf_explode_trim(',', $mapping[1]),
      ];
    }
  }
  return $result;
}

/**
 * Gets look using recognition strategy.
 *
 * @param string $strategy
 *   The strategy name.
 * @param array $params
 *   The strategy parameters.
 *
 * @return array|null
 *   The look array or null if not found.
 */
function _convivial_bootstrap_laf_look($strategy, array $params) {
  // Search entity by different strategies.
  switch ($strategy) {
    case 'entity':
      $entity = _convivial_bootstrap_laf_look_entity($params);
      break;

    case 'reference':
      $entity = _convivial_bootstrap_laf_look_reference($params);
      break;

    case 'path':
      $entity = _convivial_bootstrap_laf_look_path($params);
      break;

    case 'apply':
      $entity = _convivial_bootstrap_laf_look_apply($params);
      break;
  }
  // Only if look entity found.
  if (!empty($entity)) {
    // Get entity tag and cache item ID.
    $tag = $entity->getEntityTypeId() . ':' . $entity->id();
    $cid = 'laf:' . $tag;
    /** @var \Drupal\Core\Cache\CacheBackendInterface $cache_service */
    $cache_service = \Drupal::service('cache.default');

    // Get look from cache if available.
    if ($cache = $cache_service->get($cid)) {
      return $cache->data;
    }
    else {
      // Prepare look to be cached.
      $config = [
        'id' => $entity->getEntityTypeId() . '-' . $entity->id(),
        'config' => _convivial_bootstrap_laf_extract_config($entity),
      ];
      // Set new cache item with entity type and ID tag.
      $cache_service->set($cid, $config, Cache::PERMANENT, [$tag]);
      // Get recently cached look.
      return $config;
    }
  }
  return NULL;
}

/**
 * Gets current page entity as look.
 *
 * @param array $params
 *   The strategy parameters.
 *
 * @return \Drupal\Core\Entity\FieldableEntityInterface|null
 *   The look entity or null if empty.
 */
function _convivial_bootstrap_laf_look_entity(array $params) {
  /** @var \Symfony\Component\HttpFoundation\Request $request */
  $request = \Drupal::service('request_stack')->getCurrentRequest();
  $attributes = $request->attributes;
  $route_name = $attributes->get('_route');
  $pattern = '/^entity\.([^.]+)\.(canonical|latest_version)$/';
  preg_match($pattern, $route_name, $matches);

  // Only for full entity route of specified entity type.
  if (!empty($matches[1]) && in_array($matches[1], $params)) {
    $entity = $attributes->get($matches[1]);
    // Only if entity is available.
    if (!empty($entity) && $entity instanceof FieldableEntityInterface) {
      return $entity;
    }
  }
  return NULL;
}

/**
 * Gets look from current page entity.
 *
 * @param array $params
 *   The strategy parameters.
 *
 * @return \Drupal\Core\Entity\FieldableEntityInterface|null
 *   The look entity or null if empty.
 */
function _convivial_bootstrap_laf_look_reference(array $params) {
  // Only if parameter is available.
  if (!empty($params[0])) {
    /** @var \Symfony\Component\HttpFoundation\Request $request */
    $request = \Drupal::service('request_stack')->getCurrentRequest();
    $attributes = $request->attributes;
    $route_name = $attributes->get('_route');
    $pattern = '/^entity\.([^.]+)\.(canonical|latest_version)$/';
    preg_match($pattern, $route_name, $matches);

    // Only for full entity route.
    if (!empty($matches[1])) {
      $entity = $attributes->get($matches[1]);
      // Only if entity is available.
      if (!empty($entity) && $entity instanceof FieldableEntityInterface) {
        // Check if field is available.
        if ($entity->hasField($params[0])) {
          $field = $entity->get($params[0]);

          // Only if field is supported.
          if ($field instanceof EntityReferenceFieldItemListInterface) {
            $entities = $field->referencedEntities();
            // Provide entity if available.
            if (!empty($entities)) {
              return array_pop($entities);
            }
          }
        }
      }
    }
  }
  return NULL;
}

/**
 * Gets look from request path.
 *
 * @param array $params
 *   The strategy parameters.
 *
 * @return \Drupal\Core\Entity\FieldableEntityInterface|null
 *   The look entity or null if empty.
 */
function _convivial_bootstrap_laf_look_path(array $params) {
  // Only if parameters are available.
  if (!empty($params[0]) && !empty($params[1]) && !empty($params[2])) {
    /** @var \Drupal\Core\Condition\ConditionManager $condition_service */
    $condition_service = \Drupal::service('plugin.manager.condition');
    /** @var \Drupal\system\Plugin\Condition\RequestPath $path_check */
    $path_check = $condition_service->createInstance('request_path');

    // Only if path matches current request path.
    if ($path_check->setConfig('pages', $params[0])->execute()) {
      // Search look by its key.
      return _convivial_bootstrap_laf_look_by_key($params[1], $params[2]);
    }
  }
  return NULL;
}

/**
 * Gets specified look.
 *
 * @param array $params
 *   The strategy parameters.
 *
 * @return \Drupal\Core\Entity\FieldableEntityInterface|null
 *   The look entity or null if empty.
 */
function _convivial_bootstrap_laf_look_apply(array $params) {
  // Only if parameters are available.
  if (!empty($params[0]) && !empty($params[1])) {
    // Search look by its key.
    return _convivial_bootstrap_laf_look_by_key($params[0], $params[1]);
  }
  return NULL;
}

/**
 * Gets look by its key.
 *
 * @param string $type
 *   The look entity type.
 * @param string $key
 *   The look entity key.
 *
 * @return \Drupal\Core\Entity\FieldableEntityInterface|null
 *   The look entity or null if not found.
 */
function _convivial_bootstrap_laf_look_by_key($type, $key) {
  // Only if parameters are available.
  if (!empty($type) && !empty($key)) {
    $entity_manager = \Drupal::entityTypeManager();
    // Only if entity type exists.
    if ($entity_manager->hasHandler($type, 'storage')) {

      // Search entity by key.
      $entities = $entity_manager->getStorage($type)->loadByProperties([
        'field_laf_key' => $key,
      ]);
      // Provide entity if available.
      if (!empty($entities)) {
        return array_pop($entities);
      }
    }
  }
  return NULL;
}

/**
 * Extracts configuration from look entity.
 *
 * @param \Drupal\Core\Entity\FieldableEntityInterface $entity
 *   The look entity.
 *
 * @return array
 *   The configuration array.
 */
function _convivial_bootstrap_laf_extract_config(FieldableEntityInterface $entity) {
  $config = [];
  if (!empty($entity)) {
    /** @var \Drupal\modifiers\Modifiers $modifiers */
    $modifiers = \Drupal::service('modifiers');
    // Process all fields.
    foreach ($entity->getFields() as $field_name => $field) {
      /** @var \Drupal\Core\Field\FieldItemListInterface $field */
      $storage = $field->getFieldDefinition()->getFieldStorageDefinition();
      // Extract values only from configurable fields.
      if (!$storage->isBaseField()) {

        // Only if field starts with "field_laf_" or ends with "_modifiers".
        if (preg_match('/(^field_laf_|_modifiers$)/', $field_name)) {
          $config = $modifiers->extractEntityConfig($entity, $field_name, $config);
        }
      }
    }
  }
  return $config;
}

/**
 * Splits string by delimiter and trims all parts.
 *
 * @param string $delimiter
 *   The boundary string.
 * @param string $string
 *   The input string.
 *
 * @return array
 *   The array of trimmed string parts.
 */
function _convivial_bootstrap_laf_explode_trim($delimiter, $string) {
  $result = explode($delimiter, $string);
  return empty($result) ? $result : array_map('trim', $result);
}
