<?php

namespace Drupal\convivial_layouts\Plugin\Layout;

/**
 * Configurable three column layout plugin class.
 *
 * @internal
 */
class ThreeColumnLayout extends ConfigurableLayoutBase {

  /**
   * {@inheritdoc}
   */
  protected function getArrangementOptions() {
    return [
      'arrangement arrangement--4-4-4' => '4-4-4',
      'arrangement arrangement--3-6-3' => '3-6-3',
      'arrangement arrangement--6-3-3' => '6-3-3',
      'arrangement arrangement--3-3-6' => '3-3-6',
    ];
  }

}
