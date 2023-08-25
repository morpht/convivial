<?php

namespace Drupal\convivial_content;

use GuzzleHttp\ClientInterface;
use Symfony\Component\Yaml\Yaml;

/**
 * Data Source Manager.
 */
class DataSourceManager {

  /**
   * The GuzzleHttp client.
   *
   * @var \GuzzleHttp\ClientInterface
   */
  protected ClientInterface $httpClient;

  /**
   * Constructs a SiteSource object.
   *
   * @param \GuzzleHttp\ClientInterface $http_client
   *   The GuzzleHttp client.
   */
  public function __construct(ClientInterface $http_client) {
    $this->httpClient = $http_client;
  }

  /**
   * Get all the datasets from index.yml.
   *
   * @param string $sourceUrl
   *   Source URL of the data.
   *
   * @return array|null
   *   Return list of retrived datasets.
   *
   * @throws \GuzzleHttp\Exception\GuzzleException
   */
  public function fetchDatasets(string $sourceUrl): array {
    $return = [];
    $datasets = $this->getFileContent($sourceUrl);
    if (isset($datasets)) {
      foreach ($datasets as $name => $dataset) {
        $return[$name] = $dataset['name'];
      }
    }
    return $return;
  }

  /**
   * Get a specific dataset.
   *
   * @param string $sourceUrl
   *   Source URL of the data.
   * @param string $dataset
   *   Name of the dataset.
   *
   * @return array|null
   *   All the data associated with a particular site.
   *
   * @throws \GuzzleHttp\Exception\GuzzleException
   */
  public function fetchDataset(string $sourceUrl, string $dataset): ?array {
    $datasets = $this->getFileContent($sourceUrl);
    if (isset($datasets)) {
      return $datasets[$dataset];
    }
    return NULL;
  }

  /**
   * Get the parsed YAML content from a specified URL.
   *
   * @param string $sourceUrl
   *   Source URL of the data.
   * @param string $fileName
   *   (optional)File name which needs to be fetched.
   *
   * @return array|null
   *   Parsed data of a YML set file.
   *
   * @throws \GuzzleHttp\Exception\GuzzleException
   * @throws \Exception
   */
  public function getFileContent(string $siteSource, string $fileName = 'index.yaml') {
    try {
      $response = $this->httpClient->request('GET', $siteSource . $fileName);
      $content = $response->getBody()->getContents();
    }
    catch (\Exception $e) {
      throw new \Exception('Failed to fetch content: ' . $e->getMessage());
    }
    return $content ? Yaml::parse($content) : NULL;
  }

}
