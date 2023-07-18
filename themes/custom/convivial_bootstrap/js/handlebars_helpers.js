(function (Handlebars) {
  Handlebars.registerHelper('setVar', function (varName, varValue, options) {
    options.data.root[varName] = varValue
  })
  Handlebars.registerHelper('greaterThan', function (a, b, options) {
    return a > b
  })
  Handlebars.registerHelper('lessThan', function (a, b, options) {
    return a < b
  })
  Handlebars.registerHelper('eq', function (a, b, options) {
    return a === b
  })
  Handlebars.registerHelper('or', function (a, b, options) {
    return a || b
  })
})(Handlebars);
