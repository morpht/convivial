<?php

/**
 * @file
 * Meta functionality for the Convivial theme.
 */

use Drupal\Component\Serialization\Json;
use Drupal\Core\Field\EntityReferenceFieldItemListInterface;

/**
 * Initializes Meta behavior.
 */
function _convivial_bootstrap_meta(&$attachments) {
  // Get configured strategies and skip if empty.
  $strategies = _convivial_bootstrap_meta_strategies();
  if (empty($strategies)) {
    return;
  }
  /** @var \Drupal\node\Entity\Node $node */
  $node = \Drupal::routeMatch()->getParameters()->get('node');
  // Skip for other than node entity.
  if (!is_object($node)) {
    return;
  }

  // Process all strategies.
  foreach ($strategies as $strategy) {
    $values = [];

    // Skip if limited to another bundles.
    if (!empty($strategy['bundles'])
      && !in_array($node->bundle(), $strategy['bundles'])
    ) {
      continue;
    }

    switch ($strategy['id']) {

      case 'token':
        if (!empty($strategy['params'])) {
          [$text] = $strategy['params'];
          $values[] = \Drupal::token()->replace($text, ['node' => $node], [
            'langcode' => $node->language()->getId(),
            'clear' => TRUE,
          ]);
        }
        break;

      case 'references':
      case 'reference':
        if (!empty($strategy['params']) && count($strategy['params']) > 1) {
          [$field_name_1, $field_name_2] = $strategy['params'];
          if ($node->hasField($field_name_1)) {
            $field_1 = $node->get($field_name_1);
            if ($field_1 instanceof EntityReferenceFieldItemListInterface) {

              /** @var \Drupal\Core\Entity\FieldableEntityInterface $entity */
              foreach ($field_1->referencedEntities() as $entity) {
                if ($entity->hasField($field_name_2)) {
                  foreach ($entity->get($field_name_2)->getValue() as $item) {
                    $values[] = $item['value'];
                  }
                }
              }
            }
          }
        }
        break;

      case 'values':
      case 'value':
        if (!empty($strategy['params'])) {
          [$field_name] = $strategy['params'];
          if ($node->hasField($field_name)) {
            foreach ($node->get($field_name)->getValue() as $item) {
              $values[] = $item['value'];
            }
          }
        }
        break;

      case 'bundle':
        $values[] = $node->bundle();
        break;

      case 'constant':
        if (!empty($strategy['params'])) {
          [$value] = $strategy['params'];
          $values[] = $value;
        }
        break;
    }

    if (!empty($values)) {
      // Create a meta tag for each value.
      foreach ($values as $index => $value) {
        // Build attributes.
        $attributes = [
          $strategy['type'] => $strategy['key'],
          'content' => $value,
        ];
        if (!empty($strategy['attribs'])) {
          $attributes = array_merge($attributes, $strategy['attribs']);
        }
        // Add attributes as a meta tag.
        $attachments['#attached']['html_head'][] = [
          [
            '#type' => 'html_tag',
            '#tag' => 'meta',
            '#attributes' => $attributes,
          ],
          // Append unique value to strategy key to allow duplicate meta tags with same property.
          implode('_', ['meta', $strategy['type'], $strategy['key'] . $index]),
        ];
      }
    }
  }
}

/**
 * Gets configured meta strategy mappings.
 *
 * @return array
 *   The array of strategies with ID and parameters.
 */
function _convivial_bootstrap_meta_strategies() {
  $result = [];
  // Search for every meta type configuration.
  foreach (['name', 'property'] as $type) {
    // Get configured value and split by lines.
    $value = theme_get_setting('meta.meta_strategies_' . $type);
    $lines = explode(PHP_EOL, $value);

    // Get mappings from every line.
    foreach ($lines as $line) {
      $mapping = _convivial_bootstrap_meta_explode_trim(',', $line, 5);
      // Only if required parts are not empty.
      if (!empty($mapping[0]) && !empty($mapping[1])) {
        $strategy = [
          'type' => $type,
          'id' => $mapping[0],
          'key' => $mapping[1],
        ];
        if (!empty($mapping[2])) {
          $strategy['params'] = _convivial_bootstrap_meta_explode_trim('|', $mapping[2]);
        }
        if (!empty($mapping[3])) {
          $attribs = _convivial_bootstrap_meta_explode_trim('|', $mapping[3]);
          foreach ($attribs as $attrib) {
            $item = _convivial_bootstrap_meta_explode_trim('=', $attrib, 2);
            $strategy['attribs'][$item[0]] = $item[1];
          }
        }
        if (!empty($mapping[4])) {
          $strategy['bundles'] = _convivial_bootstrap_meta_explode_trim('|', $mapping[4]);
        }
        $result[] = $strategy;
      }
    }
  }
  return $result;
}

/**
 * Splits string by delimiter and trims all parts.
 *
 * @param string $delimiter
 *   The boundary string.
 * @param string $string
 *   The input string.
 * @param int $limit
 *   The parts maximum.
 *
 * @return array
 *   The array of trimmed string parts.
 */
function _convivial_bootstrap_meta_explode_trim($delimiter, $string, $limit = NULL) {
  $result = isset($limit) ? explode($delimiter, $string, $limit) : explode($delimiter, $string);
  return empty($result) ? $result : array_map('trim', $result);
}
