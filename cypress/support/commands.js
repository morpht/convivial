// ***********************************************
// This example commands.js shows you how to
// create various custom commands and overwrite
// existing commands.
//
// For more comprehensive examples of custom
// commands please read more here:
// https://on.cypress.io/custom-commands
// ***********************************************

/**
 * Defines a cypress command that executes drush commands
 */
Cypress.Commands.add('drush', (command, args = [], options = {}) => {
  return cy.exec(`lando drush ${command} ${stringifyArguments(args)} ${stringifyOptions(options)} -y`);
});

/**
 * Returns a series of arguments, separated by spaces
 *
 * @param {*} args
 * @returns
 */
function stringifyArguments(args) {
  return args.join(' ');
}

/**
 * Returns a string from an array of options
 *
 * @param {array} options
 * @returns
 */
function stringifyOptions(options) {
  return Object.keys(options).map(option => {
    let output = `--${option}`;

    if (options[option] === true) {
      return output;
    }

    if (options[option] === false) {
      return '';
    }

    if (typeof options[option] === 'string') {
      output += `="${options[option]}"`;
    } else {
      output += `=${options[option]}`;
    }

    return output;
  }).join(' ');
}

/**
 * Logs out the user.
 */
Cypress.Commands.add('drupalLogout', () => {
  cy.visit('/user/logout');
});

/**
 * Logs a user in by their uid via drush uli
 */
Cypress.Commands.add('loginUserByUid', (uid) => {
  cy.drush('uli', [], { uid, uri: Cypress.config('baseUrl') })
    .its('stdout')
    .then(function (url) {
      cy.visit(url);
    });
});

let currentSession;

Cypress.Commands.add('setCurrentSession', (session) => {
  currentSession = session;
});

Cypress.Commands.add('getCurrentSession', () => {
  return currentSession;
});

Cypress.Commands.add('validateSitemap', (regex, is_matching) => {
  // To prevent tests to randomly fail we give server some time to load before regenerating the sitemap
  cy.wait(3000);
  // Visit /admin/config/search/simplesitemap
  cy.visit('/admin/config/search/simplesitemap');
  // Regenerate XML
  cy.get('#edit-regenerate-submit').click();
  cy.wait(3000);

  // Validate that page is or is not present on /sitemap.xml
  cy.request('/sitemap.xml').then((response) => {
    // Parse the XML body
    let parser = new DOMParser();
    let xml = parser.parseFromString(response.body, 'text/xml');

    // Extract the loc elements (which contain the page URLs)
    let urls = Array.from(xml.getElementsByTagName('loc')).map((loc) => loc.textContent);

    // Check if any URL matches the regular expression
    let hasMatch = urls.some(url => regex.test(url));

    // Assert that URL matches (or not based on is_matching parameter) the regular expression
    expect(hasMatch).to.equal(is_matching);
  });
});

/**
 * Add login command to Cypress
 */
Cypress.Commands.add('drupalLogin', (user) => {
  let password = generateRandomPassword(10);
  cy.setPasswordToUsername(user, password);
  // let was_enabled = false;
  // if (cy.isModuleEnabled('antibot').result) {
  //   cy.drush('pm:uninstall antibot');
  //   was_enabled = true;
  // }
  return cy.request({
    method: 'POST',
    url: '/user/login',
    form: true,
    body: {
      name: user,
      pass: password,
      form_id: 'user_login_form',
    },
  });
}, {
  cacheAcrossSpecs: true,
});

Cypress.Commands.add('drupalLogout', () => {
  return cy.request('/user/logout');
});

Cypress.Commands.add('drupalDrushCommand', (command) => {
  var cmd = Cypress.env('drupalDrushCmdLine');

  if (cmd == null) {
    cmd = 'drush %command';
  }

  if (typeof command === 'string') {
    command = [command];
  }

  const execCmd = cmd.replace('%command', command.join(' '));

  return cy.exec(execCmd);
});

/**
 * Unblocks a user by their username via drush user:unblock
 */
Cypress.Commands.add('unblockUserByUsername', (username) => {
  cy.exec(`lando drush user:unblock ${username}`);
});

/**
 * Blocks a user by their username via drush user:block
 */
Cypress.Commands.add('blockUserByUsername', (username) => {
  cy.exec(`lando drush user:block ${username}`);
});

/**
 * Set password to a user by their username via drush user:password
 */
Cypress.Commands.add('setPasswordToUsername', (username, password) => {
  cy.exec(`lando drush upwd ${username} "${password}"`);
});

/**k
 * Generates a random password
 *
 * @param length
 * @returns {string}
 */
function generateRandomPassword(length) {
  let result = '';
  const characters = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#%&*_+:<>';
  const charactersLength = characters.length;
  for (let i = 0; i < length; i++) {
    result += characters.charAt(Math.floor(Math.random() * charactersLength));
  }
  return result;
}

Cypress.Commands.add('waitForDrupalBatch', (url, timeout = 30000) => {
  const startTime = new Date().getTime();

  const requestLoop = () => {
    cy.request({
      url: url,
      failOnStatusCode: false,
    }).then((response) => {
      // If the batch process is finished (you can check this based on the response status, body, headers, etc.), then stop the loop
      if (response.body.finished) {
        return;
      }

      // If the timeout has been exceeded, then throw an error
      if (new Date().getTime() - startTime > timeout) {
        throw new Error('Timeout exceeded while waiting for Drupal batch process to finish');
      }

      // If the batch process is not yet finished, then wait for a short period of time and try again
      cy.wait(500); // wait for 500 ms
      requestLoop();
    });
  };

  requestLoop();
});

/**
 * Validate if module is activated
 */
Cypress.Commands.add('isModuleEnabled', (module_machine_name) => {
  cy.exec(`lando drush pm-list --type=module --status=enabled --format='string' --field='name' --filter='${module_machine_name}' --no-core`)
    .then((result) => {
      let enabledModules = Array.from(result.stdout.split('\n'));
      return enabledModules.some(module => module === module_machine_name);
    });
});

/**
 * Visit page and delete it if it exists
 */
Cypress.Commands.add('visitAndDeletePage', (pageUrl) => {
  cy.request({
    url: pageUrl,
    failOnStatusCode: false,
  }).then((response) => {
    // Check if the page exists (status code 2xx or 3xx)
    if (response.status >= 200 && response.status < 400) {
      cy.visit(pageUrl);
      cy.timeout(1000);
      cy.get('.nav-link').contains('Delete').focus().click().then(() => {
        cy.get('#edit-submit').click();
      });
    } else {
      // Page doesn't exist, log a message
      cy.log(`Page ${pageUrl} does not exist.`);
    }
  });
});