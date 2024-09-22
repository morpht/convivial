<?php

namespace Drupal\Tests\dtt\ExistingSite;

use weitzman\DrupalTestTraits\ExistingSiteBase;

/**
 * Tests config values that are set at install.
 *
 * @group dtt
 * @group profile
 * @group risky
 */
class InstallStateTest extends ExistingSiteBase {

  /**
   * {@inheritdoc}
   */
  protected function setUp(): void {
    parent::setUp();
    // Update configuration so that password policy can be tested by
    // registering an account through UI.
    $this->container->get('config.factory')
      ->getEditable('user.settings')
      ->set('verify_mail', FALSE)
      ->set('register', 'visitors_admin_approval')
      ->save();
  }

  /**
   * Tests that key administrative pages are available.
   *
   * Acquia CMS is a big and complicated system, and it is possible that simple
   * dependency updates can produce WSODs in key administrative places. To
   * detect that kind of thing, this method logs in as an administrator, visits
   * a bunch of those key administrative pages, and verifies that they produce
   * a 200 status code. That doesn't mean they work as intended, of course, but
   * at least they are not producing scary blank (or error) screens.
   */
  public function testKeyAdministrativePages(): void {
    $rid = $this->createAdminRole();
    $account = $this->createUser();
    $account->addRole($rid);
    $account->save();
    $this->drupalLogin($account);

    $pages_to_check = [
      '/admin/content',
      '/admin/structure/block',
      '/admin/people',
    ];
    $session = $this->getSession();
    foreach ($pages_to_check as $path) {
      $this->drupalGet($path);
      $status_code = $session->getStatusCode();
      $this->assertSame(200, $status_code, "$path returned status code $status_code.");
    }
  }

  /**
   * {@inheritdoc}
   */
  public function tearDown(): void {
    // Delete user created during testing password policy.
    $user = user_load_by_mail('example@example.com');
    if ($user) {
      $user->delete();
    }
    // Revert the configuration back to default, once password policy is tested.
    $this->container->get('config.factory')
      ->getEditable('user.settings')
      ->set('verify_mail', TRUE)
      ->set('register', 'visitors')
      ->save();
    parent::tearDown();
  }

}
