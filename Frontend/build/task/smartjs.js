const closureCompiler = require('google-closure-compiler').gulp({
        extraArguments: "--process_common_js_modules --module_resolution=NODE --dependency_mode=STRICT"
    }),
    concat = require('gulp-concat'),
    gulp = require('gulp'),
    path = require('path'),
    fileInclude = require('./../tools/file-includer'),
    workers = [];

class JSWorker {
    constructor(config, helper) {
        this.config = config;
        this.helper = helper.clone();
        if (!this.config.name) {
            throw Error("Found Empty name for JS Job...");
        }
        this.validateConfig();
        this.input = this.helper.replaceVariables(this.config.input);
        this.watchInput = '.' + this.helper.replaceAbsolutePath(this.input);
        this.helper.addConfigItem("$name", this.config.name);
        this.helper.addConfigItem("$watch", this.watchInput)
        this.helper.addConfigItem("$", this.input);
        this.getCompilerConfig();
        this.out = this.helper.replaceVariables(this.config.output);
        this.watch = this.helper.replaceVariables(this.config.watch || '$watch/**/*.js');
        if (this.config.includeFile) {
            this.includeFile = `${this.input}/${this.config.includeFile}`;
        }
        this.prepareFiles()
        this.path = path.parse(this.out);
        this.path = this.path.dir + "/" + this.path.base + "/";
        gulp.task(this.taskName, this.work.bind(this));
    }

    get taskName() {
        return `js-${this.config.name}`;
    }

    validateConfig() {
        let config = this.config;
        config.onlyMinify = config.onlyMinify || false;
        config.minify = config.minify || false;
        if (!this.config.input || !this.config.output || !(this.config.files || this.config.includeFile)) {
            throw Error("Invalid Config for: " + this.config.name + ")");
        }
    }

    getCompilerConfig() {
        this.compiler = {
            compilation_level: 'SIMPLE',
            warning_level: 'DEFAULT',
            language_in: 'UNSTABLE',
            language_out: 'UNSTABLE',
            js_output_file: this.config.name + ".min.js"
        }
        if (this.config.compiler) {
            this.compiler = Object.assign(this.compiler, this.config.compiler);
        }
        const keys = Object.keys(this.compiler);
        keys.forEach((key) => {
            if (typeof this.compiler[key] === 'string') {
                this.compiler[key] = this.helper.replaceVariables(this.compiler[key]);
            } else if (Array.isArray(this.compiler[key])) {
                this.compiler[key] = this.compiler[key].map((el) => {
                    return `--${key}=${this.helper.replaceVariables(el)}`;
                }).join("").split(`--${key}=`);
                this.compiler[key].shift();
                this.compiler[key].join(`--${key}=`);
            }
        })
    }

    startWatch() {
        console.log(`[WATCH][${this.config.name}] >> ${this.watch}`);
        const watchArray = [this.watch];
        if (this.includeFile) {
            watchArray.push(this.includeFile);
        }
        gulp.watch(watchArray, gulp.parallel([this.taskName]));
    }

    prepareFiles() {
        if (this.config.includeFile) {
            this.files = fileInclude.findFiles(this.input, this.includeFile, this.helper);
        } else {
            this.files = [];
            for (const file of this.config.files) {
                this.files.push(this.input + "/" + this.helper.replaceVariables(file));
            }
        }
    }

    work() {
        if (this.config.includeFile) {
            this.prepareFiles();
        }
        let d = gulp.src(this.files).pipe(concat(this.config.name + '.js'))
        if (!this.config.onlyMinify) {
            d.pipe(gulp.dest(this.path));
        }
        if (this.config.minify) {
            d.pipe(closureCompiler(this.compiler))
                .on('error', console.log)
                .pipe(gulp.dest(this.path));
        }
        return d;
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
        // we load the json and parse it here
        const js = config.js || [];
        for (let jsConfig of js) {
            const worker = new JSWorker(jsConfig, config.helper);
            workers.push(worker);
        }
    },
    startWatch: function () {
        for (let worker of workers) {
            worker.startWatch();
        }
    }
}
