<?php

/**
 * @file
 * Glossary feature.
 *
 * @see convivial_bootstrap_preprocess_field()
 */

use Drupal\Component\Utility\Html;

/**
 * Attach Glossary feature.
 */
function _convivial_bootstrap_glossary(&$variables) {
  /** @var \Drupal\Core\Entity\FieldableEntityInterface $entity */
  $entity = $variables['element']['#object'];

  // Skip if entity is not node.
  if ($entity->getEntityTypeId() !== 'node') {
    return;
  }
  // Skip if body value is empty.
  $body = $entity->get('field_body')->value;
  if (empty($body)) {
    return;
  }

  $entity_bundle = $entity->bundle();
  $entity_fields = $entity->getFields();
  $glossary_exclude = isset($entity_fields['field_glossary_exclude']) ? (int) $entity_fields['field_glossary_exclude']->value : 0;
  $glossary_bundles = !empty(theme_get_setting('glossary_bundles')) ? array_filter(theme_get_setting('glossary_bundles')) : [];
  $tags_blacklist = !empty(theme_get_setting('glossary_tags_blacklist')) ? array_flip(explode(',', theme_get_setting('glossary_tags_blacklist'))) : [];
  // Array_filter is here to filter non-selected options which have value 0.
  $tooltip_sources = !empty(theme_get_setting('glossary_bundles_tooltip_sources')) ? array_filter(theme_get_setting('glossary_bundles_tooltip_sources')) : [];
  $link_sources = !empty(theme_get_setting('glossary_bundles_link_sources')) ? array_filter(theme_get_setting('glossary_bundles_link_sources')) : [];

  // Test if glossary feature should be applied.
  if (empty($glossary_exclude) && in_array($entity_bundle, $glossary_bundles)) {
    $terms = [];
    foreach ($tooltip_sources as $content_type) {
      $terms[] = _convivial_bootstrap_glossary_get_terms($content_type, 'tooltip');
    }
    foreach ($link_sources as $content_type) {
      $terms[] = _convivial_bootstrap_glossary_get_terms($content_type, 'link');
    }

    // Merge all arrays into one.
    $terms = array_merge(...$terms);

    foreach ($terms as $term => $properties) {
      if (strpos($body, $term) === FALSE) {
        unset($terms[$term]);
      }
      else {
        $terms[$term]['status'] = TRUE;
      }
    }

    // Create DOM.
    $dom = Html::load($body);

    // Get all text nodes.
    $x_path = new \DOMXPath($dom);
    $text_nodes = $x_path->evaluate("//text()");
    foreach ($text_nodes as $text_node) {

      $fragments = _convivial_bootstrap_glossary_get_fragments($text_node, $terms);

      // Skip processing this text node if no matches found. We need to preserve spaces.
      if (count($fragments) < 2) {
        continue;
      }

      // Rebuild TextNode as a series of TextNode, tooltip and link elements.
      foreach ($fragments as $key => $fragment) {
        // When text should be marked up.
        if (isset($terms[$fragment])) {
          // TODO: This doesn't work well if there are multiple instances of
          // term inside of different HTML elements.
          if (!isset($tags_blacklist[$text_node->parentNode->tagName])) {
            $element = _convivial_bootstrap_glossary_add_widget($dom, $fragment, $terms);
          }
          else {
            $element = $dom->createTextNode($fragment);
            $terms[$fragment]['status'] = FALSE;
          }

        }
        // When text should be a TextNode.
        else {
          $element = $dom->createTextNode($fragment);
        }
        // Add new element before original text.
        $text_node->parentNode->insertBefore($element, $text_node);
      }
      // Remove the original text.
      $text_node->parentNode->removeChild($text_node);
    }
    $html = Html::serialize($dom);
    $html .= _convivial_bootstrap_glossary_add_definition_list($terms);

    $variables["items"][0]["content"]["#text"] = $html;
  }
}

/**
 * Get nodes of a given content type and augment with important properties.
 *
 * @param string $content_type
 *   Bundle of node entity type.
 * @param string $widget
 *   Widget to use for the term.
 *
 * @return array
 *   Array of terms.
 */
function _convivial_bootstrap_glossary_get_terms($content_type, $widget) {
  $terms = [];
  $nodes = \Drupal::entityTypeManager()
    ->getStorage('node')
    ->loadByProperties(['type' => $content_type]);

  /** @var \Drupal\node\Entity\Node $node */
  foreach ($nodes as $nid => $node) {
    $body = $node->get('field_body')->value;
    if (empty($body)) {
      $body = '';
    }
    $terms[$node->label()] = [
      'definition' => $body,
      'widget' => $widget,
      'url' => $node->toUrl()->toString(),
    ];
  }

  return $terms;
}

/**
 * Search for presence of terms on text part.
 *
 * @param \DOMText $text_node
 *   DOMText representation of text part.
 * @param array $terms
 *   List of terms.
 *
 * @return array[]|false|string[]
 *   Array of preg_split results.
 */
function _convivial_bootstrap_glossary_get_fragments(\DOMText &$text_node, array $terms) {
  // Create a regex that matches on all terms.
  $terms_regex = '/\b(' . implode('|', array_keys($terms)) . ')\b/';
  // Splice text into an array of "before", "match" and "after" strings.
  $fragments = preg_split($terms_regex, $text_node->nodeValue, NULL, PREG_SPLIT_DELIM_CAPTURE);

  return $fragments;
}

/**
 * Enhance a fragment with additional markup according to widget type.
 *
 * @param DOMDocument $dom
 *   Node converted to the DOMDocument.
 * @param string $fragment
 *   Term to add the widget to.
 * @param array $terms
 *   Definitions of terms found in the document.
 *
 * @return \DOMElement
 *   Modified element.
 */
function _convivial_bootstrap_glossary_add_widget(DOMDocument &$dom, $fragment, array $terms) {
  $widget = $terms[$fragment]['widget'];
  switch ($widget) {
    case 'tooltip':
      $definition = $terms[$fragment]['definition'];
      // Remove HTML tags and newlines. We're using standard browser-based tooltips.
      $definition = str_replace(["\r", "\n"], '', strip_tags($definition));
      $anchor = 'term-' . strtolower($fragment);

      // Create a <dfn> element.
      $element = $dom->createElement('dfn');
      // Add TextNode to <dfn> element.
      $element->appendChild($dom->createTextNode($fragment));
      $element->setAttribute('title', $definition);
      // Add a describedby attribute connecting term to definition.
      $element->setAttribute('aria-describedby', $anchor);
      // Add a role attribute.
      $element->setAttribute('role', 'tooltip');
      break;

    case 'link':
      $url = $terms[$fragment]['url'];
      // Create a <a> element.
      $element = $dom->createElement('a');
      // Add TextNode to <a> element.
      $element->appendChild($dom->createTextNode($fragment));
      // Add a title attribute containing the definition text.
      $element->setAttribute('href', $url);
      break;
  }

  return $element;
}

/**
 * Helper function which generates definition list of terms contained in node.
 *
 * @param array $terms
 *   Array of terms.
 *
 * @return string
 *   Generated HTML.
 */
function _convivial_bootstrap_glossary_add_definition_list(array $terms) {

  $enabled_terms = [];
  foreach ($terms as $term => $properties) {
    if ($properties['status'] === TRUE && $properties['widget'] === 'tooltip') {
      $enabled_terms[] = $term;
    }
  }

  $html = '';
  // Definition container.
  if (!empty($enabled_terms)) {
    $html .= '<div class="glossary-definition-list">';
    $html .= '<h2>' . t('Definitions') . '</h2>';
    // Definition list.
    $html .= '<dl>';
  }

  foreach ($enabled_terms as $term) {
    $html .= '<dt>' . $term . '</dt>';
    $html .= '<dd id="term-' . strtolower($term) . '">';
    $html .= $terms[$term]['definition'];
    $html .= '</dd>';
  }
  if (!empty($enabled_terms)) {
    $html .= '</dl>';
    // End definition container.
    $html .= '</div>';
  }
  return $html;
}
