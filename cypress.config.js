const { defineConfig } = require("cypress");

module.exports = defineConfig({
  viewportHeight: 1500,
  viewportWidth: 1000,
  videoCompression: 10,
  e2e: {
    setupNodeEvents(on, config) {
      // implement node event listeners here
    },
    specPattern: 'cypress/e2e/**/*.{js,jsx,ts,tsx}',
    supportFile: 'cypress/support/e2e.js',
    baseUrl: 'https://convivial-demo.lndo.site/', // Replace with your application's base URL
  },
  chromeWebSecurity: false,
});