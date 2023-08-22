<?php

namespace Drupal\convivial_content\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Configure Convivial Content settings for this site.
 */
class SiteSourceConfigForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'convivial_content_site_source_config';
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

    $form['site-source'] = [
      '#type' => 'url',
      '#title' => $this->t('Site Source'),
      '#default_value' => $config->get('source'),
      '#description' => $this->t('For example: https://raw.githubusercontent.com/morpht/convivial-default-content/main/'),
      '#required' => TRUE,
    ];
    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $this->config('convivial_content.settings')
      ->set('source', $form_state->getValue('site-source'))
      ->save();
    parent::submitForm($form, $form_state);
  }

}
