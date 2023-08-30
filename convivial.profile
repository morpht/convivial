<?php

/**
 * @file
 * Enables modules and site configuration for a convivial site installation.
 */

use Drupal\Core\Form\FormStateInterface;
use Symfony\Component\Yaml\Yaml;

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function convivial_form_install_configure_form_alter(&$form, FormStateInterface $form_state, $form_id) {
  $form['site_information']['site_name']['#default_value'] = 'Convivial CXP';
  $moduleHandler = \Drupal::service('module_handler');
  if ($moduleHandler->moduleExists('convivial_content')) {

    $config = \Drupal::config('convivial_content.settings');
    $sourceUrl = $config->get('source_url');

    /** @var \Drupal\convivial_content\DataSourceManager $sourceManager */
    $sourceManager = \Drupal::service('convivial_content.data_source_manager');

    $datasets = ['none' => 'None'];
    $datasets += $sourceManager->fetchDatasets($sourceUrl);
    $datasets += ['custom' => 'Custom'];

    $form['dataset'] = [
      '#type' => 'select',
      '#title' => t('Dataset'),
      '#options' => $datasets,
      '#description' => t("Select the dataset retrieved from the specified data source."),
    ];
    $form['custom_dataset'] = [
      '#type' => 'textarea',
      '#title' => t('Paste the Custom YML Data'),
      '#description' => t("Please provide the YML dataset for the site content."),
      '#rows' => 10,
      '#states' => [
        'visible' => [
          ':input[name="dataset"]' => [
            'value' => 'custom',
          ],
        ],
      ],
    ];

    $form['#submit'][] = 'convivial_import_content';
  }
}

/**
 * Submission handler to import the dataset.
 */
function convivial_import_content(array $form, FormStateInterface $form_state) {
  $dataset = $form_state->getValue('dataset');
  if ($dataset !== 'none') {
    /** @var \Drupal\convivial_content\DataSourceManager $sourceManager */
    $sourceManager = \Drupal::service('convivial_content.data_source_manager');

    $config = \Drupal::config('convivial_content.settings');
    $sourceUrl = $config->get('source_url');

    $custom_yaml = $form_state->getValue('custom_dataset') === '' ? NULL : $form_state->getValue('custom_dataset');
    $yml = '';
    if ($dataset === 'custom' && isset($custom_yaml)) {
      $yml = Yaml::parse($custom_yaml);
    }
    else {
      $datasetValue = $sourceManager->fetchDataset($sourceUrl, $dataset);
      array_key_exists('file', $datasetValue) &&
      $yml = $sourceManager->getFileContent($sourceUrl, $datasetValue['file']);
    }

    /** @var \Drupal\convivial_content\DataImporter $dataImporter */
    $dataImporter = \Drupal::service('convivial_content.data_importer');

    try {
      $dataImporter->importContent($yml, $sourceUrl, 1);
      \Drupal::messenger()->addMessage(ucfirst($dataset) . ' demo content installed.');
    }
    catch (\Exception $e) {
      \Drupal::logger('convivial_content')->warning('An unexpected error occurred: ' . $e->getMessage());
    }
  }
}
