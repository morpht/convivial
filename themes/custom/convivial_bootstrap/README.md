# Convivial Boostrap

Convivial theme based on Bootstrap 5.

## Local setup

1. Run command `npm run setup`.
2. Copy and rename `default.config-local.json` to `config-local.json`.

## Production setup

* run command `npm run gulp`

**This will produce:**
* compressed CSS output
* optimized Image assets
* minified JS

## Development setup

* run command `npm run gulp dev`

**This will produce:**
* nested CSS output
* un-minified JS
* source maps for both CSS and JS
* watch task for changes in SCSS and JS files
* BrowserSync links

### Stylelint

* run command `npm run stylelint` to check errors in stylesheets in components folder.
* run command `npm run stylelint-fix` to fix errors in stylesheets in components folder.

Note that some errors needs to be fixed manually.

## Look and Feel

**Settings page:** "/admin/appearance/settings/convivial_bootstrap"

This behavior is attached only if "Enable Look and Feel" checkbox is checked.

### Field Selectors

A list of rows with a pipe-separated mappings of the fields to selectors.

### Strategies

A list of rows with a pipe-separated list of the strategies and parameters.

* **"entity|node,taxonomy_term"** - Get look from current full page entity
of specified types.

* **"reference|field_section"** - Get look from the entity reference field
"field_section" attached on the current full page entity.

* **"path|/section/\*,taxonomy_term,section-a"** - If current URL matches
"/section/*", search specific look with key "section-a" between all taxonomy
term entities.

* **"apply|taxonomy_term,section-a"** - Search specific look with key
"section-a" between all taxonomy term entities.

**Completes** - Only first matching look will be applied.

**Partials** - All matching looks will be applied.

## Containment options for layouts

Following containment options can be found on One column layout, Two column layout, Three column layout and Four column layout.

* **Edgy with container** - Blocks and Paragraphs will be always inside the container but you can apply background modifier to the layout section so you can do some nice design.


* **Edgy** - Blocks will be always edgy. Paragraphs wrapper will be edgy but the content is still in the container. You can break the container by enabling "breakout" class.

* **Container** - Blocks and Paragraphs will be always inside the container and also background will be in the container.

* **Fluid (padding on both sides)** - Similar to Edgy but it has padding from both sides.

## Style classes

* **Breakout** - Usually paragraphs components on edgy pages has container. If there is a special design which needs to have content expanded across whole screen width, use this class.

* **Hero** - Make a paragraph or block component bigger by adding some extra padding to the top and bottom.

* **No background padding** - This class on the other side removes all padding in the component. Even if the colour palette is applied.

* **No column gaps** - This style removes padding between list/section item's columns. (It is recommended to use this class in combination with **No row gaps** and **No side gaps**.)

* **No row gaps** - This style removes margin between list/section item's rows. (It is recommended to use this class in combination with **No row gaps** and **No side gaps**.)

* **No side gaps** - This style removes padding from each side of the component when you want to achieve true edge to edge layout. (It is recommended to use this class in combination with **No row gaps** and **No side gaps**.)

* **Text left** - Align text to the left.

* **Text center** - Align text to the center.

* **Text right** - Align text to the right.

* **Text shadow** - Add the shadow to the text.

* **Transparent card** - Make a image card and basic card slightly transparent.

## View classes for various layouts

General CSS class which needs to be placed onto the view in "advanced" section is `view-layout`.
For multi-column layout also add `view-layout--row` class.

Then in format settings you need to add `views-layout__item`.

Layout variants:

* `views-layout--full-width` - On column layout.
* `views-layout--halves` - Two column layout.
* `views-layout--thirds` - Three column layout.
* `views-layout--quarters` - Foue column layout.
* `views-layout--alternate` - This is mainly for Teaser view modes. Allows to alternate order of image and content.

*Note: If you want to use Title view mode, you should also add class `views-layout--title` which removes the spaceing between each item.*

## Meta

**Settings page:** "/admin/appearance/settings/convivial_bootstrap"

This behavior is attached only if "Enable Meta" checkbox is checked.

The Meta feature allows inserting of custom HTML meta tags into page markup with
content based on the current page entity values (only node entity is supported).

**Every tag has:**

1) One of the base attributes ("name" or "property") with value containing a
   specified key.
2) Attribute "content" with value fetched from the current entity based on a
   specified strategy (e.g. using a token).
3) Could be extended by other specified attributes and values.
4) Could be limited only for a specified entity bundles.

**Example of a full configuration input:**

* `token,topic,[node:field_topic],attr1=x|attr2=y,page|article`

**Example of output for "page" and "article" nodes only:**

* `<meta name="topic" content="Drupal" attr1="x" attr2="y" />`

### Meta strategies

Every row is a comma-separated mapping of the strategy name, value key, optional
pipe-separated parameters, optional pipe-separated attributes, and optional
pipe-separated bundle limitation.

* Format: **"strategy,key,param1|param2,attr1=x|attr2=y,bundle1|bundle2"**

#### These strategies are supported:

* **"constant,site,alpha"** - For a "site" key, set the specific value "alpha".

* **"bundle,type"** - For a "type" key, set the current full-page entity bundle.

* **"values,summary,field_summary"** - For a "summary" key, set the
  "field_summary" field value from the current full-page entity.

  * Use **"value, ..."** for a single string value instead of an array.

* **"references,topic,field_topics|field_identifier"** - For a "topic" key, set
  the "field_identifier" field value from an entity referenced by "field_topics"
  of the current full-page entity.

  * Use **"reference, ..."** for a single string value instead of an array.

* **"token,type,[node:content-type]"** - For a "type" key, set the current
  full-page entity value rendered using a specified token.

## Licence

Copyright © 2017-2020 Morpht Pty Ltd
