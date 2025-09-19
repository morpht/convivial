<?php

/**
 * @file
 * Theme settings form.
 *
 * @see convivial_bootstrap_form_system_theme_settings_alter()
 */

use Drupal\Core\Form\FormStateInterface;

require_once 'theme_ui.inc';

/**
 * Attach theme settings form.
 */
function _convivial_bootstrap_settings(&$form, FormStateInterface $form_state) {
  // Create a section for Convivial theme settings.
  $form['convivial_theme_settings'] = [
    '#type' => 'vertical_tabs',
    '#title' => t('Theme settings'),
    '#description' => t('Configure options'),
    '#weight' => -1000,
    '#open' => TRUE,
  ];

  $groups = [
    'theme_ui' => t('Theme UI'),
    'header' => t('Header'),
    'page' => t('Page'),
    'footer' => t('Footer'),
    'laf' => t('Look and feel'),
    'charts' => t('Charts'),
    'glossary' => t('Glossary'),
    'external_links' => t('External links'),
    'debugging' => t('Debugging'),
    'icon' => t('Icon'),
    'redirect_import' => t('Import redirects'),
    'meta' => t('Meta'),
  ];

  foreach ($groups as $group => $title) {
    $form[$group] = [
      '#type' => 'details',
      '#title' => $title,
      '#group' => 'convivial_theme_settings',
    ];
  }

  $form['header']['general'] = [
    '#type' => 'details',
    '#title' => t('General'),
    '#open' => TRUE,
  ];
  $form['header']['general']['header_style'] = [
    '#type' => 'select',
    '#title' => t('Color scheme'),
    '#options' => [
      '' => t('-none-'),
      'theme--standard' => t('Standard'),
      'theme--alternate' => t('Alternate'),
      'theme--primary' => t('Primary'),
      'theme--secondary' => t('Secondary'),
      'theme--tertiary' => t('Tertiary'),
      'theme--light' => t('Light'),
      'theme--dark' => t('Dark'),
    ],
    '#description' => t('Set style for header.'),
    '#default_value' => theme_get_setting('header_style'),
  ];
  $form['header']['general']['header_stickiness'] = [
    '#type' => 'radios',
    '#title' => t('Placement'),
    '#options' => [
      'header_stickiness--non-sticky' => t('Static'),
      'header_stickiness--sticky' => t('Sticky'),
    ],
    '#description' => t('A Static header remains in place while a Sticky header will stick to the top of the screen when you scroll.'),
    '#default_value' => theme_get_setting('header_stickiness'),
  ];
  $form['header']['general']['header_container'] = [
    '#type' => 'radios',
    '#title' => t('Layout'),
    '#options' => [
      'header_container--fluid' => t('Fluid-width'),
      'header_container--contained' => t('Contained'),
    ],
    '#description' => t('Set whether the Header should be Contained within the website’s outer boundary or unconstrained and Fluid-width.'),
    '#default_value' => theme_get_setting('header_container'),
  ];

  $form['header']['primary_navigation'] = [
    '#type' => 'details',
    '#title' => t('Primary navigation'),
    '#open' => TRUE,
  ];
  $form['header']['primary_navigation']['menu_style'] = [
    '#type' => 'radios',
    '#title' => t('Type'),
    '#options' => [
      'menu-style--simple' => t('Simple (mobile open down) – 1 level'),
      'menu-style--dropdown' => t('Dropdown – up to 3 levels'),
      'menu-style--megamenu' => t('Megamenu – up to 4 levels'),
      'menu-style--overview' => t('Megamenu (description first level) – up to 4 levels'),
      'menu-style--description' => t('Megamenu (descriptions all levels) – up to 4 levels'),
    ],
    '#description' => t('Select the type of menu to use for the Primary Navigation.
"Description" option uses the standard Drupal menu items description field.<br>
Note that if <strong>Display: Inline menu</strong> is set, the menu type will be always
<strong>Simple (mobile slide out) – 1 level</strong>.'),
    '#default_value' => theme_get_setting('menu_style'),
  ];
  $form['header']['primary_navigation']['megamenu_columns'] = [
    '#type' => 'radios',
    '#title' => t('Megamenu columns'),
    '#options' => [
      'megamenu-columns--2' => t('Two columns'),
      'megamenu-columns--3' => t('Three columns'),
      'megamenu-columns--4' => t('Four columns'),
      'megamenu-columns--5' => t('Five columns'),
      'megamenu-columns--6' => t('Six columns'),
    ],
    '#description' => t('In to how many columns should the items be sorted.'),
    '#default_value' => theme_get_setting('megamenu_columns'),
    '#states' => [
      // Display only if menu_style is set to megamenu, overview or description.
      'visible' => [
        'input[name="menu_style"]' => [
          ['value' => 'menu-style--megamenu'],
          ['value' => 'menu-style--overview'],
          ['value' => 'menu-style--description'],
        ],
      ],
    ],
  ];
  $form['header']['primary_navigation']['header_layout'] = [
    '#type' => 'radios',
    '#title' => t('Display'),
    '#options' => [
      'header_layout--inline-menu' => t('Inline menu'),
      'header_layout--menu-bar' => t('Menu bar'),
      'header_layout--no-header' => t('No header'),
    ],
    '#description' => t('Set the layout for the header.'),
    '#default_value' => theme_get_setting('header_layout'),
  ];

  $form['header']['search_form'] = [
    '#type' => 'details',
    '#title' => t('Search form'),
    '#open' => TRUE,
  ];
  $form['header']['search_form']['search_form_display'] = [
    '#type' => 'radios',
    '#title' => t('Display'),
    '#options' => [
      'search-form--inline' => t('Inline'),
      'search-form--inline-reveal' => t('Inline reveal'),
      'search-form--top-reveal' => t('Top reveal'),
    ],
    '#description' => t('Set how the search form is displayed. An Inline search form will remain visible on larger screens while reveal options toggle their visibility by clicking a button.'),
    '#default_value' => theme_get_setting('search_form_display'),
  ];

  $form['page']['top_bar_style'] = [
    '#type' => 'select',
    '#title' => t('Top bar style'),
    '#options' => [
      '' => t('-none-'),
      'theme--light' => t('Light'),
      'theme--dark' => t('Dark'),
      'theme--primary' => t('Primary'),
      'theme--primary-dark' => t('Primary dark'),
    ],
    '#description' => t('Set style for the top bar region.'),
    '#default_value' => theme_get_setting('top_bar_style'),
  ];
  $form['page']['main_nav_style'] = [
    '#type' => 'select',
    '#title' => t('Main navigation style'),
    '#options' => [
      '' => t('-none-'),
      'theme--standard' => t('Standard'),
      'theme--alternate' => t('Alternate'),
      'theme--primary' => t('Primary'),
      'theme--secondary' => t('Secondary'),
      'theme--tertiary' => t('Tertiary'),
      'theme--light' => t('Light'),
      'theme--dark' => t('Dark'),
    ],
    '#description' => t('Set style for the main navigation region.'),
    '#default_value' => theme_get_setting('main_nav_style'),
  ];
  $form['page']['postscript_style'] = [
    '#type' => 'select',
    '#title' => t('Postscript style'),
    '#options' => [
      '' => t('-none-'),
      'theme--standard' => t('Standard'),
      'theme--alternate' => t('Alternate'),
      'theme--primary' => t('Primary'),
      'theme--secondary' => t('Secondary'),
      'theme--tertiary' => t('Tertiary'),
      'theme--light' => t('Light'),
      'theme--dark' => t('Dark'),
    ],
    '#description' => t('Set style for postscript region.'),
    '#default_value' => theme_get_setting('postscript_style'),
  ];

  $form['footer']['footer_style'] = [
    '#type' => 'select',
    '#title' => t('Footer style'),
    '#options' => [
      '' => t('-none-'),
      'theme--standard' => t('Standard'),
      'theme--alternate' => t('Alternate'),
      'theme--primary' => t('Primary'),
      'theme--secondary' => t('Secondary'),
      'theme--tertiary' => t('Tertiary'),
      'theme--light' => t('Light'),
      'theme--dark' => t('Dark'),
    ],
    '#description' => t('Set style for footer region.'),
    '#default_value' => theme_get_setting('footer_style'),
  ];
  $form['footer']['footer_info_style'] = [
    '#type' => 'select',
    '#title' => t('Footer info style'),
    '#options' => [
      '' => t('-none-'),
      'theme--standard' => t('Standard'),
      'theme--alternate' => t('Alternate'),
      'theme--primary' => t('Primary'),
      'theme--secondary' => t('Secondary'),
      'theme--tertiary' => t('Tertiary'),
      'theme--light' => t('Light'),
      'theme--dark' => t('Dark'),
    ],
    '#description' => t('Set style for footer info region.'),
    '#default_value' => theme_get_setting('footer_info_style'),
  ];

  $form['debugging']['highlighted_layouts'] = [
    '#type' => 'checkbox',
    '#title' => t('Layout highlighter'),
    '#description' => t('Default Convivial layouts get borders for easier debugging. Do not forgot to turn off before going live.'),
    '#default_value' => theme_get_setting('highlighted_layouts'),
  ];

  $form['external_links']['mark_external_links'] = [
    '#type' => 'checkbox',
    '#title' => t('Mark external links'),
    '#description' => t('Enable special styling for external links.'),
    '#default_value' => theme_get_setting('mark_external_links'),
  ];

  $form['charts']['chart_colors'] = [
    '#type' => 'textfield',
    '#title' => t('Chart colors'),
    '#description' => t('Hex color codes separated by a space. Example: <pre>#111111 #222222 #333333</pre>'),
    '#maxlength' => 256,
    '#default_value' => theme_get_setting('chart_colors'),
  ];

  $form['glossary']['glossary_enable'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable glossary'),
    '#description' => t('Enable adding tooltips and links to text, based on following settings.'),
    '#default_value' => theme_get_setting('glossary_enable'),
  ];
  $form['glossary']['target'] = [
    '#type' => 'fieldset',
    '#title' => t('Target content'),
    '#description' => t('Content that should be modified using glossary functionality.'),
  ];
  $form['glossary']['target']['glossary_bundles'] = [
    '#type' => 'checkboxes',
    '#title' => t('Node types to process'),
    '#description' => t('Nodes of selected types will be enhanced with tooltips.'),
    '#options' => _convivial_bootstrap_get_entity_bundles(),
    '#default_value' => theme_get_setting('glossary_bundles'),
    '#multiple' => TRUE,
  ];
  $form['glossary']['sources'] = [
    '#type' => 'fieldset',
    '#title' => t('Glossary sources'),
    '#description' => t('Content types that should be used for providing glossary definitions.'),
  ];
  $form['glossary']['sources']['glossary_bundles_tooltip_sources'] = [
    '#type' => 'checkboxes',
    '#title' => t('Tooltip sources'),
    '#description' => t('Node types that should be available as tooltip sources.'),
    '#options' => _convivial_bootstrap_get_entity_bundles(),
    '#default_value' => theme_get_setting('glossary_bundles_tooltip_sources'),
    '#multiple' => TRUE,
  ];
  $form['glossary']['sources']['glossary_bundles_link_sources'] = [
    '#type' => 'checkboxes',
    '#title' => t('Link sources'),
    '#description' => t('Node types that should be available as link sources.'),
    '#options' => _convivial_bootstrap_get_entity_bundles(),
    '#default_value' => theme_get_setting('glossary_bundles_link_sources'),
    '#multiple' => TRUE,
  ];
  $form['glossary']['glossary_tags_blacklist'] = [
    '#type' => 'textfield',
    '#title' => t('Tags blacklist'),
    '#description' => t("List of comma-separated HTML tags, that shouldn't be modified by the glossary."),
    '#maxlength' => 256,
    '#default_value' => theme_get_setting('glossary_tags_blacklist'),
  ];

  $form['laf']['laf_enable'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable Look and Feel'),
    '#description' => t('Apply Look and Feel feature.'),
    '#default_value' => theme_get_setting('laf_enable'),
  ];
  $form['laf']['laf_selectors'] = [
    '#type' => 'textarea',
    '#title' => t('Field Selectors'),
    '#description' => t('Define which elements the modifier should be applied to. Pipe-separated mappings of the fields to selectors (e.g. "field_teaser_modifiers|.teaser").'),
    '#default_value' => theme_get_setting('laf_selectors'),
  ];
  $form['laf']['laf_completes'] = [
    '#type' => 'textarea',
    '#title' => t('Complete strategies'),
    '#description' => t('Assign a complete Look and Feel via an ordered list of strategies and parameters (e.g. "reference|field_section"). The first match will be applied.'),
    '#default_value' => theme_get_setting('laf_completes'),
  ];
  $form['laf']['laf_partials'] = [
    '#type' => 'textarea',
    '#title' => t('Partial strategies'),
    '#description' => t('Assign a partial Look and Feel via an ordered list of strategies and parameters (e.g. "reference|field_section"). All matches will be applied.'),
    '#default_value' => theme_get_setting('laf_partials'),
  ];

  $icon_aspect_ratio = theme_get_setting('icon_aspect_ratio');
  $form['icon']['icon_aspect_ratio'] = [
    '#type' => 'number',
    '#title' => t('Icon aspect ratio'),
    '#description' => t('The width to height ration to apply to icons.'),
    '#default_value' => !empty($icon_aspect_ratio) ? $icon_aspect_ratio : 1.414,
    '#step' => '0.001',
  ];

  $form['redirect_import']['csv'] = [
    '#type' => 'textarea',
    '#title' => t('Redirect CSV'),
    '#description' => t('Enter the CSV content for redirect import. The
      content should contain 2 columns "source" and "destination" with no title row.<br /><br />
      <strong>Example</strong><br />
      /page-1, /<br />
      /page-2, /node/2<br />
      '),
  ];

  $form['meta']['#tree'] = TRUE;
  $form['meta']['meta_enable'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable Meta'),
    '#description' => t('Expose current page related data into the data layer.'),
    '#default_value' => theme_get_setting('meta.meta_enable'),
  ];
  $form['meta']['meta_strategies_name'] = [
    '#type' => 'textarea',
    '#title' => t('Meta tags with "name" attribute'),
    '#description' => t('Define which data should be exposed as meta tags. See theme\'s README.md for examples.'),
    '#default_value' => theme_get_setting('meta.meta_strategies_name'),
  ];
  $form['meta']['meta_strategies_property'] = [
    '#type' => 'textarea',
    '#title' => t('Meta tags with "property" attribute'),
    '#description' => t('Define which data should be exposed as meta tags. See theme\'s README.md for examples.'),
    '#default_value' => theme_get_setting('meta.meta_strategies_property'),
  ];

  _add_theme_ui_settings($form);

  // Flush render cache.
  $form['#submit'][] = '_convivial_bootstrap_flush_render_cache';

  // System theme settings form submit.
  $form['#submit'][] = '_convivial_bootstrap_form_system_theme_settings_form_submit';
}

/**
 * System theme settings form submit handler.
 */
function _convivial_bootstrap_form_system_theme_settings_form_submit(array $form, FormStateInterface $form_state) {
  // Import redirects.
  if (!$form_state->isValueEmpty('csv')) {
    require_once __DIR__ . '/import_redirects.inc';
    _convivial_bootstrap_import_redirects($form_state);
  }
}
