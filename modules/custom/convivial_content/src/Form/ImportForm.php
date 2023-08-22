<?php

namespace Drupal\convivial_content\Form;

use Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException;
use Drupal\convivial_content\Importer;
use Drupal\convivial_content\SiteSource;
use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Entity\EntityStorageException;
use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\Yaml\Yaml;

/**
 * Configure Convivial Content Import config form.
 */
class ImportForm extends ConfigFormBase {

  /**
   * Helper Importer Service.
   *
   * @var Importer
   */
  protected Importer $importer;

  /**
   * Helper for Sourcing the sites.
   *
   * @var SiteSource
   */
  protected SiteSource $siteSource;

  /**
   * Constructs a new SettingsForm instance.
   *
   * @param ConfigFactoryInterface $config_factory
   *   The factory for configuration objects.
   * @param Importer $importer
   *   The Helper service for Importing contents.
   * @param SiteSource $siteSource
   *   A service that retrieves YAML file content from a specified URL.
   */
  public function __construct(ConfigFactoryInterface $config_factory, Importer $importer, SiteSource $siteSource) {
    parent::__construct($config_factory);
    $this->importer = $importer;
    $this->siteSource = $siteSource;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('config.factory'),
      $container->get('convivial_content.importer'),
      $container->get('convivial_content.site_source')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'convivial_content_settings';
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

    $form = parent::buildForm($form, $form_state);

    $config = $this->config('convivial_content.settings');
    $source = $config->get('source') ?? "https://raw.githubusercontent.com/morpht/convivial-default-content/main/";
    $sites = $this->siteSource->getSites($source);
    $sites += ['custom' => 'Custom'];

    $form['consent_delete'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Delete All Content'),
      '#description' => $this->t('Opt in to delete existing content when importing new content. Additionally, this action will modify the basic site settings.'),
      '#default_value' => $config->get('consent_delete'),
    ];

    $form['sites'] = [
      '#type' => 'select',
      '#title' => $this->t('Sites'),
      '#options' => $sites,
      '#default_value' => $config->get('sites'),
      '#description' => $this->t('Retrieve the chosen YAML data from the predetermined data source.'),
    ];

    $form['custom_yaml'] = [
      '#type' => 'textarea',
      '#title' => $this->t('Paste the Custom YAML Data'),
      '#description' => $this->t("Please provide the YAML data for the site. This will override any selected option from the above dropdown list."),
      '#default_value' => $config->get('custom_yaml'),
      '#states' => [
        'visible' => [
          ':input[name="sites"]' => [
            'value' => 'custom',
          ],
        ],
      ],
    ];

    $form['actions']['submit']['#value'] = $this->t('Import');

    return $form;
  }

  /**
   * {@inheritdoc}
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \GuzzleHttp\Exception\GuzzleException
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('convivial_content.settings');

    $consent = $form_state->getValue('consent_delete');
    $config->set('consent_delete', $consent)->save();

    $site = $form_state->getValue('sites');
    $custom_yaml = $form_state->getValue('custom_yaml') === '' ? NULL : $form_state->getValue('custom_yaml');
    $sourceUrl = $config->get('source');
    if ($site === 'custom' && isset($custom_yaml)) {
      $yaml = Yaml::parse($custom_yaml);
    }
    else {
      $config->set('sites', $site)->save();

      $siteData = $this->siteSource->getSiteData($sourceUrl, $site);
      $yaml = $this->siteSource->getFileContent($sourceUrl, $siteData['file']);

    }

    try {
      $this->importer->importContent($yaml, $sourceUrl, $consent);
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

    parent::submitForm($form, $form_state);
  }

}
