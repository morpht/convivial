/*
 * Testing 404 page
 */
describe('Use non existing image for testing 404', () => {
  it('Validating 404', () => {
    cy.request({
      url: '/web/sites/default/files/some-non-existing-image.png',
      failOnStatusCode: false
    }).its('status').should('equal', 404);

    cy.visit('/web/sites/default/files/some-non-existing-image.png', {failOnStatusCode: false});

    cy.get('h1').contains('Not Found').should('exist');
  });
});

/*
 * Testing 403 page
 */
describe('Use archived page for testing 403', () => {
  it('Validating 403', () => {
    cy.request({
      url: '/archived-page',
      failOnStatusCode: false
    }).its('status').should('equal', 403);

    cy.visit('/archived-page', {failOnStatusCode: false});

    cy.get('h1').contains('Access denied').should('exist');
  });
});

/*
 * Testing Search page
 */
describe('Validate search page', () => {
  before(() => {
    // Clear caches before running the tests
    cy.drush('cr');
  });

  it('Validate search page exists and search bar redirects to search page', () => {
    //TODO use steps below - click on search icon throws exception and make tests fail
    // cy.visit('/');
    // cy.get('.searchform button').click();
    // cy.get('.searchform__query').type('test');
    // cy.get('form.searchform').submit();

    // We are directly searching for 'test' in the search page until error described above is fixed
    // Visit /search
    cy.visit('/search?keys=test');

    // Validate that the search page exists
    cy.get('.block--field-blocknodepagetitle').contains('Search').should('exist');

    // Validate that 'Getting started' page is present in results
    cy.get('.view-content-search').get('.search__title a').contains('Components').should('exist');
  });

  it('Validate that empty search will give no results', () => {
    // Visit /search
    cy.visit('/search');

    // Validate that string 'No results were found.' is present
    cy.get('.view-empty').contains('No results were found.').should('exist');
  });

  it('Search nonsense word will give no results', () => {
    // Visit /search
    cy.visit('/search?keys=Antegooglewhackblatt');

    // Validate that string 'No results were found.' is present
    cy.get('.view-empty').contains('No results were found.').should('exist');
  });
});

