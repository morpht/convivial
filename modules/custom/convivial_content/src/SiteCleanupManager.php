<?php

namespace Drupal\convivial_content;

use Drupal\Core\Entity\EntityTypeManagerInterface;

/**
 * Site cleanup manager.
 */
class SiteCleanupManager {

  /**
   * The entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected EntityTypeManagerInterface $entityTypeManager;

  /**
   * Constructs a DeleteContent object.
   *
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   The entity type manager.
   */
  public function __construct(EntityTypeManagerInterface $entity_type_manager) {
    $this->entityTypeManager = $entity_type_manager;
  }

  /**
   * Delete any specified content.
   *
   * @param string $entityType
   *   Entity type name.
   * @param string|null $bundle
   *   Bundle name.
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   * @throws \Drupal\Core\Entity\EntityStorageException
   */
  public function delete(string $entityType, string $bundle = NULL) {
    $entityStorage = $this->entityTypeManager->getStorage($entityType);

    if ($bundle) {
      $query = $entityStorage->getQuery();
      switch ($entityType) {
        case 'media':
        case 'taxonomy_term':
          $query
            ->accessCheck('TRUE')
            ->condition('bundle', $bundle);
          break;

        case 'node':
        case 'block_content':
        case 'paragraph':
          $query
            ->accessCheck('TRUE')
            ->condition('type', $bundle);
          break;
      }
      $entity_ids = $query->execute();

      if (!empty($entity_ids)) {
        $entities = $entityStorage->loadMultiple($entity_ids);
        $entityStorage->delete($entities);
      }
    }
    else {
      $entities = $entityStorage->loadMultiple();
      $entities && $entityStorage->delete($entities);
    }
  }

}
