const fs = require('fs'),
    HelperUnit = require('./helperUnit'),
    helper = new HelperUnit(),
    config = {};

module.exports = {
    config: config,
    helper: helper,
    prepare: function () {
        if (!fs.existsSync(__dirname + '/../../buildConfig.json')) {
            console.error("Cannot find Config JSON");
            process.exit(40);
        }
        const currentPath = process.cwd();
        const baseDir = currentPath + '/../';
        const data = JSON.parse(fs.readFileSync(currentPath + '/../buildConfig.json').toString());
        const src = data['src'].replace(/(\$dir)/gm, baseDir);
        const out = data['out'].replace(/(\$dir)/gm, baseDir);
        helper.setConfig({
            $dir: baseDir,
            $out: out,
            $src: src,
            $rawDir: __dirname,
            $forWatch: data['src'].replace(/(\$dir)/gm, '../')
        })
        config.dir = baseDir;
        config.src = src;
        config.out = out;
        config.js = data.js || [];
        config.scss = data.scss || [];
        config.icons = data.icons || [];
        this.js = config.js;
        this.scss = config.scss;
        this.icons = config.icons;
    }
}
