const gulp = require('gulp'),
    HelperUnit = require('./../tools/helperUnit'),
    sass = require('gulp-sass')(require('sass')),
    clean = require('gulp-clean-css'),
    path = require('path'),
    workers = [];

class SCSSWorker {
    constructor(config, helper) {
        this.config = config;
        this.helper = helper.clone();
        this.input = this.helper.replaceVariables(this.config.input);
        this.watchInput = '.' + this.helper.replaceAbsolutePath(this.input);
        this.helper.addConfigItem("$name", this.config.name);
        this.helper.addConfigItem("$watch", this.watchInput);
        this.helper.addConfigItem("$", this.input);
        this.out = this.helper.replaceVariables(this.config.output);
        this.watch = this.helper.replaceVariables(this.config.watch || '$watch');
        gulp.task(this.taskName, this.work.bind(this));
    }

    get taskName() {
        return `scss-${this.config.name || 'unknown'}`;
    }

    startWatch() {
        console.log(`[WATCH][${this.config.name}] >> ${this.watch}`);
        gulp.watch(this.watch, gulp.series(this.taskName))
    }

    work() {
        return gulp.src(this.input)
            .pipe(sass().on('error', sass.logError))
            .pipe(clean())
            .pipe(gulp.dest(this.out));
    }
}

module.exports = {
    build: function () {
        let array = [];
        for (let worker of workers) {
            array.push(worker.taskName);
        }
        return array;
    },
    prepare: function (config) {
        const scss = config.scss || [];
        for (let scssItem of scss) {
            workers.push(new SCSSWorker(scssItem, config.helper));
        }
    },
    startWatch: function () {
        for (let worker of workers) {
            worker.startWatch();
        }
    }
}
