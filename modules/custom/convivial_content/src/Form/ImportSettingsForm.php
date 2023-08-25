<?php

namespace Drupal\convivial_content\Form;

use Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException;
use Drupal\convivial_content\Importer;
use Drupal\convivial_content\DataSourceManager;
use Drupal\Core\Entity\EntityStorageException;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\Yaml\Yaml;

/**
 * Convivial Content Import settings form.
 */
class ImportSettingsForm extends FormBase {

  /**
   * Helper Importer Service.
   *
   * @var Importer
   */
  protected Importer $importer;

  /**
   * Helper for Sourcing the sites.
   *
   * @var DataSourceManager
   */
  protected DataSourceManager $dataSourceManager;

  /**
   * Constructs a new ImportSettingsForm instance.
   *
   *   The factory for configuration objects.
   *
   * @param Importer $importer
   *   The Helper service for Importing contents.
   * @param DataSourceManager $siteSource
   *   A service that retrieves YAML file content from a specified URL.
   */
  public function __construct(Importer $importer, DataSourceManager $dataSourceManager) {
    $this->importer = $importer;
    $this->dataSourceManager = $dataSourceManager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('convivial_content.importer'),
      $container->get('convivial_content.data_source_manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'convivial_content_import_settings_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->configFactory()->get('convivial_content.settings');
    $source = $config->get('source_url');
    $datasets = $this->dataSourceManager->fetchDatasets($source);
    $datasets += ['custom' => 'Custom'];

    $form['site_cleanup'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Site Clean Up'),
      '#description' => $this->t('Use this to checkbox to delete existing content when importing new content. Additionally, this action will modify the basic site settings.'),
    ];

    $form['dataset'] = [
      '#type' => 'select',
      '#title' => $this->t('Dataset'),
      '#options' => $datasets,
      '#description' => $this->t("Select the dataset retrieved from the specified data source."),
    ];

    $form['custom_dataset'] = [
      '#type' => 'textarea',
      '#title' => $this->t('Paste the Custom YAML Data'),
      '#description' => $this->t("Please provide the YAML data for the site. This will override any selected option from the above dropdown list."),
      '#states' => [
        'visible' => [
          ':input[name="dataset"]' => [
            'value' => 'custom',
          ],
        ],
      ],
    ];

    $form['actions']['submit'] = [
      '#type' => 'submit',
      '#value' => $this->t('Import'),
      '#attributes' => [
        'class' => ['button', 'button--primary'],
      ],
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \GuzzleHttp\Exception\GuzzleException
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {

    $config = $this->configFactory()->get('convivial_content.settings');
    $sourceUrl = $config->get('source_url');
    $siteCleanup = $form_state->getValue('site_cleanup');

    $dataset = $form_state->getValue('dataset');
    $custom_yaml = $form_state->getValue('custom_dataset') === '' ? NULL : $form_state->getValue('custom_dataset');
    if ($dataset === 'custom' && isset($custom_yaml)) {
      $yaml = Yaml::parse($custom_yaml);
    }
    else {
      $datasetValue = $this->dataSourceManager->fetchDataset($sourceUrl, $dataset);
      $yaml = $this->dataSourceManager->getFileContent($sourceUrl, $datasetValue['file']);

    }

    try {
      $this->importer->importContent($yaml, $sourceUrl, $siteCleanup);
    }
    catch (InvalidPluginDefinitionException $e) {
      throw new InvalidPluginDefinitionException($e->getPluginId(), str_replace('plugin definition', 'derived plugin definition', $e->getMessage()));
    }
    catch (EntityStorageException $e) {
      $this->logger('convivial_content')->warning('Entity not found: ' . $e->getMessage());
    }
    catch (\Exception $e) {
      $this->logger('convivial_content')->warning('An unexpected error occurred: ' . $e->getMessage());
    }
  }

}
