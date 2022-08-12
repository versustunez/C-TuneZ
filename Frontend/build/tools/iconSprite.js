const path = require('path'),
    SVGSpriter = require('svg-sprite'),
    fs = require('fs'),
    fsExtra = require('node-fs-extra');

let toMergeData = [];

function buildIconSprites(config) {
    config.spriter = {
        shape: {
            id: {
                generator: function (name) {
                    return "v-" + name.replace('.svg', '');
                }
            }
        },
        svg: {
            xmlDeclaration: true,
            doctypeDeclaration: true
        },
        mode: {
            symbol: true,
            defs: true
        }
    }

    builder.init(config);
    builder.generateFromIcons();
}

let builder = {
    init: function (config) {
        this.config = config;
    },
    generateFromIcons: function () {
        const spriter = new SVGSpriter(this.config.spriter);
        this.readFiles(this.config.input, spriter);
        spriter.compile(function (error, result) {
            if (error !== null) {
                throw new Error(error);
            }
            builder.writeData(result.symbol.sprite.contents.toString());
        });
    },

    writeData: function (data) {
        let out = builder.config.out + builder.config.config.name + ".svg";
        let dirname = path.dirname(out);
        if (!fs.existsSync(dirname)) {
            fsExtra.mkdirpSync(dirname);
        }
        fs.writeFileSync(out, data);
    },

    readFiles: function (dir, spriter) {
        let files = fs.readdirSync(dir);
        for (let file of files) {
            const split = file.split('.');
            if (split[split.length - 1] === 'svg') {
                let filePath = path.resolve(dir, file);
                spriter.add(
                    filePath,
                    file,
                    fs.readFileSync(filePath, {encoding: 'utf-8'})
                );
            }
        }
    }
};

module.exports.buildIconSprites = buildIconSprites;
