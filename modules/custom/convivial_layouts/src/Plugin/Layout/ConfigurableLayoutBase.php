<?php

namespace Drupal\convivial_layouts\Plugin\Layout;

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Layout\LayoutDefault;
use Drupal\Core\Plugin\PluginFormInterface;

/**
 * Base class of configurable layouts.
 *
 * @internal
 */
abstract class ConfigurableLayoutBase extends LayoutDefault implements PluginFormInterface {

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    $arrangement_classes = array_keys($this->getArrangementOptions());
    $containment_classes = array_keys($this->getContainmentOptions());
    $color_classes = array_keys($this->getColorOptions());

    // The first option is used as the default configuration value.
    return [
      'arrangement' => array_shift($arrangement_classes),
      'containment' => array_shift($containment_classes),
      'color' => array_shift($color_classes),
      'styles' => NULL,
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $arrangement_options = $this->getArrangementOptions();
    $containment_options = $this->getContainmentOptions();
    $color_options = $this->getColorOptions();
    $style_options = $this->getStyleOptions();

    if (!empty($arrangement_options)) {
      $form['arrangement'] = [
        '#type' => 'select',
        '#title' => $this->t('Column arrangement'),
        '#description' => $this->t('Choose the column arrangement for this layout.'),
        '#options' => $arrangement_options,
        '#default_value' => $this->configuration['arrangement'],
      ];
    }
    if (!empty($containment_options)) {
      $form['containment'] = [
        '#type' => 'select',
        '#title' => $this->t('Containment'),
        '#description' => $this->t('<strong>Choose the containment for this layout:</strong><br><br>
        <strong>Edgy</strong> - Blocks will be always edgy. Paragraphs wrapper will be edgy but the content is still in the container. You can break the container by enabling "breakout" class.<br><br>
<strong>Edgy with container</strong> - Blocks and Paragraphs will be always inside the container but you can apply background modifier to the layout section so you can do some nice design.<br><br>
<strong>Container (with padding)</strong> - Blocks and Paragraphs will be always inside the container and also background will be in the container. The container has padding from each side (NSD thing).<br><br>
<strong>Container (without padding)</strong> - Blocks and Paragraphs will be always inside the container and also background will be in the container. The container does not have padding from each side.'),
        '#options' => $containment_options,
        '#default_value' => $this->configuration['containment'],
      ];
    }
    if (!empty($color_options)) {
      $form['color'] = [
        '#type' => 'select',
        '#title' => $this->t('Colour palette'),
        '#description' => $this->t('Choose the colour palette for this layout.'),
        '#options' => $color_options,
        '#default_value' => $this->configuration['color'],
      ];
    }
    if (!empty($style_options)) {
      $form['styles'] = [
        '#type' => 'select',
        '#multiple' => TRUE,
        '#title' => $this->t('Styles'),
        '#description' => $this->t('Choose the styles for this layout.'),
        '#options' => $style_options,
        '#default_value' => $this->configuration['styles'],
      ];
    }
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function validateConfigurationForm(array &$form, FormStateInterface $form_state) {
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    foreach (['arrangement', 'containment', 'color', 'styles'] as $key) {
      if ($form_state->hasValue($key)) {
        $this->configuration[$key] = $form_state->getValue($key);
      }
    }
  }

  /**
   * {@inheritdoc}
   */
  public function build(array $regions) {
    $build = parent::build($regions);
    $template = $this->getPluginDefinition()->getTemplate();

    $build['#attributes']['class'] = [
      'layout',
      $template,
    ];
    foreach (['arrangement', 'containment', 'color'] as $key) {
      if (!empty($this->configuration[$key])) {
        $build['#attributes']['class'][] = $this->configuration[$key];
      }
    }
    // Turn array values into a string.
    if (!empty($this->configuration['styles'])) {
      $build['#attributes']['class'][] = implode(' ', $this->configuration['styles']);
    }
    return $build;
  }

  /**
   * Gets the arrangement options for the configuration form.
   *
   * @return string[]
   *   The array of human readable labels, keyed by CSS classes.
   */
  abstract protected function getArrangementOptions();

  /**
   * Gets the containment options for the configuration form.
   *
   * @return string[]
   *   The array of human readable labels, keyed by CSS classes.
   */
  protected function getContainmentOptions() {
    return [
      'edgy edgy--contained' => 'Edgy with container',
      'edgy' => 'Edgy',
      'container' => 'Container',
      'container-fluid' => 'Fluid (padding on both sides)',
    ];
  }

  /**
   * Gets the color options for the configuration form.
   *
   * @return string[]
   *   The array of human readable labels, keyed by CSS classes.
   */
  protected function getColorOptions() {
    return [
      '' => $this->t('- None -'),
      'cp cp--standard' => 'Standard',
      'cp cp--light' => 'Light',
      'cp cp--dark' => 'Dark',
      'cp cp--primary' => 'Primary',
      'cp cp--primary-dark' => 'Primary dark',
    ];
  }

  /**
   * Gets the style options for the configuration form.
   *
   * @return string[]
   *   The array of human readable labels, keyed by CSS classes.
   */
  protected function getStyleOptions() {
    return [
      'visually-hidden' => 'Visually hidden',
    ];
  }

}
