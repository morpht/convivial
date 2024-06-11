<?php

use Drupal\Component\Render\MarkupInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\user\Entity\User;

/**
 * Class for formatting a string to inject to html head.
 */
class convivial_bootstrap_javascript_snippet implements MarkupInterface {

  /**
   * The string to escape.
   *
   * @var string
   */
  protected $string;

  /**
   * Constructs an HtmlEscapedText object.
   *
   * @param string $string
   *   The string to escape. This value will be cast to a string.
   */
  public function __construct($string) {
    $this->string = (string) $string;
  }

  /**
   * {@inheritdoc}
   */
  public function __toString() {
    return $this->string;
  }

  /**
   * {@inheritdoc}
   */
  #[\ReturnTypeWillChange]
  public function jsonSerialize() {
    return $this->__toString();
  }

}

/**
 * Add Matomo to theme settings.
 */
function _convivial_bootstrap_matomo_theme_settings(&$form) {
  $form['matomo_settings'] = [
    '#type' => 'details',
    '#title' => t('Matomo'),
    '#group' => 'convivial_bootstrap_settings',
  ];

  $form['matomo_settings']['#tree'] = TRUE;
  $form['matomo_settings']['#markup'] = t('Matomo web statistics tracking system to your website.');
  $form['matomo_settings']['tracking'] = [
    '#type' => 'checkbox',
    '#title' => t('Enabled'),
    '#description' => t('Enable Matomo tracking'),
    '#default_value' => theme_get_setting('matomo_settings.tracking'),
  ];
  $form['matomo_settings']['site_id'] = [
    '#type' => 'textfield',
    '#title' => t('Site ID'),
    '#description' => t('The Site ID on the Matomo server'),
    '#default_value' => theme_get_setting('matomo_settings.site_id'),
  ];
  $form['matomo_settings']['server'] = [
    '#type' => 'url',
    '#title' => t('Server'),
    '#description' => t('The URL of the Matomo server'),
    '#default_value' => theme_get_setting('matomo_settings.server'),
  ];
  $form['matomo_settings']['dimensions'] = [
    '#type' => 'textarea',
    '#title' => t('Dimensions'),
    '#description' => t('Line separated list of values (key, strategy, dimension id, default) which will be sent as dimensions on pageviews. eg.<br><br>
    type,meta,1<br>
    utm_campaign,param,2<br>
    city,cookie,3<br>
    topic,localstorage,4<br>
    personalise,localstorage,5,1'),
    '#default_value' => theme_get_setting('matomo_settings.dimensions'),
  ];

  $roles = user_roles();
  $form['matomo_settings']['user_roles'] = [
    '#type' => 'fieldset',
    '#title' => t('User Roles'),
    '#description' => t('Select which user roles the script should be displayed for.'),
  ];

  foreach ($roles as $rid => $name) {
    $form['matomo_settings']['user_roles'][$rid] = [
      '#type' => 'checkbox',
      '#title' => $name->label(),
      '#default_value' => theme_get_setting('matomo_settings.user_roles.' . $rid),
    ];
  }
  $form['#validate'][] = '_convivial_bootstrap_matomo_theme_settings_validate';
}

/**
 * Validate handler for the Matomo theme settings.
 */
function _convivial_bootstrap_matomo_theme_settings_validate(&$form, FormStateInterface $form_state) {
  $settings = $form_state->getValue('matomo_settings');
  // If Matomo tracking is enabled; Make site_id and server mandatory.
  if ($settings['tracking']) {
    if (empty($settings['site_id'])) {
      $form_state->setErrorByName('matomo_settings[site_id]', t('Matomo tracking is enabled so Site ID is required.'));
    }
    if (empty($settings['server'])) {
      $form_state->setErrorByName('matomo_settings[server]', t('Matomo tracking is enabled so Server is required.'));
    }
  }
}

/**
 * Add Matomo script to the pages.
 */
function _convivial_bootstrap_matomo_tracking_script(array &$variables) {
  $matomo_settings = theme_get_setting('matomo_settings');
  if (!isset($matomo_settings['tracking']) || !is_int($matomo_settings['tracking'])) {
    return;
  }

  if (!empty($matomo_settings['dimensions'])) {
    $dimensions = array_map(function ($item) {
      return explode(',', preg_replace('/\s+/', '', $item));
    }, explode(PHP_EOL, $matomo_settings['dimensions']));

    foreach ($dimensions as $dimension) {
      if (count($dimension) == 3 || count($dimension) == 4) {
        $matomo_settings['all_dimensions'][] = [
          'key' => $dimension[0],
          'strategy' => $dimension[1],
          'id' => $dimension[2],
          'default' => $dimension[3] ?? FALSE,
        ];
      }
    }
  }

  // Get all user roles.
  $roles = user_roles();

  // Get the current user object.
  $current_user = \Drupal::currentUser();

  // Load the user object for the current user.
  $user = User::load($current_user->id());

  // Get an array of the roles for the current user.
  $user_roles = $user->getRoles();

  foreach ($roles as $rid => $name) {
    // If proper role is selected in admin and current user has that role.
    if (theme_get_setting('matomo_settings.user_roles.' . $rid) && in_array($rid, $user_roles)) {
      $matomo_settings['show_for_current_role'] = TRUE;
    }
  }

  // Check if Matomo settings are valid.
  if (isset($matomo_settings['show_for_current_role'])) {
    $tracker_js = "function matomo_get_meta_values(key) {return Array.from(document.head.querySelectorAll('[name='+key+'][content]')).map(meta => meta.getAttribute('content')).join(',')};";
    $tracker_js .= "function isLocalStorageAvailable() { try { if (typeof localStorage === 'undefined') { return false; } var test = 'test'; localStorage.setItem(test, test); localStorage.removeItem(test); return true; } catch(e) { console.log('Matomo analytics may not work optimally because the browser local storage is not enabled or accessible.'); return false; } }";

    $tracker_js .= "window.addEventListener('load', function() {var _paq = window._paq = window._paq || [];";

    //Loop through all dimensions and add custom dimension tracking code to the tracker JS.
    foreach ($matomo_settings['all_dimensions'] as $dimension) {
      switch ($dimension['strategy']) {
        case 'meta':
          $tracker_js .= "var value = matomo_get_meta_values('{$dimension['key']}') ?? '{$dimension['default']}';if(value){_paq.push(['setCustomDimension', customDimensionId = {$dimension['id']}, customDimensionValue = value])};";
          break;

        case 'localstorage':
          $tracker_js .= "if (isLocalStorageAvailable()) { ";
          $tracker_js .= "var value = localStorage.getItem('{$dimension['key']}') ?? '{$dimension['default']}';if(value){_paq.push(['setCustomDimension', customDimensionId = {$dimension['id']}, customDimensionValue = value ])};\n";
          $tracker_js .= "}";
          break;

        case 'param':
          $tracker_js .= "var value = urlParams.get('{$dimension['key']}') ?? '{$dimension['default']}';if(value){_paq.push(['setCustomDimension', customDimensionId = {$dimension['id']}, customDimensionValue = value])};";
          break;

        case 'cookie':
          $tracker_js .= "var value = document.cookie.split('; ').find(row => row.startsWith('{$dimension['key']}=')).split('=')[1] ?? '{$dimension['default']}';if(value){_paq.push(['setCustomDimension', customDimensionId = {$dimension['id']}, customDimensionValue = value])};";
          break;
      }
    }

    $tracker_js .= "_paq.push(['trackPageView']);_paq.push(['enableLinkTracking']);(function () {var u = \"{$matomo_settings['server']}\";_paq.push(['setTrackerUrl', u + 'matomo.php']);_paq.push(['setSiteId', \"{$matomo_settings['site_id']}\"]);var d = document, g = d.createElement('script'), s = d.getElementsByTagName('script')[0];g.async = true;g.src = '//cdn.matomo.cloud/convivial.matomo.cloud/matomo.js';s.parentNode.insertBefore(g, s);})();});";
    $tracker_js_snippet = new convivial_bootstrap_javascript_snippet($tracker_js);
    $variables['#attached']['html_head'][] = [
      [
        '#tag' => 'script',
        '#value' => $tracker_js_snippet->__toString(),
      ],
      'matomo_tracking_script',
    ];
  }
}
