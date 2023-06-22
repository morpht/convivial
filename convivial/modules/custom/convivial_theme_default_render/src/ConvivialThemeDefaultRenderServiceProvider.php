<?php

namespace Drupal\convivial_theme_default_render;

use Drupal\Core\DependencyInjection\ContainerBuilder;
use Drupal\Core\DependencyInjection\ServiceProviderBase;

/**
 * Service provider for the theme manager decorator.
 */
class ConvivialThemeDefaultRenderServiceProvider extends ServiceProviderBase {

  /**
   * {@inheritdoc}
   */
  public function alter(ContainerBuilder $container) {
    if ($container->hasDefinition('theme.manager')) {
      $definition = $container->getDefinition('theme.manager');
      $definition->setMethodCalls([]);
    }
  }

}
