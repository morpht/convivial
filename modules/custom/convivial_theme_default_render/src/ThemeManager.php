<?php

namespace Drupal\convivial_theme_default_render;

use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Routing\RouteMatchInterface;
use Drupal\Core\Theme\ActiveTheme;
use Drupal\Core\Theme\Registry;
use Drupal\Core\Theme\ThemeInitializationInterface;
use Drupal\Core\Theme\ThemeManager as CoreThemeManager;
use Drupal\Core\Theme\ThemeManagerInterface;

/**
 * Service decorator which renders entities on admin pages with front theme.
 */
class ThemeManager extends CoreThemeManager implements ThemeManagerInterface {

  /**
   * Decorated service.
   *
   * @var \Drupal\Core\Theme\ThemeManager
   */
  protected $themeManager;

  /**
   * Theme initialization.
   *
   * @var \Drupal\Core\Theme\ThemeInitializationInterface
   */
  protected $themeInitialization;

  /**
   * Config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * The service ID
   *
   * @var string
   */
  public $_serviceId;

  /**
   * ThemeManager constructor.
   */
  public function __construct(
    CoreThemeManager $theme_manager,
    ThemeInitializationInterface $theme_initialization,
    ConfigFactoryInterface $config_factory
  ) {
    $this->themeManager = $theme_manager;
    $this->themeInitialization = $theme_initialization;
    $this->configFactory = $config_factory;
  }

  /**
   * {@inheritdoc}
   */
  public function render($hook, array $variables) {
    $overloaded = FALSE;

    // We need to make sure to not render nested <form> elements in edit forms.
    // Webform has a nice code https://git.drupalcode.org/project/webform/-/blob/ea1765bfc1d6052bfcc10c6b7cfbadbc1fb72643/src/Plugin/Field/FieldFormatter/WebformEntityReferenceEntityFormatter.php#L142
    // and related issue: https://www.drupal.org/project/webform/issues/3122506
    if ($hook === 'paragraph' && isset($variables['#array_parents'][1]) && $variables['#array_parents'][1] === 'widget') {
      // Render early to check for potentially problematic parts.
      $render = $this->themeManager->render($hook, $variables);
      if((strpos($render, '<!-- nopreview -->') !== false) || (strpos($render, '<form ') !== false)) {
        // It's hard to alter $variables properly as it's structure
        // is changing quite a bit.
        // Also, paragraph--hero-search.html.twig has hardcoded form
        // which can't be altered from here.
        // Triggers explained:
        // '<!–– nopreview ––>'
        // * HTML comment to make it possible to indicate no render from content
        //   editor side.
        // '<form '
        // * We don't want "Form in a form". This is best explained in issues
        //   linked in webform related comments above.
        // @ToDo: Use t().
        $render = "Paragraph preview containing another form can not be previewed when editing content.";
      }
    }

    if ($hook === 'paragraph') {
      $front_theme_name = $this->configFactory->get('system.theme')->get('default');
      $current_theme = $this->themeInitialization->getActiveThemeByName($front_theme_name);
      $this->themeManager->setActiveTheme($current_theme);
      $variables['#overloaded'] = $overloaded = TRUE;
    }

    // Normal render or render of the nested <form> warning.
    if(!isset($render)) {
      $render = $this->themeManager->render($hook, $variables);
    }

    if ($overloaded) {
      $this->themeManager->resetActiveTheme();
    }
    return $render;
  }

  /**
   * {@inheritdoc}
   */
  public function getActiveTheme(RouteMatchInterface $route_match = NULL) {
    return $this->themeManager->getActiveTheme($route_match);
  }

  /**
   * {@inheritdoc}
   */
  public function hasActiveTheme() {
    return $this->themeManager->hasActiveTheme();
  }

  /**
   * {@inheritdoc}
   */
  public function resetActiveTheme() {
    return $this->themeManager->resetActiveTheme();
  }

  /**
   * {@inheritdoc}
   */
  public function setActiveTheme(ActiveTheme $active_theme) {
    return $this->themeManager->setActiveTheme($active_theme);
  }

  /**
   * {@inheritdoc}
   */
  public function alter($type, &$data, &$context1 = NULL, &$context2 = NULL) {
    $this->themeManager->alter($type, $data, $context1, $context2);
  }

  /**
   * {@inheritdoc}
   */
  public function alterForTheme(ActiveTheme $theme, $type, &$data, &$context1 = NULL, &$context2 = NULL) {
    $this->themeManager->alterForTheme($theme, $type, $data, $context1, $context2);
  }

  /**
   * {@inheritdoc}
   *
   * @see \Drupal\Core\Theme\ThemeManager::setThemeRegistry()
   */
  public function setThemeRegistry(Registry $theme_registry) {
    return $this->themeManager->setThemeRegistry($theme_registry);
  }

}
