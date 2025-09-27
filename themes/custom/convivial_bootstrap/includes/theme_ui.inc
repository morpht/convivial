<?php

/**
 * @file
 * Color palette settings.
 */

/**
 * Add Color Palettes to theme settings.
 */
function _add_theme_ui_settings(&$form) {
  $form['theme_ui']['#tree'] = TRUE;
  $form['theme_ui']['#attributes'] = [
    'class' => ['theme-ui'],
  ];

  $form['theme_ui']['enable'] = [
    '#type' => 'checkbox',
    '#title' => t('Enable theme settings'),
    '#description' => t('Enable theme features.'),
    '#default_value' => theme_get_setting('theme_ui.enable'),
  ];

  $form['theme_ui']['color_schemes'] = [
    '#type' => 'details',
    '#title' => t('Colour schemes'),
    '#open' => TRUE,
  ];

  $form['theme_ui']['radius'] = [
    '#type' => 'details',
    '#title' => t('Radius'),
    '#open' => FALSE,
  ];

  $form['theme_ui']['radius']['box'] = [
    '#type' => 'number',
    '#title' => t('Boxes'),
    '#description' => t('Apply to boxes, cards, tiles, modals, and alerts.'),
    '#default_value' => theme_get_setting('theme_ui.radius.box') ?: '0',
    '#step' => 0.01,
    '#field_suffix' => 'rem',
    '#attributes' => [
      'min' => 0,
      'max' => 100,
    ],
  ];

  $form['theme_ui']['radius']['field'] = [
    '#type' => 'number',
    '#title' => t('Fields'),
    '#description' => t('Apply to form elements such as inputs, selects, fields and tabs.'),
    '#default_value' => theme_get_setting('theme_ui.radius.field') ?: '0',
    '#step' => 0.01,
    '#field_suffix' => 'rem',
    '#attributes' => [
      'min' => 0,
      'max' => 100,
    ],
  ];

  $form['theme_ui']['radius']['selector'] = [
    '#type' => 'number',
    '#title' => t('Selectors'),
    '#description' => t('Apply to form elements such as checkboxes, toggle switches.'),
    '#default_value' => theme_get_setting('theme_ui.radius.selector') ?: '0',
    '#step' => 0.01,
    '#field_suffix' => 'rem',
    '#attributes' => [
      'min' => 0,
      'max' => 100,
    ],
  ];

  $form['theme_ui']['radius']['button'] = [
    '#type' => 'number',
    '#title' => t('Buttons'),
    '#description' => t('Apply to buttons and badges.'),
    '#default_value' => theme_get_setting('theme_ui.radius.button') ?: '0',
    '#step' => 0.01,
    '#field_suffix' => 'rem',
    '#attributes' => [
      'min' => 0,
      'max' => 100,
    ],
  ];

  $form['theme_ui']['radius']['image'] = [
    '#type' => 'number',
    '#title' => t('Images'),
    '#description' => t('Apply to images in content and teasers.'),
    '#default_value' => theme_get_setting('theme_ui.radius.image') ?: '0',
    '#step' => 0.01,
    '#field_suffix' => 'rem',
    '#attributes' => [
      'min' => 0,
      'max' => 100,
    ],
  ];

  $form['theme_ui']['radius']['default'] = [
    '#type' => 'number',
    '#title' => t('Default'),
    '#description' => t('Apply to remaining components.'),
    '#default_value' => theme_get_setting('theme_ui.radius.default') ?: '0',
    '#step' => 0.01,
    '#field_suffix' => 'rem',
    '#attributes' => [
      'min' => 0,
      'max' => 100,
    ],
  ];

  $color_schemes = [
    'default' => [
      'label' => 'Default scheme',
      'open' => TRUE,
    ],
    'dark' => [
      'label' => 'Dark scheme',
      'open' => FALSE,
    ],
  ];

  $color_palettes = [
    'standard' => [
      'label' => 'Standard',
      'base' => [
        'background_base' => [
          'label' => 'Background base',
          'default' => '#fcfcfc',
        ],
        'foreground_base' => [
          'label' => 'Foreground base',
          'default' => '#000000',
        ],
        'link_base' => [
          'label' => 'Link base',
          'default' => '#00698f',
        ],
        'accent_base' => [
          'label' => 'Accent base',
          'default' => '#8f0069',
        ],
      ],
      'derivatives' => [
        'background' => [
          'label' => 'Background',
          'default' => '#fcfcfc',
          'base' => 'background_base',
        ],
        'background_strong' => [
          'label' => 'Background strong',
          'default' => '#fdfdfd',
          'base' => 'background_base',
          'processor' => 'strengthen-20',
        ],
        'background_stronger' => [
          'label' => 'Background stronger',
          'default' => '#fdfdfd',
          'base' => 'background_base',
          'processor' => 'strengthen-30',
        ],
        'background_weak' => [
          'label' => 'Background weak',
          'default' => '#f6f6f6',
          'base' => 'background_base',
          'processor' => 'weaken-05',
        ],
        'background_weaker' => [
          'label' => 'Background weaker',
          'default' => '#b2b2b2',
          'base' => 'background_base',
          'processor' => 'weaken-50',
        ],
        'foreground' => [
          'label' => 'Foreground',
          'default' => '#000000',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'foreground_weak' => [
          'label' => 'Foreground weak',
          'default' => '#727272',
          'base' => 'foreground_base',
          'contrast' => 'background',
          'processor' => 'strengthen-20',
        ],
        'focus' => [
          'label' => 'Focus',
          'default' => '#000000',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'link' => [
          'label' => 'Link',
          'default' => '#00698f',
          'base' => 'link_base',
          'contrast' => 'background',
        ],
        'link_hover' => [
          'label' => 'Link hover',
          'default' => '#00516f',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'link_active' => [
          'label' => 'Link active',
          'default' => '#005e80',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'link_contrast' => [
          'label' => 'Link contrast',
          'default' => '#e4e9ed',
          'base' => 'link_base',
          'processor' => 'strengthen-80',
        ],
        'accent' => [
          'label' => 'Accent',
          'default' => '#8f0069',
          'base' => 'accent_base',
          'contrast' => 'background',
        ],
        'accent_hover' => [
          'label' => 'Accent hover',
          'default' => '#6f0051',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'accent_active' => [
          'label' => 'Accent active',
          'default' => '#80005e',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'accent_contrast' => [
          'label' => 'Accent contrast',
          'default' => '#ede4e9',
          'base' => 'accent_base',
          'processor' => 'strengthen-80',
        ],
      ],
    ],
    'alternate' => [
      'label' => 'Alternate',
      'base' => [
        'background_base' => [
          'label' => 'Background base',
          'default' => '#ebebeb',
        ],
        'foreground_base' => [
          'label' => 'Foreground base',
          'default' => '#000000',
        ],
        'link_base' => [
          'label' => 'Link base',
          'default' => '#00698f',
        ],
        'accent_base' => [
          'label' => 'Accent base',
          'default' => '#8f0069',
        ],
      ],
      'derivatives' => [
        'background' => [
          'label' => 'Background',
          'default' => '#ebebeb',
          'base' => 'background_base',
        ],
        'background_strong' => [
          'label' => 'Background strong',
          'default' => '#efefef',
          'base' => 'background_base',
          'processor' => 'strengthen-20',
        ],
        'background_stronger' => [
          'label' => 'Background stronger',
          'default' => '#f1f1f1',
          'base' => 'background_base',
          'processor' => 'strengthen-30',
        ],
        'background_weak' => [
          'label' => 'Background weak',
          'default' => '#e5e5e5',
          'base' => 'background_base',
          'processor' => 'weaken-05',
        ],
        'background_weaker' => [
          'label' => 'Background weaker',
          'default' => '#a6a6a6',
          'base' => 'background_base',
          'processor' => 'weaken-50',
        ],
        'foreground' => [
          'label' => 'Foreground',
          'default' => '#000000',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'foreground_weak' => [
          'label' => 'Foreground weak',
          'default' => '#727272',
          'base' => 'foreground_base',
          'contrast' => 'background',
          'processor' => 'strengthen-20',
        ],
        'focus' => [
          'label' => 'Focus',
          'default' => '#000000',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'link' => [
          'label' => 'Link',
          'default' => '#00698f',
          'base' => 'link_base',
          'contrast' => 'background',
        ],
        'link_hover' => [
          'label' => 'Link hover',
          'default' => '#00516f',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'link_active' => [
          'label' => 'Link active',
          'default' => '#005e80',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'link_contrast' => [
          'label' => 'Link contrast',
          'default' => '#e4e9ed',
          'base' => 'link_base',
          'processor' => 'strengthen-80',
        ],
        'accent' => [
          'label' => 'Accent',
          'default' => '#8f0069',
          'base' => 'accent_base',
          'contrast' => 'background',
        ],
        'accent_hover' => [
          'label' => 'Accent hover',
          'default' => '#6f0051',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'accent_active' => [
          'label' => 'Accent active',
          'default' => '#80005e',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'accent_contrast' => [
          'label' => 'Accent contrast',
          'default' => '#ede4e9',
          'base' => 'accent_base',
          'processor' => 'strengthen-80',
        ],
      ],
    ],
    'primary' => [
      'label' => 'Primary',
      'base' => [
        'background_base' => [
          'label' => 'Background base',
          'default' => '#135e70',
        ],
        'foreground_base' => [
          'label' => 'Foreground base',
          'default' => '#ffffff',
        ],
        'link_base' => [
          'label' => 'Link base',
          'default' => '#61daff',
        ],
        'accent_base' => [
          'label' => 'Accent base',
          'default' => '#ffb6ee',
        ],
      ],
      'derivatives' => [
        'background' => [
          'label' => 'Background',
          'default' => '#135e70',
          'base' => 'background_base',
        ],
        'background_strong' => [
          'label' => 'Background strong',
          'default' => '#115464',
          'base' => 'background_base',
          'processor' => 'strengthen-20',
        ],
        'background_stronger' => [
          'label' => 'Background stronger',
          'default' => '#104f5e',
          'base' => 'background_base',
          'processor' => 'strengthen-30',
        ],
        'background_weak' => [
          'label' => 'Background weak',
          'default' => '#3c6c7b',
          'base' => 'background_base',
          'processor' => 'weaken-05',
        ],
        'background_weaker' => [
          'label' => 'Background weaker',
          'default' => '#b5c0c5',
          'base' => 'background_base',
          'processor' => 'weaken-50',
        ],
        'foreground' => [
          'label' => 'Foreground',
          'default' => '#ffffff',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'foreground_weak' => [
          'label' => 'Foreground weak',
          'default' => '#e4e4e4',
          'base' => 'foreground_base',
          'contrast' => 'background',
          'processor' => 'strengthen-20',
        ],
        'focus' => [
          'label' => 'Focus',
          'default' => '#ffffff',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'link' => [
          'label' => 'Link',
          'default' => '#61daff',
          'base' => 'link_base',
          'contrast' => 'background',
        ],
        'link_hover' => [
          'label' => 'Link hover',
          'default' => '#8fe2ff',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'link_active' => [
          'label' => 'Link active',
          'default' => '#2b6172',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'link_contrast' => [
          'label' => 'Link contrast',
          'default' => '#2b6172',
          'base' => 'link_base',
          'processor' => 'strengthen-80',
        ],
        'accent' => [
          'label' => 'Accent',
          'default' => '#ffb6ee',
          'base' => 'accent_base',
          'contrast' => 'background',
        ],
        'accent_hover' => [
          'label' => 'Accent hover',
          'default' => '#ffd6f5',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'accent_active' => [
          'label' => 'Accent active',
          'default' => '#ffc7f1',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'accent_contrast' => [
          'label' => 'Accent contrast',
          'default' => '#72516a',
          'base' => 'accent_base',
          'processor' => 'strengthen-80',
        ],
      ],
    ],
    'secondary' => [
      'label' => 'Secondary',
      'base' => [
        'background_base' => [
          'label' => 'Background base',
          'default' => '#dbb2d0',
        ],
        'foreground_base' => [
          'label' => 'Foreground base',
          'default' => '#000000',
        ],
        'link_base' => [
          'label' => 'Link base',
          'default' => '#8f0069',
        ],
        'accent_base' => [
          'label' => 'Accent base',
          'default' => '#00698f',
        ],
      ],
      'derivatives' => [
        'background' => [
          'label' => 'Background',
          'default' => '#dbb2d0',
          'base' => 'background_base',
        ],
        'background_strong' => [
          'label' => 'Background strong',
          'default' => '#e3c4da',
          'base' => 'background_base',
          'processor' => 'strengthen-20',
        ],
        'background_stronger' => [
          'label' => 'Background stronger',
          'default' => '#e6ccdf',
          'base' => 'background_base',
          'processor' => 'strengthen-30',
        ],
        'background_weak' => [
          'label' => 'Background weak',
          'default' => '#d5adcb',
          'base' => 'background_base',
          'processor' => 'weaken-05',
        ],
        'background_weaker' => [
          'label' => 'Background weaker',
          'default' => '#9b7e93',
          'base' => 'background_base',
          'processor' => 'weaken-50',
        ],
        'foreground' => [
          'label' => 'Foreground',
          'default' => '#000000',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'foreground_weak' => [
          'label' => 'Foreground weak',
          'default' => '#727272',
          'base' => 'foreground_base',
          'contrast' => 'background',
          'processor' => 'strengthen-20',
        ],
        'focus' => [
          'label' => 'Focus',
          'default' => '#000000',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'link' => [
          'label' => 'Link',
          'default' => '#8f0069',
          'base' => 'link_base',
          'contrast' => 'background',
        ],
        'link_hover' => [
          'label' => 'Link hover',
          'default' => '#6f0051',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'link_active' => [
          'label' => 'Link active',
          'default' => '#80005e',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'link_contrast' => [
          'label' => 'Link contrast',
          'default' => '#ede4e9',
          'base' => 'link_base',
          'processor' => 'strengthen-80',
        ],
        'accent' => [
          'label' => 'Accent',
          'default' => '#00698f',
          'base' => 'accent_base',
          'contrast' => 'background',
        ],
        'accent_hover' => [
          'label' => 'Accent hover',
          'default' => '#00516f',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'accent_active' => [
          'label' => 'Accent active',
          'default' => '#005e80',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'accent_contrast' => [
          'label' => 'Accent contrast',
          'default' => '#e4e9ed',
          'base' => 'accent_base',
          'processor' => 'strengthen-80',
        ],
      ],
    ],
    'tertiary' => [
      'label' => 'Tertiary',
      'base' => [
        'background_base' => [
          'label' => 'Background base',
          'default' => '#8f0069',
        ],
        'foreground_base' => [
          'label' => 'Foreground base',
          'default' => '#ffffff',
        ],
        'link_base' => [
          'label' => 'Link base',
          'default' => '#ffb6ee',
        ],
        'accent_base' => [
          'label' => 'Accent base',
          'default' => '#61daff',
        ],
      ],
      'derivatives' => [
        'background' => [
          'label' => 'Background',
          'default' => '#8f0069',
          'base' => 'background_base',
        ],
        'background_strong' => [
          'label' => 'Background strong',
          'default' => '#80005e',
          'base' => 'background_base',
          'processor' => 'strengthen-20',
        ],
        'background_stronger' => [
          'label' => 'Background stronger',
          'default' => '#780058',
          'base' => 'background_base',
          'processor' => 'strengthen-30',
        ],
        'background_weak' => [
          'label' => 'Background weak',
          'default' => '#973975',
          'base' => 'background_base',
          'processor' => 'weaken-05',
        ],
        'background_weaker' => [
          'label' => 'Background weaker',
          'default' => '#cfb4c3',
          'base' => 'background_base',
          'processor' => 'weaken-50',
        ],
        'foreground' => [
          'label' => 'Foreground',
          'default' => '#ffffff',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'foreground_weak' => [
          'label' => 'Foreground weak',
          'default' => '#e4e4e4',
          'base' => 'foreground_base',
          'contrast' => 'background',
          'processor' => 'strengthen-20',
        ],
        'focus' => [
          'label' => 'Focus',
          'default' => '#ffffff',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'link' => [
          'label' => 'Link',
          'default' => '#ffb6ee',
          'base' => 'link_base',
          'contrast' => 'background',
        ],
        'link_hover' => [
          'label' => 'Link hover',
          'default' => '#ffd6f5',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'link_active' => [
          'label' => 'Link active',
          'default' => '#ffc7f1',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'link_contrast' => [
          'label' => 'Link contrast',
          'default' => '#72516a',
          'base' => 'link_base',
          'processor' => 'strengthen-80',
        ],
        'accent' => [
          'label' => 'Accent',
          'default' => '#61daff',
          'base' => 'accent_base',
          'contrast' => 'background',
        ],
        'accent_hover' => [
          'label' => 'Accent hover',
          'default' => '#b2eaff',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'accent_active' => [
          'label' => 'Accent active',
          'default' => '#8fe2ff',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'accent_contrast' => [
          'label' => 'Accent contrast',
          'default' => '#2b6172',
          'base' => 'accent_base',
          'processor' => 'strengthen-80',
        ],
      ],
    ],
    'light' => [
      'label' => 'Light',
      'base' => [
        'background_base' => [
          'label' => 'Background base',
          'default' => '#fcfcfc',
        ],
        'foreground_base' => [
          'label' => 'Foreground base',
          'default' => '#000000',
        ],
        'link_base' => [
          'label' => 'Link base',
          'default' => '#00698f',
        ],
        'accent_base' => [
          'label' => 'Accent base',
          'default' => '#8f0069',
        ],
      ],
      'derivatives' => [
        'background' => [
          'label' => 'Background',
          'default' => '#fcfcfc',
          'base' => 'background_base',
        ],
        'background_strong' => [
          'label' => 'Background strong',
          'default' => '#fdfdfd',
          'base' => 'background_base',
          'processor' => 'strengthen-20',
        ],
        'background_stronger' => [
          'label' => 'Background stronger',
          'default' => '#fdfdfd',
          'base' => 'background_base',
          'processor' => 'strengthen-30',
        ],
        'background_weak' => [
          'label' => 'Background weak',
          'default' => '#f6f6f6',
          'base' => 'background_base',
          'processor' => 'weaken-05',
        ],
        'background_weaker' => [
          'label' => 'Background weaker',
          'default' => '#b2b2b2',
          'base' => 'background_base',
          'processor' => 'weaken-50',
        ],
        'foreground' => [
          'label' => 'Foreground',
          'default' => '#000000',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'foreground_weak' => [
          'label' => 'Foreground weak',
          'default' => '#727272',
          'base' => 'foreground_base',
          'contrast' => 'background',
          'processor' => 'strengthen-20',
        ],
        'focus' => [
          'label' => 'Focus',
          'default' => '#000000',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'link' => [
          'label' => 'Link',
          'default' => '#00698f',
          'base' => 'link_base',
          'contrast' => 'background',
        ],
        'link_hover' => [
          'label' => 'Link hover',
          'default' => '#00516f',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'link_active' => [
          'label' => 'Link active',
          'default' => '#005e80',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'link_contrast' => [
          'label' => 'Link contrast',
          'default' => '#e4e9ed',
          'base' => 'link_base',
          'processor' => 'strengthen-80',
        ],
        'accent' => [
          'label' => 'Accent',
          'default' => '#8f0069',
          'base' => 'accent_base',
          'contrast' => 'background',
        ],
        'accent_hover' => [
          'label' => 'Accent hover',
          'default' => '#6f0051',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'accent_active' => [
          'label' => 'Accent active',
          'default' => '#80005e',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'accent_contrast' => [
          'label' => 'Accent contrast',
          'default' => '#ede4e9',
          'base' => 'accent_base',
          'processor' => 'strengthen-80',
        ],
      ],
    ],
    'dark' => [
      'label' => 'Dark',
      'base' => [
        'background_base' => [
          'label' => 'Background base',
          'default' => '#333333',
        ],
        'foreground_base' => [
          'label' => 'Foreground base',
          'default' => '#ffffff',
        ],
        'link_base' => [
          'label' => 'Link base',
          'default' => '#61daff',
        ],
        'accent_base' => [
          'label' => 'Accent base',
          'default' => '#ffb6ee',
        ],
      ],
      'derivatives' => [
        'background' => [
          'label' => 'Background',
          'default' => '#333333',
          'base' => 'background_base',
        ],
        'background_strong' => [
          'label' => 'Background strong',
          'default' => '#2e2e2e',
          'base' => 'background_base',
          'processor' => 'strengthen-20',
        ],
        'background_stronger' => [
          'label' => 'Background stronger',
          'default' => '#2b2b2b',
          'base' => 'background_base',
          'processor' => 'strengthen-30',
        ],
        'background_weak' => [
          'label' => 'Background weak',
          'default' => '#4c4c4c',
          'base' => 'background_base',
          'processor' => 'weaken-05',
        ],
        'background_weaker' => [
          'label' => 'Background weaker',
          'default' => '#b8b8b8',
          'base' => 'background_base',
          'processor' => 'weaken-50',
        ],
        'foreground' => [
          'label' => 'Foreground',
          'default' => '#ffffff',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'foreground_weak' => [
          'label' => 'Foreground weak',
          'default' => '#e4e4e4',
          'base' => 'foreground_base',
          'contrast' => 'background',
          'processor' => 'strengthen-20',
        ],
        'focus' => [
          'label' => 'Focus',
          'default' => '#ffffff',
          'base' => 'foreground_base',
          'contrast' => 'background',
        ],
        'link' => [
          'label' => 'Link',
          'default' => '#61daff',
          'base' => 'link_base',
          'contrast' => 'background',
        ],
        'link_hover' => [
          'label' => 'Link hover',
          'default' => '#b2eaff',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'link_active' => [
          'label' => 'Link active',
          'default' => '#8fe2ff',
          'base' => 'link_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'link_contrast' => [
          'label' => 'Link contrast',
          'default' => '#2b6172',
          'base' => 'link_base',
          'processor' => 'strengthen-80',
        ],
        'accent' => [
          'label' => 'Accent',
          'default' => '#ffd6f5',
          'base' => 'accent_base',
          'contrast' => 'background',
        ],
        'accent_hover' => [
          'label' => 'Accent hover',
          'default' => '#ffd6f5',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-40',
        ],
        'accent_active' => [
          'label' => 'Accent active',
          'default' => '#ffc7f1',
          'base' => 'accent_base',
          'contrast' => 'background',
          'processor' => 'weaken-20',
        ],
        'accent_contrast' => [
          'label' => 'Accent contrast',
          'default' => '#72516a',
          'base' => 'accent_base',
          'processor' => 'strengthen-80',
        ],
      ],
    ],
  ];

  $color_template = '
    <div class="form-item">
      <span class="form-item__label">Demonstration</span>
      <div
        class="color-example"
        data-cp-background-color="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][background]"
        data-cp-background-strong="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][background_strong]"
        data-cp-background-weak="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][background_weak]"
      >
        <p>
          <span data-cp-color="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][foreground]">Text</span>
          <span data-cp-color="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][foreground_weak]">Alt Text</span>
          <span data-cp-color="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][focus]">Focus</span>
        </p>
        <p>
          <span data-cp-color="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][link]">Link</span>
          <span data-cp-color="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][link_hover]">Hover</span>
          <span data-cp-color="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][link_active]">Active</span>
        </p>
        <p>
          <span data-cp-color="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][accent]">Accent</span>
          <span data-cp-color="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][accent_hover]">Hover</span>
          <span data-cp-color="theme_ui[color_schemes][SCHEME_KEY][PALETTE_KEY][derivatives][accent_active]">Active</span>
        </p>
      </div>
    </div>
  ';

  // Build color palette form
  foreach ($color_schemes as $scheme_key => $scheme) {
    $form['theme_ui']['color_schemes'][$scheme_key] = [
      '#type' => 'details',
      '#title' => t($scheme['label']),
      '#id' => $scheme_key,
      '#open' => $scheme['open'],
      '#attributes' => [
        'class' => ['scheme'],
      ],
    ];

    foreach ($color_palettes as $palette_key => $palette) {
      $id_prefix = $scheme_key === 'default'
        ? $palette_key . '__'
        : $scheme_key . '__' . $palette_key . '__';

      $form['theme_ui']['color_schemes'][$scheme_key][$palette_key] = [
        '#type' => 'details',
        '#title' => t($palette['label']),
        '#id' => $palette_key,
        '#open' => FALSE,
        '#attributes' => [
          'class' => ['palette'],
        ],
      ];

      $form['theme_ui']['color_schemes'][$scheme_key][$palette_key]['is_dark'] = [
        '#type' => 'checkbox',
        '#title' => t('Is dark'),
        '#id' => $id_prefix . 'is_dark',
        '#default_value' => theme_get_setting('theme_ui.color_schemes.' . $scheme_key . '.' . $palette_key . '.is_dark'),
        '#description' => t('Identify whether this color variant is dark, required save before changing the colors'),
        '#prefix' => '<div class="palette__item palette__item--full">',
        '#suffix' => '</div>',
      ];

      $form['theme_ui']['color_schemes'][$scheme_key][$palette_key]['base'] = [
        '#type' => 'container',
        '#attributes' => [
          'class' => ['palette__item'],
        ],
      ];

      foreach ($palette['base'] as $base_color_key => $base_color) {
        $form['theme_ui']['color_schemes'][$scheme_key][$palette_key]['base'][$base_color_key]
          = [
          '#type' => 'color',
          '#title' => t($base_color['label']),
          '#id' => $id_prefix . $base_color_key,
          '#default_value' => theme_get_setting('theme_ui.color_schemes.'
            . $scheme_key . '.' . $palette_key . '.base.' . $base_color_key)
            ?: $base_color['default'],
          '#description' => t('Setting this colour will automatically cascade colour down into the dependent colour.'),
          '#attributes' => [
            'class' => ['color-base'],
            'data-color-key' => $palette_key,
            'data-color-scheme' => $scheme_key,
          ],
        ];
      }

      // Example markup
      $example_markup = str_replace(['SCHEME_KEY', 'PALETTE_KEY'], [
        $scheme_key,
        $palette_key,
      ], $color_template);

      $form['theme_ui']['color_schemes'][$scheme_key][$palette_key]['base']['example'] = [
        '#type' => 'markup',
        '#markup' => $example_markup,
      ];

      $form['theme_ui']['color_schemes'][$scheme_key][$palette_key]['derivatives'] = [
        '#type' => 'container',
        '#attributes' => [
          'class' => ['palette__item'],
        ],
      ];

      foreach ($palette['derivatives'] as $derivative_key => $derivative) {
        $form['theme_ui']['color_schemes'][$scheme_key][$palette_key]['derivatives'][$derivative_key] = [
          '#type' => 'color',
          '#title' => t($derivative['label']),
          '#id' => $id_prefix . $derivative_key,
          '#default_value' => theme_get_setting('theme_ui.color_schemes.'
            . $scheme_key . '.' . $palette_key . '.derivatives.' . $derivative_key)
            ?: $derivative['default'],
          '#attributes' => [
            'class' => ['color', 'color-derivative'],
            'data-color-dep' => 'theme_ui[color_schemes][' . $scheme_key . '][' . $palette_key . '][base][' . $derivative['base'] . ']',
            'data-color-processor' => $derivative['processor'] ?? NULL,
          ],
          // Set empty suffix field to add contrast rating
          '#field_suffix' => t(' '),
        ];

        if (isset($derivative['contrast'])) {
          $form['theme_ui']['color_schemes'][$scheme_key][$palette_key]['derivatives'][$derivative_key]['#attributes']['class'][] = 'color-contrast';
          $form['theme_ui']['color_schemes'][$scheme_key][$palette_key]['derivatives'][$derivative_key]['#attributes']['data-color-contrast']
            = 'theme_ui[color_schemes][' . $scheme_key . '][' . $palette_key . '][derivatives][' . $derivative['contrast'] . ']';
        }
      }
    }
  }

  // Attach libraries
  $form['#attached']['library'][] = 'convivial_bootstrap/theme_ui';
}

/**
 * Attach Theme feature.
 */
function _theme_ui_variables(&$attachments) {
  $settings = theme_get_setting('theme_ui');

  if (empty($settings) || $settings['enable'] != 1) {
    return;
  }

  $styles = '';
  $root_css_variables = '';
  $root_scheme_css_variables = '';
  $palette_css_variables = '';

  // Set radius
  $radius = theme_get_setting('theme_ui.radius');
  foreach ($radius as $key => $value) {
    $formatted_value = $value . 'rem';

    if ($key === 'default') {
      $root_css_variables .= "--radius: $formatted_value;";
    }
    else {
      $root_css_variables .= "--radius-$key: $formatted_value;";
    }
  }

  // Set color schemes
  $color_schemes = theme_get_setting('theme_ui.color_schemes');
  foreach ($color_schemes as $color_scheme_key => $color_scheme) {
    foreach ($color_scheme as $palette_key => $palette) {
      $temp_root_scheme_css_variables = '';
      $temp_palette_css_variables = '';
      $formatted_palette_key = str_replace("_", "-", $palette_key);
      $palette_prefix = $color_scheme_key === 'default'
        ? ".cp--$formatted_palette_key { "
        : "[data-theme=\"$color_scheme_key\"] .cp--$formatted_palette_key { ";
      $palette_suffix = " }";

      if ($color_scheme_key === 'default') {
        $palette_prefix = ".cp--$formatted_palette_key { ";
      }

      foreach ($palette['derivatives'] as $derivative_key => $derivative) {
        $color_key = str_replace("_", "-", $derivative_key);

        // Set root variables
        if ($palette_key === 'standard') {
          if ($color_scheme_key === 'default') {
            $root_css_variables .= "--$color_key: $derivative;";
          }
          else {
            $temp_root_scheme_css_variables .= "--$color_key: $derivative;";
          }
        }

        // Set remaining variables
        $temp_palette_css_variables .= "--$color_key: $derivative;";
      }

      // Set root variables with scheme
      if ($color_scheme_key !== 'default' && $palette_key === 'standard') {
        $root_scheme_css_variables .= "[data-theme=\"$color_scheme_key\"] { $temp_root_scheme_css_variables }";
      }

      // Set remaining variables with scheme
      $palette_css_variables .= $palette_prefix
        . $temp_palette_css_variables
        . $palette_suffix;
    }
  }

  // Attached CSS variables
  if (!empty($root_css_variables)) {
    $styles .= ":root { $root_css_variables }";
  }

  if (!empty($root_scheme_css_variables)) {
    $styles .= $root_scheme_css_variables;
  }

  if (!empty($palette_css_variables)) {
    $styles .= $palette_css_variables;
  }

  if (!empty($styles)) {
    $attachments['#attached']['html_head'][] = [
      [
        '#tag' => 'style',
        '#attributes' => ['type' => 'text/css'],
        '#value' => $styles,
      ],
      'theme-ui',
      ['weight' => 100],
    ];
  }
}
