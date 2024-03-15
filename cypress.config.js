const { defineConfig } = require("cypress");

module.exports = defineConfig({
  viewportHeight: 1120,
  viewportWidth: 1792,
  video: false,
  projectId: 'convivial',
  adminUser: 'admin',
  adminPassword: 'admin',
  e2e: {
    setupNodeEvents(on, config) {
      // implement node event listeners here
    },
    specPattern: 'cypress/e2e/**/*.{js,jsx,ts,tsx}',
    supportFile: 'cypress/support/e2e.js',
  },
  chromeWebSecurity: false,
});
