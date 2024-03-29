'use strict';

// Include gulp.
const gulp = require('gulp');
const config = require('./config.json');

// Include plugins.
const sass = require('gulp-sass')(require('sass'));
const imagemin = require('gulp-imagemin');
const plumber = require('gulp-plumber');
const glob = require('gulp-sass-glob');
const uglify = require('gulp-uglify');
const concat = require('gulp-concat');
const notify = require('gulp-notify');
const rename = require('gulp-rename');
const sourcemaps = require('gulp-sourcemaps');
const jshint = require('gulp-jshint');
const postcss = require('gulp-postcss');
const autoprefixer = require('autoprefixer');
const del = require('del');
const browserSync = require('browser-sync').create();

// Check if local config exists.
var fs = require('fs');
if (!fs.existsSync('./config-local.json')) {
  console.log('\x1b[33m', 'You need to rename default.config-local.json to' +
      ' config-local.json and update its content if necessary.', '\x1b[0m');
  process.exit();
}
//Include local config.
var configLocal = require('./config-local.json');

// Process CSS for production.
gulp.task('css', function() {
  var postcssPlugins = [
    autoprefixer()
  ];

  return gulp.src(config.css.src)
      .pipe(glob())
      .pipe(plumber({
        errorHandler: function (error) {
          notify.onError({
            title:    "Gulp",
            subtitle: "Failure!",
            message:  "Error: <%= error.message %>"
          }) (error);
          this.emit('end');
        }}))
      .pipe(sass({
        outputStyle: 'compressed',
        errLogToConsole: true
      }))
      .pipe(postcss(postcssPlugins))
      .pipe(gulp.dest(config.css.dest));
});

// Process CSS for development.
gulp.task('css_dev', function() {
  var postcssPlugins = [
    autoprefixer()
  ];

  return gulp.src(config.css.src)
    .pipe(glob())
    .pipe(plumber({
      errorHandler: function (error) {
        notify.onError({
          title:    "Gulp",
          subtitle: "Failure!",
          message:  "Error: <%= error.message %>"
        }) (error);
        this.emit('end');
      }}))
    .pipe(sourcemaps.init())
    .pipe(sass({
      outputStyle: 'nested',
      errLogToConsole: true
    }))
    .pipe(postcss(postcssPlugins))
    .pipe(sourcemaps.write('./'))
    .pipe(gulp.dest(config.css.dest))
    .pipe(browserSync.reload({ stream: true, match: '**/*.css' }));
});

// Process CSS for production.
gulp.task('css_components', function() {
  var postcssPlugins = [
    autoprefixer()
  ];

  return gulp.src(config.css_components.src)
    .pipe(glob())
    .pipe(plumber({
      errorHandler: function (error) {
        notify.onError({
          title:    "Gulp",
          subtitle: "Failure!",
          message:  "Error: <%= error.message %>"
        }) (error);
        this.emit('end');
      }}))
    .pipe(sass({
      outputStyle: 'compressed',
      errLogToConsole: true
    }))
    .pipe(postcss(postcssPlugins))
    .pipe(gulp.dest(config.css_components.dest));
});

// Process CSS for development.
gulp.task('css_components_dev', function() {
  var postcssPlugins = [
    autoprefixer()
  ];

  return gulp.src(config.css_components.src)
    .pipe(glob())
    .pipe(plumber({
      errorHandler: function (error) {
        notify.onError({
          title:    "Gulp",
          subtitle: "Failure!",
          message:  "Error: <%= error.message %>"
        }) (error);
        this.emit('end');
      }}))
    .pipe(sourcemaps.init())
    .pipe(sass({
      outputStyle: 'nested',
      errLogToConsole: true
    }))
    .pipe(postcss(postcssPlugins))
    .pipe(sourcemaps.write('./'))
    .pipe(gulp.dest(config.css_components.dest))
    .pipe(browserSync.reload({ stream: true, match: 'components/**/*.css' }));
});

// Compress images.
gulp.task('images', function () {
  return gulp.src(config.images.src)
      .pipe(imagemin([
        imagemin.gifsicle({interlaced: true}),
        imagemin.mozjpeg({progressive: true}),
        imagemin.optipng({optimizationLevel: 5}),
        imagemin.svgo({
          plugins: [
            {removeViewBox: false},
            {cleanupIDs: false}
          ]
        })
      ]))
      .pipe(gulp.dest(config.images.dest));
});

// Concat all JS files into one file and minify it.
gulp.task('scripts', function() {
  return gulp.src(config.js.src)
      .pipe(plumber({
        errorHandler: function (error) {
          notify.onError({
            title: 'Gulp scripts processing',
            subtitle: 'Failure!',
            message: 'Error: <%= error.message %>'
          })(error);
          this.emit('end');
        }}))
      .pipe(concat('./index.js'))
      .pipe(gulp.dest('./assets/scripts/'))
      .pipe(rename(config.js.file))
      .pipe(uglify())
      .pipe(gulp.dest(config.js.dest));
});

// Concat all JS files into one file.
gulp.task('scripts_dev', function() {
  return gulp.src(config.js.src)
      .pipe(plumber({
        errorHandler: function (error) {
          notify.onError({
            title: 'Gulp scripts processing',
            subtitle: 'Failure!',
            message: 'Error: <%= error.message %>'
          })(error);
          this.emit('end');
        }}))
      .pipe(concat('./index.js'))
      .pipe(gulp.dest('./assets/scripts/'))
      .pipe(sourcemaps.init())
      .pipe(rename(config.js.file))
      .pipe(sourcemaps.write('./'))
      .pipe(gulp.dest(config.js.dest))
      .pipe(browserSync.reload({ stream: true, match: '**/*.js' }))
      .pipe(notify({message: 'Rebuild all custom scripts. Please refresh your browser', onLast: true}));
});

// Move external libraries into final destination.
gulp.task('scripts_libraries', function() {
  return gulp.src(config.libraries.src, {
    base: './node_modules'
  })
    .pipe(plumber({
      errorHandler: function (error) {
        notify.onError({
          title: 'Gulp scripts processing',
          subtitle: 'Failure!',
          message: 'Error: <%= error.message %>'
        })(error);
        this.emit('end');
      }}))
    .pipe(gulp.dest(config.libraries.dest));
});

// Remove temporary JS storage.
gulp.task('removeTemporaryStorage', function() {
  return del('./assets/scripts/');
});

// Remove sourcemaps.
gulp.task('removeSourceMaps', function() {
  return del(['./css/style.css.map', './js/convivial_bootstrap.js.map']);
});

// Watch task.
gulp.task('watch', function() {
  gulp.watch(config.css.src, { usePolling: true }, gulp.series('css_dev'))
  gulp.watch(config.css_components.src, { usePolling: true }, gulp.series('css_components_dev'))
  gulp.watch(config.js.src, { usePolling: true }, gulp.series('scripts_dev', 'removeTemporaryStorage'));
});

// JS Linting.
gulp.task('js-lint', function() {
  return gulp.src(config.js.src)
      .pipe(jshint())
      .pipe(jshint.reporter('default'));
});

// BrowserSync settings.
gulp.task('browserSync', function() {
  browserSync.init({
    proxy: 'http://appserver', // Could be 'http://appserver' if you're running apache.
    host: 'bs.convivial-demo.localhost',
    socket: {
      domain: 'bs.convivial-demo.localhost', // The node proxy domain you defined in .lando.yaml. Must be https?
      port: 80 // NOT the 3000 you might expect.
    },
    open: false,
    logConnections: true,
  });
});

// Compile for production.
gulp.task('serve', gulp.parallel('css', 'css_components', gulp.series('scripts', 'removeTemporaryStorage'), 'scripts_libraries', 'images', 'removeSourceMaps'));

// Compile for development + BrowserSync + Watch
gulp.task('serve_dev', gulp.series(gulp.parallel('css_dev', 'css_components_dev', gulp.series('scripts_dev', 'removeTemporaryStorage')), 'scripts_libraries', gulp.parallel('watch', 'browserSync')));

// Default Task
gulp.task('default', gulp.series('serve'));

// Development Task
gulp.task('dev', gulp.series('serve_dev'));
