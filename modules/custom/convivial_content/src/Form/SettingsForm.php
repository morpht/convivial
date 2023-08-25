<?php

namespace Drupal\convivial_content\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Convivial Content settings form.
 */
class SettingsForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'convivial_content_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return ['convivial_content.settings'];
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('convivial_content.settings');

    $form['source_url'] = [
      '#type' => 'url',
      '#title' => $this->t('Source URL'),
      '#default_value' => $config->get('source_url'),
      '#description' => $this->t('Enter the source URL to fetch the content. For example: https://raw.githubusercontent.com/morpht/convivial-default-content/main/'),
      '#required' => TRUE,
    ];
    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('convivial_content.settings')
      ->set('source_url', $form_state->getValue('source_url'))
      ->save();
    parent::submitForm($form, $form_state);
  }

}
