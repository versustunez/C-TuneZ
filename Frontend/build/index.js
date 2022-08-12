const gulp = require('gulp-cli'),
    shouldWatch = process.argv.length > 2 && process.argv[2] === 'watch';

gulp()
require('./gulpfile')
if (shouldWatch) {
    gulp.series('watchMe')();
} else {
    gulp.series('default')();
}
