<?php

/**
 * @file
 * Import 301 redirects.
 */

use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\redirect\Entity\Redirect;

/**
 * Import Redirects.
 */
function _convivial_bootstrap_import_redirects(FormStateInterface $form_state) {

  // Parse CSV string by rows into array.
  $csv_lines = explode("\n", $form_state->getValue('csv'));

  // Record of failed imports.
  $imports_failed = [];

  // Record of successful imports.
  $imports_successed = [];

  // Process row by row.
  foreach ($csv_lines as $key => $line) {
    $line = trim($line);

    // Parse CSV line string into array.
    $row = str_getcsv($line);

    // Clean values.
    $source = trim($row[0]);
    $from = ltrim($source, '/');

    $to = trim($row[1]);

    $redirect = Redirect::create();
    $redirect->setSource($from);
    $redirect->setRedirect($to);
    $redirect->setStatusCode(301);
    try {
      $redirect->save();
      $imports_successed[$redirect->id()] = $line;
    }
    catch (Exception $e) {
      $imports_failed[] = $line;
    }

  }

  // Report - Summary.
  \Drupal::messenger()->addStatus(
    t('@success redirect(s) were imported, @failed failed.', [
      '@success' => count($imports_successed),
      '@failed' => count($imports_failed),
    ])
  );

  // Report - Success.
  if (count($imports_successed)) {
    $message_success = new TranslatableMarkup('<strong>@title</strong><br /><pre>@items</pre>', [
      '@title' => 'Imported items',
      '@items' => print_r($imports_successed, TRUE),
    ]);
    \Drupal::messenger()->addStatus($message_success);
  }

  // Report - Failed.
  if (count($imports_failed)) {
    $message_failed = new TranslatableMarkup('<strong>@title</strong><br /><pre>@items</pre>', [
      '@title' => 'Failed items',
      '@items' => print_r($imports_failed, TRUE),
    ]);
    \Drupal::messenger()->addStatus($message_failed);
  }

  // Set CSV to empty string.
  $form_state->setValue('csv', '');

}
