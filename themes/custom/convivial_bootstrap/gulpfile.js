'use strict';

// Include gulp.
import gulp from 'gulp';

// Include plugins.
import gulpSass from 'gulp-sass';
import * as dartSass from 'sass';
import glob from 'gulp-sass-glob';
import uglify from 'gulp-uglify';
import concat from 'gulp-concat';
import rename from 'gulp-rename';
import sourcemaps from 'gulp-sourcemaps';
import jshint from 'gulp-jshint';
import postcss from 'gulp-postcss';
import autoprefixer from 'autoprefixer';
import del from 'del';
import browserSync from 'browser-sync';
import {createRequire} from 'module';
import {existsSync, readFileSync} from 'fs';

// Include config.
const config = createRequire(import.meta.url)("./config.json");

// Initialize browserSync
const bs = browserSync.create();

// Check if local config exists.
if (!existsSync('./config-local.json')) {
  console.log('\x1b[33m', 'You need to rename default.config-local.json to' +
    ' config-local.json and update its content if necessary.', '\x1b[0m');
  process.exit();
}

// Include local config.
const configLocal = JSON.parse(readFileSync('./config-local.json', 'utf8'));

// Import sass module dynamically.
const sass = gulpSass(dartSass);

// Process CSS for production.
gulp.task('css', function () {
  const postcssPlugins = [
    autoprefixer()
  ];

  return gulp.src(config.css.src)
    .pipe(glob())
    .pipe(sass({
      outputStyle: 'compressed'
    }).on('error', sass.logError))
    .pipe(postcss(postcssPlugins))
    .pipe(gulp.dest(config.css.dest));
});

// Process CSS for development.
gulp.task('css_dev', function () {
  const postcssPlugins = [
    autoprefixer()
  ];

  return gulp.src(config.css.src)
    .pipe(glob())
    .pipe(sourcemaps.init())
    .pipe(sass({
      outputStyle: 'expanded'
    }).on('error', sass.logError))
    .pipe(postcss(postcssPlugins))
    .pipe(sourcemaps.write('./'))
    .pipe(gulp.dest(config.css.dest))
    .pipe(bs.reload({stream: true, match: '**/*.css'}));
});

// Process CSS for production.
gulp.task('css_components', function () {
  const postcssPlugins = [
    autoprefixer()
  ];

  return gulp.src(config.css_components.src)
    .pipe(glob())
    .pipe(sass({
      outputStyle: 'compressed'
    }).on('error', sass.logError))
    .pipe(postcss(postcssPlugins))
    .pipe(gulp.dest(config.css_components.dest));
});

// Process CSS for development.
gulp.task('css_components_dev', function () {
  const postcssPlugins = [
    autoprefixer()
  ];

  return gulp.src(config.css_components.src)
    .pipe(glob())
    .pipe(sourcemaps.init())
    .pipe(sass({
      outputStyle: 'expanded'
    }).on('error', sass.logError))
    .pipe(postcss(postcssPlugins))
    .pipe(sourcemaps.write('./'))
    .pipe(gulp.dest(config.css_components.dest))
    .pipe(bs.reload({stream: true, match: 'components/**/*.css'}));
});

// Concat all JS files into one file and minify it.
gulp.task('scripts', function () {
  return gulp.src(config.js.src)
    .pipe(concat('./index.js'))
    .pipe(gulp.dest('./assets/scripts/'))
    .pipe(rename(config.js.file))
    .pipe(uglify())
    .pipe(gulp.dest(config.js.dest));
});

// Concat all JS files into one file.
gulp.task('scripts_dev', function () {
  return gulp.src(config.js.src)
    .pipe(concat('./index.js'))
    .pipe(gulp.dest('./assets/scripts/'))
    .pipe(sourcemaps.init())
    .pipe(rename(config.js.file))
    .pipe(sourcemaps.write('./'))
    .pipe(gulp.dest(config.js.dest))
    .pipe(bs.reload({stream: true, match: '**/*.js'}));
});

// Move external libraries into final destination.
gulp.task('scripts_libraries', function () {
  return gulp.src(config.libraries.src, {
    base: './node_modules',
    encoding: false,
  })
    .pipe(gulp.dest(config.libraries.dest));
});

// Remove temporary JS storage.
gulp.task('removeTemporaryStorage', function () {
  return del('./assets/scripts/');
});

// Remove sourcemaps.
gulp.task('removeSourceMaps', function () {
  return del(['./css/style.css.map', './js/convivial_bootstrap.js.map']);
});

// Watch task.
gulp.task('watch', function () {
  gulp.watch(config.css.src, {usePolling: true}, gulp.series('css_dev'))
  gulp.watch(config.css_components.src, {usePolling: true}, gulp.series('css_components_dev'))
  gulp.watch(config.js.src, {usePolling: true}, gulp.series('scripts_dev', 'removeTemporaryStorage'));
});

// JS Linting.
gulp.task('js-lint', function () {
  return gulp.src(config.js.src)
    .pipe(jshint())
    .pipe(jshint.reporter('default'));
});

// BrowserSync settings.
gulp.task('browserSync', function () {
  // Setup a browsersync server.
  bs.init({
    // Could be 'http://appserver' if you're running apache.
    proxy: 'http://appserver',
    socket: {
      // The node proxy domain you defined in .lando.yaml. Must be https?
      domain: 'https://bs.convivial-demo.lndo.site',
      // NOT the 3000 you might expect.
      port: 80
    },
    open: false,
    logLevel: 'debug',
    logConnections: true,
  });
});

// Compile for production.
gulp.task('serve', gulp.parallel('css', 'css_components', gulp.series('scripts', 'removeTemporaryStorage'), 'scripts_libraries', 'removeSourceMaps'));

// Compile for development + BrowserSync + Watch
gulp.task('serve_dev', gulp.series(gulp.parallel('css_dev', 'css_components_dev', gulp.series('scripts_dev', 'removeTemporaryStorage')), 'scripts_libraries', gulp.parallel('watch', 'browserSync')));

// Default Task
gulp.task('default', gulp.series('serve'));

// Development Task
gulp.task('dev', gulp.series('serve_dev'));
