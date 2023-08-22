<?php

namespace Drupal\convivial_content;

use GuzzleHttp\ClientInterface;
use Symfony\Component\Yaml\Yaml;

/**
 * A service that retrieves YAML data.
 */
class SiteSource {

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
   * Get all the sites name from index.yaml.
   *
   * @param string $siteSource
   *   URL source of Site data.
   *
   * @return array
   *   Return array format of all the sites.
   *
   * @throws \GuzzleHttp\Exception\GuzzleException
   */
  public function getSites(string $siteSource): array {
    $site = [];
    $siteData = $this->getFileContent($siteSource);
    if (isset($siteData)) {
      foreach ($siteData as $siteName => $siteDatum) {
        $site[$siteName] = $siteDatum['name'];
      }
    }
    return $site;
  }

  /**
   * Get site data for a specific site.
   *
   * @param string $siteSource
   *   Site source URL.
   * @param string $siteName
   *   Site name.
   *
   * @return array|null
   *   All the data associated with a particular site.
   *
   * @throws \GuzzleHttp\Exception\GuzzleException
   */
  public function getSiteData(string $siteSource, string $siteName): ?array {
    $siteData = $this->getFileContent($siteSource);
    if (isset($siteData)) {
      return $siteData[$siteName];
    }
    return NULL;
  }

  /**
   * Retrieve the parsed YAML content from a specified URL.
   *
   * @param string $siteSource
   *   Site source URL.
   * @param string $fileName
   *   (optional)File name which needs to be fetched.
   *
   * @return array|null
   *   Parsed data of a YAML set file.
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
