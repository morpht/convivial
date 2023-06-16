<?php

/**
 * @file
 * View feature.
 *
 * @see convivial_bootstrap_preprocess_field()
 */

use Drupal\views\Views;

/**
 * Attach View feature.
 */
function _convivial_bootstrap_view(&$variables) {
  /** @var \Drupal\Core\Entity\FieldableEntityInterface $entity */
  $entity = $variables['element']['#object'];
  // Get the value of the field.
  $field_value = trim($entity->get('field_view_definition')->value);

  // Only if field has value.
  if (!empty($field_value)) {
    // Divide the string by space.
    $parts = explode(' ', $field_value);
    $count = count($parts);

    // Set the each part of string to the particular variable.
    switch ($count) {
      // Machine name of the view.
      case 1:
        [$id] = $parts;
        break;

      // Display of the view.
      case 2:
        [$id, $display] = $parts;
        break;

      // Arguments of the view.
      case 3:
        [$id, $display, $args_string] = $parts;
        $args = explode(',', $args_string);
        break;
    }

    // Pass each variable to the view.
    if (!empty($id)) {
      $view = Views::getView($id);
    }
    if (!empty($view)) {
      if (!empty($display)) {
        // Initialize display and check if display exists.
        $view->initDisplay();
        if ($view->displayHandlers->has($display)) {
          $view->setDisplay($display);
        }
      }
      if (!empty($args)) {
        $view->setArguments($args);
      }
      // Render view.
      if ($view->execute()) {
        $rendered_view = $view->render();
        if (!empty($rendered_view)) {
          $result = \Drupal::service('renderer')->render($rendered_view);
        }
        else {
          $result = t('Unable to render view.');
        }
      }
      else {
        $result = t('Unable to execute view.');
      }
    }
    else {
      $result = t('Unknown view');
    }
  }
  else {
    $result = t('No definition found.');
  }

  $variables['view'] = $result;
}
