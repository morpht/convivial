<?php

namespace Drupal\convivial_content;

use Drupal\block_content\Entity\BlockContent;
use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\File\FileSystemInterface;
use Drupal\Core\Http\ClientFactory;
use Drupal\Core\Messenger\MessengerInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\file\Entity\File;
use Drupal\media\Entity\Media;
use Drupal\menu_link_content\Entity\MenuLinkContent;
use Drupal\node\Entity\Node;
use Drupal\taxonomy\Entity\Term;

/**
 * Helper service for Importing content.
 */
class Importer {

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected EntityTypeManagerInterface $entityTypeManager;

  /**
   * Module handler.
   *
   * @var \Drupal\Core\Extension\ModuleHandlerInterface
   */
  protected ModuleHandlerInterface $moduleHandler;

  /**
   * Helper Service for deleting content.
   *
   * @var DeleteContent
   */
  protected DeleteContent $deleteContent;

  /**
   * The messenger service.
   *
   * @var \Drupal\Core\Messenger\MessengerInterface
   */
  protected MessengerInterface $messenger;

  /**
   * The current user.
   *
   * @var \Drupal\Core\Session\AccountInterface
   */
  protected AccountInterface $currentUser;

  /**
   * The file system service.
   *
   * @var \Drupal\Core\File\FileSystemInterface
   */
  protected FileSystemInterface $fileSystem;

  /**
   * Client Factory service.
   *
   * @var \Drupal\Core\Http\ClientFactory
   */
  protected ClientFactory $clientFactory;

  /**
   * Hold the ids of all the created value.
   *
   * @var array
   */
  protected array $dictionary;

  /**
   * Hold the schema array.
   *
   * @var array
   */
  protected array $schema;

  /**
   * Helper for Sourcing the sites.
   *
   * @var SiteSource
   */
  protected SiteSource $siteSource;

  /**
   * The system config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected ConfigFactoryInterface $configFactory;

  /**
   * Constructs an Importer object.
   *
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The entity type manager.
   * @param \Drupal\Core\Session\AccountInterface $current_user
   *   Current user.
   * @param \Drupal\Core\File\FileSystemInterface $file_system
   *   The file handler.
   * @param \Drupal\Core\Extension\ModuleHandlerInterface $moduleHandler
   *   Module handler.
   * @param DeleteContent $deleteContent
   *   Helper Service for deleting content.
   * @param \Drupal\Core\Messenger\MessengerInterface $messenger
   *   The messenger service.
   * @param \Drupal\Core\Http\ClientFactory $clientFactory
   *   The Https Client Factory service.
   * @param SiteSource $siteSource
   *   The helper service to fetch Site Source data.
   */
  public function __construct(
    EntityTypeManagerInterface $entity_type_manager,
    AccountInterface $current_user,
    FileSystemInterface $file_system,
    ModuleHandlerInterface $moduleHandler,
    DeleteContent $deleteContent,
    MessengerInterface $messenger,
    ClientFactory $clientFactory,
    SiteSource $siteSource,
    ConfigFactoryInterface $configFactory
  ) {
    $this->entityTypeManager = $entity_type_manager;
    $this->currentUser = $current_user;
    $this->fileSystem = $file_system;
    $this->moduleHandler = $moduleHandler;
    $this->deleteContent = $deleteContent;
    $this->messenger = $messenger;
    $this->clientFactory = $clientFactory;
    $this->siteSource = $siteSource;
    $this->configFactory = $configFactory;
    $this->dictionary = [];
    $this->schema = [];

  }

  /**
   * Import all the entities.
   *
   * @param array $yamlData
   *   Array of all the entities.
   * @param string $sourceUrl
   *   Source Url.
   * @param int $consent
   *   Consent for deleting existing entities.
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   * @throws \Drupal\Core\Entity\EntityStorageException
   * @throws \GuzzleHttp\Exception\GuzzleException
   * @throws \Exception
   */
  public function importContent(array $yamlData, string $sourceUrl, int $consent) {

    $schemaFilename = $yamlData['schema'] . '.yaml';
    $this->schema = $this->siteSource->getFileContent($sourceUrl, $schemaFilename);

    if (array_key_exists('vocabulary', $yamlData)) {
      $consent && $this->deleteContent->delete('taxonomy_term');
      $terms = $this->processContent('vocabulary', 'taxonomy_term', $yamlData['vocabulary']);
      $terms && $this->importTerms($terms);
    }
    if (array_key_exists('media', $yamlData)) {
      $consent && $this->deleteContent->delete('media', 'image');
      $images = $this->processContent('media', 'media', $yamlData['media']);
      $images && $this->importMedia($images);
    }
    if (array_key_exists('content-block', $yamlData)) {
      // Delete the only block_content that will be imported.
      if ($consent) {
        $type = array_keys($yamlData['content-block']);
        foreach ($type as $name) {
          $this->deleteContent->delete('block_content', $name);
        }
      }
      // Import block content.
      $blockContent = $this->processContent('content-block', 'block_content', $yamlData['content-block']);
      $blockContent && $this->importBlockContent($blockContent);
    }
    if (array_key_exists('content-type', $yamlData)) {
      if ($consent) {
        // Delete only the nodes which have exported data.
        foreach ($yamlData['content-type'] as $bundleName => $fields) {
          $this->deleteContent->delete('node', $bundleName);
        }
      }

      $nodes = $this->processContent('content-type', 'node', $yamlData['content-type']);
      $this->importNode($nodes);
    }

    // Update the reference fields' value for nodes.
    if (isset($this->dictionary['need-update'])) {
      foreach ($this->dictionary['need-update'] as $entity) {
        if ($entity['entity'] === 'node') {
          $entityData = $this->searchDictionary($entity['entity'], $entity['name']);
          $entityId = NULL;
          $entityData && $entityId = $this->getTargetIdFromName($entityData['entity'], 'title', $entityData['title']);
          $target = $this->searchDictionary('node', $entity['value']);
          $target && $targetId = $this->getTargetIdFromName($target['entity'], 'title', $target['title']);
          if (isset($targetId) && isset($entityId)) {
            try {
              $node = $this->entityTypeManager->getStorage('node')->load($entityId);
              $existing = $node->get($entity['field_name'])->getValue() ?? NULL;
              $existing[] = [
                'target_id' => $targetId,
              ];
              $node->set($entity['field_name'], $existing);
              $node->save();
            }
            catch (\Exception $e) {
              $this->messenger->addWarning($e->getMessage() . ' for ' . $entity['entity'] . ':' . $entity['bundle']);
            }
          }
        }
      }
    }

    // @todo Delete existing paragraph which might get imported.
    $this->deleteContent->delete('paragraph');

    // Create a paragraph and set its place in any reference entity.
    $para = $this->processContent('paragraphs', 'paragraph', $this->dictionary["paragraphs"]);
    $this->importParagraphs($para);

    if (array_key_exists('menu', $yamlData)) {
      $consent && $this->importMenuLinks($yamlData['menu']);
    }
    if (array_key_exists('site', $yamlData)) {
      $consent && $this->setSiteConfig($yamlData['site']);
    }

  }

  /**
   * Process any entity type so that I can be created.
   *
   * @param string $collectionName
   *   Name of the collection in the YAML file.
   * @param string $entityType
   *   Entity type name.
   * @param array $entityData
   *   Entity Data.
   *
   * @return array
   *   Return the processed data.
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   * @throws \Exception
   */
  protected function processContent(string $collectionName, string $entityType, array $entityData): array {
    $content = [];
    $schema = $this->schema[$collectionName];
    foreach ($entityData as $bundleName => $data) {
      // The paragraph data is handled separately
      // and created after its referred entity is created.
      if ($entityType === 'paragraph') {
        $content[$bundleName] = $data;
        foreach ($data as $dataKey => $paraEntityData) {
          foreach ($paraEntityData['para'] as $key => $para) {
            $paraTypeSchema = $schema[$para['type']];
            if ($this->checkIfBundleExists($entityType, $para['type'])) {
              $paraData = $this->defaultStructure($entityType, $para['type'], $para);
              unset($para['type']);
              foreach ($para as $fieldKey => $fieldData) {
                if (isset($paraTypeSchema[$fieldKey])) {
                  // Need to handle block_content separately,
                  // as a paragraph might have a reference to it.
                  if ($paraTypeSchema[$fieldKey]['type'] === 'block_content') {
                    $target = $this->searchDictionary('block_content', $fieldData);
                    $targetId = $this->getTargetIdFromName($target['entity'], 'info', $target['info']);
                    $paraData += [
                      $paraTypeSchema[$fieldKey]['field'] => [
                        'target_id' => $targetId,
                      ],
                    ];
                  }
                  else {
                    $paraData += [
                      $paraTypeSchema[$fieldKey]['field'] => $fieldData,
                    ];
                  }
                }
                else {
                  $this->messenger->addWarning("Schema of \"" . $entityType . "\" for \"" . $fieldKey . "\" field key not found.");
                }
              }
              $content[$bundleName][$dataKey]['para'][$key] = $paraData;
            }
          }
        }
      }
      else {
        if ($this->checkIfBundleExists($entityType, $bundleName)) {
          foreach ($data as $propertyName => $propertyValues) {

            $entityValues = $this->defaultStructure($entityType, $bundleName, $propertyValues);
            $bundleSchema = $schema[$bundleName];

            foreach ($propertyValues as $key => $value) {
              // Ignore any default structure which is already set.
              if (!isset($entityValues[$key])) {
                if (isset($bundleSchema[$key])) {
                  switch ($bundleSchema[$key]['type']) {
                    case 'date':
                    case 'text':
                    case 'list':
                    case 'boolean':
                      $entityValues[$bundleSchema[$key]['field']] = $value;
                      break;

                    case 'media-image':
                      if (isset($this->dictionary['media']['image'][$value])) {
                        $mediaName = $this->dictionary['media']['image'][$value]['name'];
                        $targetId = $this->getTargetIdFromName('media', 'name', $mediaName);
                        $entityValues[$bundleSchema[$key]['field']]['target_id'] = $targetId;
                      }
                      break;

                    case 'html':
                      $entityValues[$bundleSchema[$key]['field']] = [
                        'value' => $value,
                        'format' => 'rich_text',
                      ];
                      break;

                    case 'alias':
                      $entityValues['path'] = [
                        'alias' => $value,
                      ];
                      break;

                    case 'paragraphs':
                      if ($this->moduleHandler->moduleExists('paragraphs')) {
                        // Add paragraph data to the dictionary,
                        // so that its reference can be added
                        // when the entity is created.
                        $this->dictionary['paragraphs'][$entityType][] = [
                          'name' => ($entityType === 'block_content') ? $propertyValues['info'] : $propertyValues['title'],
                          'field_name' => $bundleSchema[$key]['field'],
                          'para' => $value,
                        ];
                      }
                      else {
                        throw new \Exception('The Paragraph module is not enabled.');
                      }
                      break;

                    case 'ref':
                      foreach ($value as $item) {
                        // Assuming it is a reference term field,
                        // or else a node reference that has not been created.
                        $dictionaryValue = $this->searchDictionary('taxonomy_term', $item);
                        // If a reference term field value is found,
                        // then set the target ID.
                        if ($dictionaryValue) {
                          $targetId = $this->getTargetIdFromName($dictionaryValue['entity'], 'name', $dictionaryValue['name']);
                          $entityValues[$bundleSchema[$key]['field']][] = [
                            'target_id' => $targetId,
                          ];
                        }
                        else {
                          $this->dictionary['need-update'][] = [
                            'entity' => $entityType,
                            'bundle' => $bundleName,
                            'name' => $propertyName,
                            'field_name' => $bundleSchema[$key]['field'],
                            'value' => $item,
                          ];

                        }
                      }
                      break;
                  }
                }
                elseif ($key === 'source') {
                  $entityValues['name'] = $propertyValues['name'];
                  $entityValues['field_media_image'] = $propertyValues['source'];
                }
                else {
                  $this->messenger
                    ->addWarning(
                      "Schema for \"" . $entityType . ':' . $bundleName . ':' . $key . "\"was not found."
                    );
                }
              }
            }
            $content[$propertyName] = $entityValues;
          }
        }
        else {
          $this->messenger
            ->addWarning(
              "The \"" . $entityType . ":" . $bundleName . "\" doesn't exist and was skipped."
            );
        }
      }
    }
    return $content;
  }

  /**
   * Set a default Structure which are required for every entity.
   *
   * @param string $entityType
   *   Entity Type.
   * @param string $bundle
   *   Bundle Name.
   * @param array $data
   *   Value for the entity.
   *
   * @return array|null
   *   Return the default array.
   */
  protected function defaultStructure(string $entityType, $bundle, $data): ?array {
    $entityValues = NULL;
    switch ($entityType) {
      case 'taxonomy_term':
        $entityValues['vid'] = $bundle;
        $entityValues['name'] = $data['name'];
        break;

      case 'media':
        $entityValues['uid'] = $this->currentUser->id();
        break;

      case 'block_content':
        $entityValues['info'] = $data['info'];
        $entityValues['type'] = $bundle;
        break;

      case 'node':
        $entityValues['uid'] = $this->currentUser->id();
        $entityValues['title'] = $data['title'];
        $entityValues['type'] = $bundle;
        break;

      case 'paragraph':
        $entityValues['type'] = $bundle;
    }
    return $entityValues;
  }

  /**
   * Get the target ID from the reference field name.
   *
   * @param string $entityType
   *   Entity Type Name.
   * @param string $fieldName
   *   Name of the Field.
   * @param string $name
   *   Value of the reference field.
   *
   * @return string
   *   Target ID.
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   */
  protected function getTargetIdFromName(string $entityType, string $fieldName, string $name): string {
    $query = $this->entityTypeManager->getStorage($entityType)->getQuery();
    $query->accessCheck(TRUE)->condition($fieldName, $name);
    $targetId = $query->execute();
    return reset($targetId);
  }

  /**
   * Create terms.
   *
   * @param array $terms
   *   Terms data.
   *
   * @throws \Drupal\Core\Entity\EntityStorageException
   */
  protected function importTerms(array $terms) {
    foreach ($terms as $key => $term) {
      $taxonomy_term = Term::create($term);
      $result = $taxonomy_term->save();
      $this->setDictionary($taxonomy_term->getEntityType()->id(), $taxonomy_term->bundle(), $key, $term, $result);
    }
  }

  /**
   * Create Image Media.
   *
   * @param array $images
   *   Images data.
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   * @throws \Drupal\Core\Entity\EntityStorageException
   * @throws \GuzzleHttp\Exception\GuzzleException
   */
  protected function importMedia(array $images) {
    foreach ($images as $key => $image) {
      $fid = $this->createImageFile($image);
      $media = Media::create([
        'name' => $image['name'],
        'bundle' => 'image',
        'uid' => $image['uid'],
        'field_media_image' => [
          'target_id' => $fid,
        ],
      ]);
      $result = $media->save();
      $this->setDictionary($media->getEntityTypeId(), $media->bundle(), $key, $image, $result);
    }
  }

  /**
   * Create Block Content.
   *
   * @param array $blockContent
   *   Processed block content.
   *
   * @throws \Drupal\Core\Entity\EntityStorageException
   */
  protected function importBlockContent(array $blockContent) {
    foreach ($blockContent as $key => $blockDatum) {
      $block = BlockContent::create($blockDatum);
      $result = $block->save();
      $this->setDictionary($block->getEntityTypeId(), $block->bundle(), $key, $blockDatum, $result);
    }
  }

  /**
   * Create paragraphs and set them to the respective entity.
   *
   * @param array $paragraphData
   *   Processed paragraph data.
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   * @throws \Drupal\Core\Entity\EntityStorageException
   */
  protected function importParagraphs(array $paragraphData) {
    foreach ($paragraphData as $entityName => $entity) {
      foreach ($entity as $entityData) {
        $targetIds = [];
        // Create all the required paragraphs.
        foreach ($entityData['para'] as $para) {
          $paragraph = $this->entityTypeManager->getStorage('paragraph')->create($para);
          $paragraph->save();
          $paragraph->enforceIsNew();
          $targetIds[] = [
            'target_id' => $paragraph->id(),
            'target_revision_id' => $paragraph->getRevisionId(),
          ];
        }
        $storage = $this->entityTypeManager->getStorage($entityName);
        $query = $storage->getQuery()->accessCheck(TRUE);
        $updateEntity = NULL;
        // Set the paragraph to the respective reference fields.
        switch ($entityName) {
          case 'block_content':
            $query->condition('info', $entityData['name']);
            $id = $query->execute();
            if ($id) {
              $updateEntity = BlockContent::load(reset($id));
            }
            break;

          case 'node':
            $query->condition('title', $entityData['name']);
            $id = $query->execute();
            if ($id) {
              $updateEntity = $storage->load(reset($id));
            }
            break;
        }
        $updateEntity->set($entityData["field_name"], $targetIds);
        $updateEntity->save();
      }
    }
  }

  /**
   * Create Node.
   *
   * @param array $nodes
   *   Processed nodes data.
   *
   * @throws \Drupal\Core\Entity\EntityStorageException
   */
  protected function importNode(array $nodes) {
    foreach ($nodes as $key => $node) {
      $nodeObject = Node::create($node);
      $nodeObject->set('moderation_state', 'published');
      $result = $nodeObject->save();
      $this->setDictionary('node', $node['type'], $key, $node, $result);
    }
  }

  /**
   * Create image file.
   *
   * @param array $image
   *   Images data.
   *
   * @return int|mixed|string|null
   *   Return the file ID else NULL
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   * @throws \Drupal\Core\Entity\EntityStorageException
   * @throws \GuzzleHttp\Exception\GuzzleException
   */
  protected function createImageFile(array $image): mixed {

    $client = $this->clientFactory->fromOptions();
    $response = $client->get($image['field_media_image']);
    $fileContent = $response->getBody()->getContents();
    if (!empty($fileContent)) {
      $contentType = $response->getHeader('Content-Type');
      // Extract the file format from the content type.
      $fileFormat = strtolower(explode('/', reset($contentType))[1]);

      $directory = 'public://convivial/assets';
      $this->fileSystem->prepareDirectory($directory, FileSystemInterface:: CREATE_DIRECTORY | FileSystemInterface::MODIFY_PERMISSIONS);
      $filepath = $directory . '/' . trim(strtolower(str_replace(' ', '-', $image['name']))) . '.' . $fileFormat;
      file_put_contents($filepath, $fileContent);

      // Check if the file exists, then return the existing file ID.
      if ($this->fileExists($filepath)) {
        if ($existingFileId = $this->findExistingFile($filepath)) {
          return $existingFileId;
        }
      }

      $file = File::create([
        'filename' => basename($filepath),
        'uri' => $filepath,
      ]);
      $file->setOwnerId($this->currentUser->id());
      $file->setPermanent();
      $file->save();

      return $file->id();
    }
    return NULL;
  }

  /**
   * Check if the file exists.
   *
   * @param string $path
   *   Path of the existing file.
   *
   * @return bool
   *   Return a boolean value if the file exists.
   */
  protected function fileExists(string $path): bool {
    return file_exists($path);
  }

  /**
   * Find the existing file ID.
   *
   * @param string $path
   *   Path of the existing file.
   *
   * @return false|mixed|null
   *   Return the existing file ID.
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   */
  protected function findExistingFile(string $path): mixed {
    $query = $this->entityTypeManager
      ->getStorage('file')
      ->getQuery();
    $file_ids = $query
      ->accessCheck('TRUE')
      ->condition('uri', $path)
      ->execute();
    if (!empty($file_ids)) {
      return reset($file_ids);
    }
    return NULL;
  }

  /**
   * Set module_path variable.
   *
   * @return string
   *   Return the module path.
   */
  protected function getModulePath(): string {
    return $this->moduleHandler->getModule('convivial_content')->getPath();
  }

  /**
   * Check if bundle exists.
   *
   * @param string $entityType
   *   The entity type.
   * @param string $bundle
   *   The bundle name.
   *
   * @return bool
   *   True if the bundle exists, false otherwise.
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   */
  public function checkIfBundleExists(string $entityType, string $bundle): bool {
    $entityDefinition = $this->entityTypeManager->getDefinition($entityType);
    $bundleEntityType = $entityDefinition->getBundleEntityType();

    if (!empty($bundleEntityType)) {
      $storage = $this->entityTypeManager->getStorage($bundleEntityType);
      return $storage->load($bundle) !== NULL;
    }

    return FALSE;
  }

  /**
   * Set the Dictionary with the saved values.
   *
   * @param string $entityType
   *   The entity type.
   * @param string $bundle
   *   The bundle name.
   * @param string $key
   *   Key name of the content as per the YAML.
   * @param array $processedValue
   *   Processed and saved values.
   * @param int|null $saveResult
   *   Return value of saved method call.
   */
  protected function setDictionary(string $entityType, string $bundle, string $key, array $processedValue, ?int $saveResult) {
    if ($saveResult === SAVED_NEW) {
      $processedValue['entity'] = $entityType;
      $processedValue['bundle'] = $bundle;
      $this->dictionary[$entityType][$bundle][$key] = $processedValue;
    }
    else {
      $this->messenger->addError('The ' . $entityType . ':' . $bundle . ':' . $key . 'was not skipped for some unexpected reasons');
    }
  }

  /**
   * Search for a value in the dictionary.
   *
   * @param string $entityTypeNeedle
   *   Entity type of the needle.
   * @param string $needle
   *   Value which needs to be searched.
   *
   * @return mixed|null
   *   Dictionary found value.
   */
  protected function searchDictionary(string $entityTypeNeedle, string $needle): mixed {
    $dictionary = $this->dictionary;
    unset($dictionary['need-update']);
    foreach ($this->dictionary as $entityName => $entity) {
      foreach ($entity as $bundleName => $bundle) {
        if ($entityTypeNeedle === $entityName && isset($this->dictionary[$entityName][$bundleName][$needle])) {
          return $this->dictionary[$entityName][$bundleName][$needle];
        }
      }
    }
    return NULL;
  }

  /**
   * Set Basic site settings.
   *
   * @param mixed $site
   *   YAML data for Site settings.
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   */
  protected function setSiteConfig(mixed $site) {
    $systemConfig = $this->configFactory->getEditable('system.site');

    // @todo Apply the theme logo from the config.
    $nodeData = $this->searchDictionary('node', $site['homepage']);
    $query = $this->entityTypeManager->getStorage('node')->getQuery();
    $query->accessCheck(TRUE)->condition('title', $nodeData['title']);
    $node = $query->execute();
    $systemConfig->set('page.front', '/node/' . reset($node))->save();

    $systemConfig->set('mail', $site['email'])->save();
    $systemConfig->set('name', $site['name'])->save();
  }

  /**
   * Import Menu links from provided schema.
   *
   * @param mixed $menu
   *   Menu schema data.
   *
   * @throws \Drupal\Core\Entity\EntityStorageException
   */
  protected function importMenuLinks(mixed $menu) {
    foreach ($menu as $menuName => $menuData) {
      foreach ($menuData as $linkInfo) {
        $node = $this->searchDictionary('node', $linkInfo);
        if (isset($node)) {
          $menuObj = MenuLinkContent::create([
            'title' => $node['title'],
            'link' => [
              'uri' => 'internal:' . $node['path']['alias'],
            ],
            'menu_name' => $menuName,
          ]);
          $menuObj->save();
        }
      }
    }
  }

}
