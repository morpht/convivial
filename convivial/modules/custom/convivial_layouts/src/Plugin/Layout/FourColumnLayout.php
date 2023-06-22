<?php

namespace Drupal\convivial_layouts\Plugin\Layout;

/**
 * Configurable four column layout plugin class.
 *
 * @internal
 */
class FourColumnLayout extends ConfigurableLayoutBase {

  /**
   * {@inheritdoc}
   */
  protected function getArrangementOptions() {
    return [
      'arrangement arrangement--3-3-3-3' => '3-3-3-3',
    ];
  }

}
