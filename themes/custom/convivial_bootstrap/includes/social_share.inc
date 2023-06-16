<?php

/**
 * @file
 * Social Share feature.
 *
 * @see convivial_bootstrap_preprocess_field()
 */

/**
 * Attach Social Share feature.
 */
function _convivial_bootstrap_social_share(&$variables) {
  $request = \Drupal::request();
  $route_match = \Drupal::routeMatch();
  $title = \Drupal::service('title_resolver')
    ->getTitle($request, $route_match->getRouteObject());

  if (is_array($title)) {
    $variables['node_title'] = $title['#markup'];
  }
  else {
    $variables['node_title'] = (string) $title;
  }

  $variables['base_path'] = base_path();
}
