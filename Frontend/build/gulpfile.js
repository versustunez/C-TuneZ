const gulp = require('gulp'),
    config = require('./tools/buildConfig'),
    smartJS = require('./task/smartjs'),
    smartIcon = require('./task/smarticon'),
    smartCss = require('./task/smartcss');

config.prepare();
smartJS.prepare(config);
smartCss.prepare(config);
smartIcon.prepare(config);

gulp.task('watchMe', () => {
    smartJS.startWatch();
    smartCss.startWatch();
    smartIcon.startWatch();
});

gulp.task('default', gulp.parallel([...smartJS.build(), ...smartCss.build(), ...smartIcon.build()]));

gulp.task('watch', gulp.parallel(['watchMe', ...smartJS.build(), ...smartCss.build(), ...smartIcon.build()]));
