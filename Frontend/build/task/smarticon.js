const gulp = require('gulp'),
    builder = require('./../tools/iconSprite'),
    workers = [];

class IconWorker {
    constructor(icon, helper) {
        this.config = icon;
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
        return `icon-${this.config.name}`;
    }

    startWatch() {
        console.log(`[WATCH][${this.config.name}] >> ${this.watch}`);
        gulp.watch(this.watch, gulp.series(this.taskName))
    }

    work() {
        builder.buildIconSprites(this);
        return gulp.src(this.input)
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
        const icons = config.icons || [];
        for (let icon of icons) {
            workers.push(new IconWorker(icon, config.helper));
        }
    },
    startWatch: function () {
        for (let worker of workers) {
            worker.startWatch();
        }
    }
}
