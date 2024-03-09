<?php

namespace Drupal\convivial_claro;

use Drupal\Core\Security\TrustedCallbackInterface;

/**
 * Pre render some of the components in claro theme.
 */
class ConvivialClaroPreRender implements TrustedCallbackInterface {

  /**
   * {@inheritdoc}
   */
  public static function trustedCallbacks() {
    return ['paragraphs'];
  }

  /**
   * Pre render handler for paragraph entity forms.
   */
  public static function paragraphs(array $build) {
    switch ($build['#entity']->bundle()) {

      case 'charts':
        return self::chartsParagraph($build);
    }
    return $build;
  }

  /**
   * Pre render handler for "charts" paragraph entity form.
   */
  private static function chartsParagraph(array $build) {
    $build['manual'] = [
      '#type'   => 'details',
      '#title'  => 'Charts help',
      '#weight' => -1,
    ];
    $build['manual']['content'] = [
      '#markup' => t('Charts component is based on <a href="https://naver.github.io/billboard.js/">billboard.js library</a>. Is supports the most widely used chart types. Please note, that some of the settings are applicable only for specific chart types.'),
    ];
    return $build;
  }

}
