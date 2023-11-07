# Convivial profile CXP

[![Continous Integration](https://github.com/morpht/convivial/actions/workflows/build.yml/badge.svg)](https://github.com/morpht/convivial/actions?query=branch%3Amain)

This is a Drupal installation profile that encapsulates our starting site configuration and basic scaffolding
like settings.php and Lando tooling.

## Structure

* `convivial profile`
    * **Drupal installation profile. Holds config, default content, scaffolding files like settings.php and Lando tooling.**
    * All changes here will propagate to active projects.
* `convivial-recommended`
    * **Very basic file structure that requires `convivial profile` in composer. Used as a starting point for new projects.**
    * "Use it and forget it."
* `[new-project]`
    * **New git repo based on `convivial-recommended`. Holds project specific things. Requires `convivial profile` in composer.**
    * New project code is hosted on GitHub and GitHub Actions replicate it to Pantheon.
    * `composer update` fetches latest changes form `convivial profile`.

## Starting a new site - overview

* Decide on new machine name. Make sure it's available on Pantheon.
* `Use this template` on https://github.com/morpht/convivial-recommended
    * Template provides basic file structure and pulls in the `convivial profile`.
    * Rename all mentions of `convivial-recommended` to new project name.

This profile is meant to be used in conjunction with `convivial-recommended` project template. To start a new project, you should:

* Create a new github repo from convivial-recommended as a template.
* Checkout new repo locally and edit .lando.yml (project name, local url).
* Run `lando composer install`
* Install site locally with Convivial installation profile.
* Create Pantheon Drupal 10 site from link here https://docs.pantheon.io/drupal-10.
* Edit drush/sites/pantheon/{project}.site.yml to add there Pantheon site hash instead of %hash%
* Edit repository secrets at github to add there Pantheon git url.
* Upload database from local to pantheon.
* Force push your repository to pantheon git.
* After first push, dev site at Pantheon should be functional.

## Starting a new site - Technical details

* Use the `convivial-recommended` starter kit to spin up a new website.


## Updating

To update configuration, one needs to manually copy new/updated configuration files to convivial/config/install folder and remove uuid and _core: default_hash items there.

[How to write an installation profile](https://www.drupal.org/docs/distributions/creating-distributions/how-to-write-a-drupal-installation-profile)
