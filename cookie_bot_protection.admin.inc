<?php

/**
 * @file
 */

/**
 * Settings form
 */
function cookie_bot_protection_settings_form($form, &$form_state) {
  $config = config('cookie_bot_protection.settings');

  $form['#config'] = 'cookie_bot_protection.settings';

  $form['url_protected_patterns'] = [
    '#type' => 'textarea',
    '#title' => t('Protected URL patterns (one per line, PCRE)'),
    '#description' => t('Only requests whose path matches one of these <a href="@pcre">regular expressions</a> will be challenged. E.g. <code>^protected</code> for paths like <code>protected/page</code>.', ['@pcre' => 'https://en.wikipedia.org/wiki/Perl_Compatible_Regular_Expressions']),
    '#default_value' => implode(PHP_EOL, $config->get('url_protected_patterns')),
  ];
  $form['ua_whitelist_patterns'] = [
    '#type' => 'textarea',
    '#title' => t('User-Agent whitelist patterns'),
    '#description' => t('<a href="@ua">User-Agent</a> patterns which should be whitelisted. E.g. <code>Googlebot</code>.', ['@ua' => 'https://en.wikipedia.org/wiki/User-Agent_header']),
    '#default_value' => implode(PHP_EOL, $config->get('ua_whitelist_patterns')),
  ];
  $form['ip_whitelist'] = [
    '#type' => 'textarea',
    '#title' => t('IP whitelist (CIDR, one per line)'),
    '#description' => t('<a href="@cidr">CIDR notation</a> is a compact representation of an IP address and its associated subnet mask. It is useful for specifying either a specific address or the beginning address of a network. E.g. IPv4 block <code>198.51.100.0/22</code> represents the 1024 IPv4 addresses from <code>198.51.100.0</code> to <code>198.51.103.255</code>.', ['@cidr' => 'https://en.wikipedia.org/wiki/Classless_Inter-Domain_Routing']),
    '#default_value' => implode(PHP_EOL, $config->get('ip_whitelist')),
  ];
  $form['header_response_debug'] = [
    '#type' => 'checkbox',
    '#title' => t('Add debug headers to the response'),
    '#default_value' => $config->get('header_response_debug'),
  ];
  $form['redirect_error_delay'] = [
    '#type' => 'number',
    '#title' => t('Redirect error delay (seconds)'),
    '#default_value' => $config->get('redirect_error_delay'),
    '#description' => t('If > 0, a 401 page will automatically refresh after this many seconds.'),
  ];

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  ];

  return $form;
}

/**
 * Validation handler
 *
 * @see cookie_bot_protection_settings_form().
 */
function cookie_bot_protection_settings_form_validate(&$form, $form_state) {
    foreach (['url_protected_patterns', 'ua_whitelist_patterns'] as $field) {
      foreach (cookie_bot_protection_explode_config_value($form_state['values'][$field]) as $pattern) {
        $normalized = preg_replace('/\\\\u([a-fA-F0-9]{4})/', '\\x{\\1}', $pattern);
        set_error_handler(static fn(int $errno, string $errstr): bool => TRUE);
        $valid = preg_match('{' . $normalized . '}u', '') !== FALSE;
        restore_error_handler();
        if (!$valid) {
          form_set_error($field, t('Invalid regular expression in field %field: %pattern', [
            '%field' => $form[$field]['#title'],
            '%pattern' => $pattern,
          ]));
          break;
        }
      }
    }
  }

/**
 * Submit handler
 *
 * @see cookie_bot_protection_settings_form().
 */
function cookie_bot_protection_settings_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $values = $form_state['values'];
  $config = config('cookie_bot_protection.settings');

  $config
    ->set('url_protected_patterns', cookie_bot_protection_explode_config_value($values['url_protected_patterns']))
    ->set('ua_whitelist_patterns', cookie_bot_protection_explode_config_value($values['ua_whitelist_patterns']))
    ->set('ip_whitelist', cookie_bot_protection_explode_config_value($values['ip_whitelist']))
    ->set('header_response_debug', (bool) $form_state['values']['header_response_debug'])
    ->set('redirect_error_delay', (int) $form_state['values']['redirect_error_delay'])
    ->save();

  backdrop_set_message(t('Configuration saved.'));
}

/**
 * Convert a string form value to an array config value.
 *
 * @param string $setting
 *   String form value.
 *
 * @return string[]
 *   Converted array config.
 */
function cookie_bot_protection_explode_config_value(string $setting): array {
  $value = array_map(trim(...), explode(PHP_EOL, $setting));
  return array_filter($value);
}