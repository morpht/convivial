import '../../support/commands';

let uninstalled_modules = [];

/*
 * Creating Page workflow testing
 */
describe('Validate workflow for Page content type', () => {
  before(() => {
    // Clear Drupal cache
    cy.drush('cr');

    // Clear all sessions saved on the backend, including cached global sessions
    Cypress.session.clearAllSavedSessions();

    // Unblock all users.
    cy.unblockUserByUsername('editor');
    cy.unblockUserByUsername('approver');
    cy.unblockUserByUsername('convivial-demo');

    cy.session(['site_administrator'], () => {
      cy.drupalLogin('convivial-demo');
    });

    //TODO resolve recombee and antibot issues
    // Remove recombee module if enabled
    if (cy.isModuleEnabled('recombee').result) {
      cy.drush('pm:uninstall convivial_enricher_recombee search_api_recombee recombee');
      uninstalled_modules.push('convivial_enricher_recombee');
      uninstalled_modules.push('search_api_recombee');
      uninstalled_modules.push('recombee');
    }

    cy.drush('role:perm:add', ['anonymous', '"skip antibot"']);

    if (cy.isModuleEnabled('antibot').result) {
      // cy.drush('pm:uninstall antibot');
      cy.drush('role:perm:add', ['anonymous', '"skip antibot"']);
      // uninstalled_modules.push('antibot');
    }

    // Delete testing pages if they exist
    cy.visitAndDeletePage('/testing-page');
    cy.visitAndDeletePage('/testing-page-draft');
  });

  after(() => {
    cy.session(['site_administrator'], () => {
      cy.drupalLogin('convivial-demo');
    });

    // Delete testing pages if they exist
    cy.visitAndDeletePage('/testing-page');
    cy.visitAndDeletePage('/testing-page-draft');

    // Enable uninstalled modules
    // uninstalled_modules.forEach((module) => {
    //   cy.drush(`pm:enable ${module}`);
    // });

    cy.drush('role:perm:remove', ['anonymous', '"skip antibot"']);

    // Block all users
    cy.blockUserByUsername('editor');
    cy.blockUserByUsername('approver');
    cy.blockUserByUsername('convivial-demo');

    // Clear all sessions saved on the backend, including cached global sessions
    Cypress.session.clearAllSavedSessions();
  });

  it('Creating Page as Editor', () => {
    // Anonymous user should get 404 on /testing-page
    cy.request({
      url: '/testing-page',
      failOnStatusCode: false,
    }).its('status').should('equal', 404);

    // Login as 'editor', and preserve the session
    cy.session(['editor'], () => {
      cy.drupalLogin('editor');
    });

    // Visit /node/add/pagee
    cy.visit('/node/add/page');

    // Fill title with 'Testing page'
    cy.get('#edit-title-0-value').type('Testing page');

    // Fill summary field with 'Testing page summary.'
    cy.get('#edit-field-summary-0-value').type('Testing page summary.');

    cy.window().then(win => {
      win.Drupal.CKEditor5Instances.forEach(editor => {
        // Get the id of the DOM element that the editor is attached to
        const editorId = editor.sourceElement.id;

        // Set the editor data based on the editor id
        switch (editorId) {
          case 'edit-field-body-0-value':
            // Fill body field with 'Testing page body.'
            editor.setData('<p>Testing page body.</p>');
            break;
        }
      });
    });

    // You should see options to Save as 'Draft' or 'Needs reviewed' in the 'Save as' dropdown
    cy.get('#edit-moderation-state-0-state').within(() => {
      cy.get('option[value="draft"]').should('exist');
      cy.get('option[value="needs_review"]').should('exist');
    });

    // Click on the Save button to save as Draft
    cy.get('#edit-submit').click();

    // Use anonymous and preserve session
    cy.session(['anonymous'], () => {
      cy.drupalLogout();
    });

    // Anonymous user should not see the content to be appearing on /testing-page
    cy.request({
      url: '/testing-page',
      failOnStatusCode: false,
    }).its('status').should('equal', 403);

    cy.session(['anonymous'], () => {
      cy.drupalLogout();
    });
    // Validate that page is not in search results
    cy.visit('/search?keys=testing+page');
    cy.get('.view-content-search').get('.search__title a').contains('Testing page').should('not.exist');
  });

  it('Changing from Draft to Published Page as Approver', () => {
    // Login to the site as Approver and keep the session
    cy.session(['approver'], () => {
      cy.drupalLogin('approver');
    });

    cy.visit('/testing-page');
    cy.get('.nav-link').contains('Edit').click();

    cy.timeout(1000);

    cy.get('#edit-moderation-state-0-state').select('published');

    // Assert that the selected option matches the expected value
    cy.get('#edit-moderation-state-0-state').within(() => {
      cy.get('option').should('have.length', 1);
      cy.get('option[value="published"]').should('exist');
    });

    cy.get('#edit-submit').click();

    // Block Recombee request
    cy.intercept('https://client-rapi.recombee.com/*', {});
    // I should see the content to be appearing on /testing-page
    cy.visit('/testing-page');

    // Anonymous user should see the content on /testing-page
    // Use anonymous and preserve session
    cy.session(['anonymous'], () => {
      cy.drupalLogout();
    });

    cy.visit('/testing-page');

    // I should see the title match to 'Testing page'
    cy.get('h1').should('contain', 'Testing page');
    // I should see the body match to 'Testing page body.'
    cy.get('.field--name-field-body').should('contain', 'Testing page body.');

    // Validate meta values
    cy.get('head title').should('contain', 'Testing page | Convivial Demo');
    // Get the meta description tag and assert its content
    cy.get('head meta[name="description"]').should('have.attr', 'content', 'Testing page summary.');

    cy.session(['anonymous'], () => {
      cy.drupalLogout();
    });
    // Validate that testing page is in search results
    cy.visit('/search?keys=testing+page');
    cy.get('.view-content-search').get('.search__title a').contains('Testing page').should('exist');
  });

  it('Changing Published to Draft as Editor (Creating new draft of the Page after the Page was published)', () => {
    // Login to the site as Editor and keep the session
    cy.session(['editor'], () => {
      cy.drupalLogin('editor');
    });

    // Edit Page created in scenario 1

    cy.intercept('https://client-rapi.recombee.com/*', {});
    cy.visit('/testing-page');
    cy.get('.nav-link').contains('Edit').click();
    // Change title with 'Testing page draft'
    cy.get('#edit-title-0-value').type(' draft');
    // I should have ability to select only Draft in the status dropdown
    cy.get('#edit-moderation-state-0-state').within(() => {
      cy.get('option').should('have.length', 1);
      cy.get('option[value="draft"]').should('exist');
    });
    // Change state to Draft
    cy.get('#edit-moderation-state-0-state').select('draft');
    // Save Page
    cy.get('#edit-submit').click();

    // I should see the old title (Testing page) as anonymous on /testing-page
    cy.session(['anonymous'], () => {
      cy.drupalLogout();
    });

    cy.visit('/testing-page');
    cy.get('h1').should('not.contain', 'Testing page draft');
    cy.get('h1').should('contain', 'Testing page');
    // Validate that page is in search results
    cy.visit('/search?keys=testing+page');
    cy.get('.view-content-search').get('.search__title a').contains('Testing page').should('exist');
  });

  it('Changing Page from Draft to Needs Review as Editor', () => {
    // Login to the site as Approver
    cy.session(['approver'], () => {
      cy.drupalLogin('approver');
    });
    // Edit Page
    cy.visit('/testing-page');
    cy.get('.nav-link').contains('Edit').click();
    // Approver can see in change to dropdown only published option
    cy.get('#edit-moderation-state-0-state').within(() => {
      cy.get('option').should('have.length', 1);
      cy.get('option[value="published"]').should('exist');
    });

    // Login to the site as Editor
    cy.session(['editor'], () => {
      cy.drupalLogin('editor');
    });
    // Edit Page
    cy.visit('/testing-page');
    cy.get('.nav-link').contains('Edit').click();
    // Change state to Needs Review
    cy.get('#edit-moderation-state-0-state').select('needs_review');
    // Save Page
    cy.get('#edit-submit').click();
    // Editor should see Page here /testing-page
    cy.visit('/testing-page');
    // Editor cannot now edit Page
    cy.get('.nav-link').should('not.contain', 'Edit');

    // Approver can now edit Page
    cy.session(['approver'], () => {
      cy.drupalLogin('approver');
    });
    cy.visit('/testing-page');
    cy.get('.nav-link').should('contain', 'Edit');
    // Validate that page is in search results
    cy.visit('/search?keys=testing+page');
    cy.get('.view-content-search').get('.search__title a').contains('Testing page').should('exist');
  });

  it('Changing Page from Needs Review to Draft as Approver', () => {
    // Editor should not be able to edit content
    cy.session(['editor'], () => {
      cy.drupalLogin('editor');
    });
    cy.visit('/testing-page');
    cy.get('.nav-link').should('not.contain', 'Edit');
    // Login to the site as Approver
    cy.session(['approver'], () => {
      cy.drupalLogin('approver');
    });
    // Edit Page
    cy.visit('/testing-page');
    cy.get('.nav-link').contains('Edit').click();

    // Change state to Draft
    cy.get('#edit-moderation-state-0-state').select('draft');
    // Save Page
    cy.get('#edit-submit').click();
    // Anonymous user should not see the content to be appearing on /testing-page-draft
    cy.session(['anonymous'], () => {
      cy.drupalLogout();
    });

    cy.request({
      url: '/testing-page-draft',
      failOnStatusCode: false,
    }).its('status').should('equal', 404);
    // Validate that page draft is not in search results
    cy.visit('/search?keys=testing+page');
    cy.get('.view-content-search').get('.search__title a').should('not.contain', 'Testing page draft');
    cy.get('.view-content-search').get('.search__title a').should('contain', 'Testing page');
  });

  it('Archiving Page as Approver', () => {
    // Login to the site as Approver
    cy.session(['approver'], () => {
      cy.drupalLogin('approver');
    });
    // Edit Page
    cy.visit('/testing-page');
    cy.get('.nav-link').contains('Edit').click();
    // Change state to Published
    cy.get('#edit-moderation-state-0-state').select('published');
    // Save Page
    cy.get('#edit-submit').click();
    // Anonymous user should see content on url /testing-page-draft
    cy.session(['anonymous'], () => {
      cy.drupalLogout();
    });

    cy.visit('/testing-page-draft');
    // Validate that page is in search results
    cy.visit('/search?keys=testing+page');
    cy.get('.view-content-search').get('.search__title a').should('contain', 'Testing page draft');

    // Change state to Archive
    cy.session(['approver'], () => {
      cy.drupalLogin('approver');
    });
    cy.visit('/testing-page-draft');
    cy.get('.nav-link').contains('Edit').click();
    cy.get('#edit-moderation-state-0-state').select('archived');
    // Save Page
    cy.get('#edit-submit').click();
    // Anonymous user should not see content on url /testing-page-draft
    cy.session(['anonymous'], () => {
      cy.drupalLogout();
    });

    // Validate that page is not in search results anymore
    cy.visit('/search?keys=testing+page');
    cy.get('.view-content-search').get('.search__title a').contains('Testing page').should('not.exist');
    cy.get('.view-content-search').get('.search__title a').contains('Testing page draft').should('not.exist');
  });

  it('Changing Page from Archived to Draft Page as Approver', () => {
    // Editor should not be able to edit node in this state
    cy.session(['editor'], () => {
      cy.drupalLogin('editor');
    });

    cy.visit('/testing-page-draft');
    cy.get('.nav-link').should('not.contain', 'Edit');
    // Login to the site as Approver
    cy.session(['approver'], () => {
      cy.drupalLogin('approver');
    });
    // Edit Page
    cy.visit('/testing-page-draft');
    cy.get('.nav-link').contains('Edit').click();
    // Approver should see Draft and Published in 'Change to:' dropdown
    cy.get('#edit-moderation-state-0-state').within(() => {
      cy.get('option[value="draft"]').should('exist');
      cy.get('option[value="published"]').should('exist');
    });
    // Change state to Draft
    cy.get('#edit-moderation-state-0-state').select('draft');
    // Save Page
    cy.get('#edit-submit').click();
    // Validate that page is not search results
    cy.visit('/search?keys=testing+page');
    cy.get('.view-content-search').get('.search__title a').should('not.have.text', 'Testing page');
    cy.get('.view-content-search').get('.search__title a').should('not.have.text', 'Testing page draft');

    // Anonymous user should not see content on url /testing-page-draft
    cy.session(['anonymous'], () => {
      cy.drupalLogout();
    });

    cy.request({
      url: '/testing-page-draft',
      failOnStatusCode: false,
    }).its('status').should('equal', 403);

    // Login as Approver
    cy.session(['approver'], () => {
      cy.drupalLogin('approver');
    });

    // Edit Page
    cy.visit('/testing-page-draft');
    cy.get('.nav-link').contains('Edit').click();
    // Change state to Published
    cy.get('#edit-moderation-state-0-state').select('published');
    // Save Page
    cy.get('#edit-submit').click();
    // Validate that page is in search results
    cy.visit('/search?keys=testing+page');
    cy.get('.view-content-search').get('.search__title a').contains('Testing page draft').should('exist');
    // Anonymous user should see content on url /testing-page-draft
    cy.session(['anonymous'], () => {
      cy.drupalLogout();
    });

    cy.visit('/testing-page-draft');
  });
});
