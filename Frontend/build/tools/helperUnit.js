let inConfig = {
    $dir: __dirname,
    $out: __dirname,
    $src: __dirname,
    $: __dirname
}

class HelperUnit {
    constructor(config = null) {
        this.config = config || inConfig;
        this.regEx = {};
        this.buildRegex();
    }

    static setGlobalConfig(config) {
        inConfig = config;
    }

    addConfigItem(name, value, regexOnly = false) {
        if (!regexOnly) {
            this.config[name] = value;
        }
        let replace = name.replace("$", "\\$");
        this.regEx[name] = new RegExp(`(${replace})`, "gm");
    }

    setConfig(config) {
        this.config = config;
        this.buildRegex();
    }

    buildRegex() {
        const keys = Object.keys(this.config);
        this.regEx = {};
        for (const key of keys) {
            this.addConfigItem(key, null, true);
        }
    }

    replaceVariables(string) {
        const keys = Object.keys(this.config);
        for (const key of keys) {
            string = string.replace(this.regEx[key], this.config[key]);
        }
        return string;
    }

    replaceAbsolutePath(string) {
        string = string.replace(this.config["$dir"], this.config["$forWatch"])
        return string;
    }

    clone() {
        return new HelperUnit(this.config);
    }
}

module.exports = HelperUnit;
