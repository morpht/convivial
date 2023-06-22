<?php

namespace Drupal\convivial_layouts\Plugin\Layout;

/**
 * Configurable two column layout plugin class.
 *
 * @internal
 */
class TwoColumnLayout extends ConfigurableLayoutBase {

  /**
   * {@inheritdoc}
   */
  protected function getArrangementOptions() {
    return [
      'arrangement arrangement--8-4' => '8-4',
      'arrangement arrangement--4-8' => '4-8',
      'arrangement arrangement--9-3' => '9-3',
      'arrangement arrangement--3-9' => '3-9',
      'arrangement arrangement--6-6' => '6-6',
    ];
  }

}
