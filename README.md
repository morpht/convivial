# Convivial profile CXP

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




## Updating

To update configuration, one needs to manually copy new/updated configuration files to convivial/config/install folder and remove uuid and _core: default_hash items there.

[How to write an installation profile](https://www.drupal.org/docs/distributions/creating-distributions/how-to-write-a-drupal-installation-profile)

## Workflow

### Repos

* [Convivial Demo](https://github.com/morpht/convivial-demo) repo is the source of Truth.
* [Convivial Profile](https://github.com/morpht/convivial) is a Public/opensource Profile in general, but it is a custom profile in convivial demo project.
* [Convivial Bootstrap](https://github.com/morpht/convivial_bootstrap) theme is a Public/Opensource theme in general, but it is a custom theme in convivial demo project.
* [Convivial Claro](https://github.com/morpht/convivial_claro) theme is a Public/Opensource theme in general, but it is a custom theme in convivial demo project.
* We have branching rule setup on convivial-demo to make the changes to *master* branch via PRs only. PLEASE DO NO TRY TO BYPASS IT.

### Rules

* All the changes requests should be against the [morpht/convivial-demo](https://github.com/morpht/convivial-demo).
* Github Actions automatically create the PR against public convivial, public convivial_bootstrap and public convivial_claro repo if you make any changes into any of them in the convivial-demo project
* The ticket should have the mention of all the PRs(including auto-opened PRs).
* On PR Approval, First merge the auto-opened PRs. Then, merge the convivial-demo PR
* Do not make direct changes to master branch or via manual PRs against these Repos [morpht/convivial](https://github.com/morpht/convivial), [morpht/convivial_bootstrap](https://github.com/morpht/convivial_bootstrap), and [morpht/convivial_claro](https://github.com/morpht/convivial_claro)

