<?php

namespace Drupal\convivial_content;

use Drupal\Core\Entity\EntityTypeManagerInterface;

/**
 * Service description.
 */
class DeleteContent {

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
   * @param string|null $bundleType
   *   Bundle type name.
   *
   * @throws \Drupal\Component\Plugin\Exception\InvalidPluginDefinitionException
   * @throws \Drupal\Component\Plugin\Exception\PluginNotFoundException
   * @throws \Drupal\Core\Entity\EntityStorageException
   */
  public function delete(string $entityType, string $bundleType = NULL) {
    $entityStorage = $this->entityTypeManager->getStorage($entityType);

    if ($bundleType) {
      $query = $entityStorage->getQuery();
      switch ($entityType) {
        case 'media':
        case 'taxonomy_term':
          $query
            ->accessCheck('TRUE')
            ->condition('bundle', $bundleType);
          break;

        case 'node':
        case 'block_content':
        case 'paragraph':
          $query
            ->accessCheck('TRUE')
            ->condition('type', $bundleType);
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
