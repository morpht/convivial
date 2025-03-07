import '../../support/commands';

let uninstalled_modules = [];

describe('Validate content URLS.', () => {
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

    // TODO: Disable this once the issue with the module is fixed
    cy.log(cy.isModuleEnabled('recombee').result);
    if (cy.isModuleEnabled('recombee').result) {
      cy.drush('pm:uninstall convivial_enricher_recombee');
      cy.drush('pm:uninstall search_api_recombee');
      cy.drush('pm:uninstall recombee');
      uninstalled_modules.push('recombee');
    }

    cy.drush('role:perm:add', ['anonymous', '"skip antibot"']);

    // Unblock all users
    cy.unblockUserByUsername('editor');
    cy.unblockUserByUsername('approver');
    cy.unblockUserByUsername('convivial-demo');

    cy.session(['site_administrator'], () => {
      cy.drupalLogin('convivial-demo');
    });

    cy.fixture('contentUrls.json').then((contentUrls) => {
      for (let key in contentUrls) {
        let url = contentUrls[key];

        cy.request({
          url: url,
          failOnStatusCode: false
        }).then((response) => {
          // Check if the page exists (status code 2xx or 3xx)
          if (response.status >= 200 && response.status < 400) {
            cy.visit(url);
            cy.get('.nav-link').contains('Delete').click();
            cy.wait(2000); // Use cy.wait instead of cy.timeout
            cy.get('#edit-submit').click();
          } else {
            // Page doesn't exist, log a message
            cy.log(`Page ${url} does not exist.`);
          }
        });
      }
    });
  });

  after(() => {
    cy.session(['site_administrator'], () => {
      cy.drupalLogin('convivial-demo');
    });

    cy.fixture('contentUrls.json').then((contentUrls) => {
      for (let key in contentUrls) {
        let url = contentUrls[key];

        cy.request({
          url: url,
          failOnStatusCode: false
        }).then((response) => {
          // Check if the page exists (status code 2xx or 3xx)
          if (response.status >= 200 && response.status < 400) {
            cy.visit(url);
            cy.get('.nav-link').contains('Delete').click();
            cy.wait(2000); // Use cy.wait instead of cy.timeout
            cy.get('#edit-submit').click();
          } else {
            // Page doesn't exist, log a message
            cy.log(`Page ${url} does not exist.`);
          }
        });
      }
    });

    cy.drush('role:perm:remove', ['anonymous', '"skip antibot"']);

    // Block all users
    cy.blockUserByUsername('editor');
    cy.blockUserByUsername('approver');
    cy.blockUserByUsername('convivial-demo');

    if (uninstalled_modules.includes("recombee")) {
      cy.drush('pm:install recombee');
      cy.drush('pm:install search_api_recombee');
      cy.drush('pm:install convivial_enricher_recombee');
    }

    // Clear all sessions saved on the backend, including cached global sessions
    Cypress.session.clearAllSavedSessions();
  });

  it('Validate URL for Article', () => {
    // Login to the site as Content Author
    cy.session(['editor'], () => {
      cy.drupalLogin('editor');
    });
    //TODO temporary use site admin until workflow is fixed
    // cy.session(['site_administrator'], () => {
    //   cy.drupalLogin('convivial-demo');
    // });

    // Visit /node/add/article
    cy.visit('/node/add/article');
    // Fill title with 'Testing article'
    cy.get('#edit-title-0-value').type('Testing article');
    // Fill summary field with 'Test article summary.'
    cy.get('#edit-field-summary-0-value').type('Testing article summary.');

    // Save Node
    cy.get('#edit-submit').click();
    // Validate URL to be /news/testing-article
    cy.visit('/news/testing-article');
    cy.get('h1').contains('Testing article').should('exist');
    // Validate meta values
    cy.get('head title').should('contain', 'Testing article | Convivial Demo');
    // Get the meta description tag and assert its content
    cy.get('head meta[name="description"]').should('have.attr', 'content', 'Testing article summary.');
  });

  // Audience does not need to be validated yet - no URL alias pattern

  // it('Validate URL Audience', () => {
  //   // Login to the site as Content Author
  //   cy.session(['editor'], () => {
  //     cy.drupalLogin('editor');
  //   });
  //   //TODO temporary use site admin until workflow is fixed
  //   // cy.session(['site_administrator'], () => {
  //   //   cy.drupalLogin('convivial-demo');
  //   // });
  //
  //   // Visit /node/add/audience
  //   cy.visit('/node/add/audience');
  //   // Fill title with 'Testing audience'
  //   cy.get('#edit-title-0-value').type('Testing audience');
  //
  //   // Fill summary field with 'Testing summary.'
  //   cy.get('#edit-field-summary-0-value').type('Testing summary.');
  //
  //   // Save Node
  //   cy.get('#edit-submit').click();
  //   // Validate URL to be /testing-audience
  //   cy.visit('/testing-audience');
  //   cy.get('h1').contains('Testing audience').should('exist');
  //
  //   // Validate meta values
  //   cy.get('head title').should('contain', 'Testing audience | Convivial Demo');
  //   // Get the meta description tag and assert its content
  //   cy.get('head meta[name="description"]').should('have.attr', 'content', 'Testing summary.');
  //
  // });

  it('Validate URL Event', () => {
    // Login to the site as Content Author
    cy.session(['editor'], () => {
      cy.drupalLogin('editor');
    });
    //TODO temporary use site admin until workflow is fixed
    // cy.session(['site_administrator'], () => {
    //   cy.drupalLogin('convivial-demo');
    // });

    // Visit /node/add/event
    cy.visit('/node/add/event');
    // Fill title with 'Testing event'
    cy.get('#edit-title-0-value').type('Testing event');

    // Fill Duration with 'Duration'
    cy.get('#edit-field-event-duration-0-value').type('Duration');
    // Fill summary field with 'Testing summary.'
    cy.get('#edit-field-summary-0-value').type('Testing summary.');
    // Select start date - now
    cy.get('#edit-field-event-date-0-value-date').type('2020-04-23');
    cy.get('#edit-field-event-date-0-value-time').type('11:11:11');

    // Save Node
    cy.get('#edit-submit').click();
    // Validate URL to be /events/testing-event
    cy.visit('/events/testing-event');
    cy.get('h1').contains('Testing event').should('exist');

    // Validate meta values
    cy.get('head title').should('contain', 'Testing event | Convivial Demo');
    // Get the meta description tag and assert its content
    cy.get('head meta[name="description"]').should('have.attr', 'content', 'Testing summary.');

  });

  it('Validate URL Person', () => {
    // Login to the site as Content Author
    cy.session(['editor'], () => {
      cy.drupalLogin('editor');
    });
    //TODO temporary use site admin until workflow is fixed
    // cy.session(['site_administrator'], () => {
    //   cy.drupalLogin('convivial-demo');
    // });

    // Visit /node/add/person
    cy.visit('/node/add/person');
    // Fill title with 'Testing person'
    cy.get('#edit-title-0-value').type('Testing person');
    // Fill summary field with 'Testing summary.'
    cy.get('#edit-field-summary-0-value').type('Testing summary.');

    // Save Node
    cy.get('#edit-submit').click();
    // Validate URL to be /testing-person
    cy.visit('/about/testing-person');
    cy.get('h1').contains('Testing person').should('exist');

    // Validate meta values
    cy.get('head title').should('contain', 'Testing person | Convivial Demo');
    // Get the meta description tag and assert its content
    cy.get('head meta[name="description"]').should('have.attr', 'content', 'Testing summary.');

  });

  it('Validate URL Place', () => {
    // Login to the site as Content Author
    cy.session(['editor'], () => {
      cy.drupalLogin('editor');
    });
    //TODO temporary use site admin until workflow is fixed
    // cy.session(['site_administrator'], () => {
    //   cy.drupalLogin('convivial-demo');
    // });

    // Visit /node/add/place
    cy.visit('/node/add/place');
    // Fill title with 'Testing place'
    cy.get('#edit-title-0-value').type('Testing place');
    // Fill summary field with 'Testing summary.'
    cy.get('#edit-field-summary-0-value').type('Testing summary.');

    // Save Node
    cy.get('#edit-submit').click();
    // Validate URL to be /places/testing-place
    cy.visit('/places/testing-place');
    cy.get('h1').contains('Testing place').should('exist');

    // Validate meta values
    cy.get('head title').should('contain', 'Testing place | Convivial Demo');
    // Get the meta description tag and assert its content
    cy.get('head meta[name="description"]').should('have.attr', 'content', 'Testing summary.');

  });

  // Topic does not need to be validated yet - no URL alias pattern

  // it('Validate URL Topic', () => {
  //   // Login to the site as Content Author
  //   cy.session(['editor'], () => {
  //     cy.drupalLogin('editor');
  //   });
  //   //TODO temporary use site admin until workflow is fixed
  //   // cy.session(['site_administrator'], () => {
  //   //   cy.drupalLogin('convivial-demo');
  //   // });
  //
  //   // Visit /node/add/topic
  //   cy.visit('/node/add/topic');
  //   // Fill title with 'Testing topic'
  //   cy.get('#edit-title-0-value').type('Testing topic');
  //   // Fill summary field with 'Testing summary.'
  //   cy.get('#edit-field-summary-0-value').type('Testing summary.');
  //
  //   // Save Node
  //   cy.get('#edit-submit').click();
  //   // Validate URL to be /testing-topic
  //   cy.visit('/testing-topic');
  //   cy.get('h1').contains('Testing topic').should('exist');
  //
  //   // Validate meta values
  //   cy.get('head title').should('contain', 'Testing topic | Convivial Demo');
  //   // Get the meta description tag and assert its content
  //   cy.get('head meta[name="description"]').should('have.attr', 'content', 'Testing summary.');
  // });
});
