describe('My First Test', () => {
  it('Visits the homepage', () => {
    cy.visit(Cypress.env('baseUrl') + '/node')
    cy.get('h1').should('contain', 'Welcome to Convivial')
    cy.title().should('include', 'Convivial')
  })
})
