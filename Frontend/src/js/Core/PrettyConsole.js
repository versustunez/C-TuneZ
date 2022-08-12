class PrettyConsole {
    static setColour(module, color) {
        if (!this.colours) {
            this.colours = {};
        }
        this.colours[module] = color;
    }

    static getColour(module) {
        if (!this.colours) {
            this.colours = {};
        }
        if (!this.colours.hasOwnProperty(module)) {
            if (module.startsWith("TuneZ::"))
                this.colours[module] = "#0082ff";
            else
                this.colours[module] = this.rgbToHex(...this.hexToRgb(this.generateHexFromString(module)));
        }
        return this.colours[module];
    }

    static log(module, ...args) {
        if (args.length === 1 && !Array.isArray(args[0])) {
            this.output(module, "#5c6bc0", args[0], 'log');
        } else {
            if (args.length === 1) args = args[0];
            this.outputGroup(module, "#5c6bc0", args, 'log');
        }
    }

    static error(module, ...args) {
        if (args.length === 1 && !Array.isArray(args[0])) {
            this.output(module, "#c62828", args[0], 'error');
        } else {
            if (args.length === 1) args = args[0];
            this.outputGroup(module, "#c62828", args, 'error');
        }
    }

    static warn(module, ...args) {
        if (args.length === 1 && !Array.isArray(args[0])) {
            this.output(module, "#f9a825", args[0], 'warn');
        } else {
            if (args.length === 1) args = args[0];
            this.outputGroup(module, "#f9a825", args, 'warn');
        }
    }

    static debug(module, ...args) {
        if (args.length > 1) {
            this.outputGroup(module, '#e91e63', args, 'debug');
        } else {
            this.output(module, '#e91e63', args[0], 'debug');
        }
    }

    static output(module, style, message, type) {
        const color = this.getColour(module);
        if (typeof message === "object") {
            console[type](`%c[${module}]`, `color:${color};font-weight: bold;`, message);
            return;
        }
        console[type](`%c[${module}] %c ${message}`, `color:${color};font-weight: bold;`, `color: ${style}`);
    }

    static outputGroup(module, style, messages, type) {
        const color = this.getColour(module);
        console.group(`%c===[${module}]===`, `color:${color};font-weight: bold;`);
        for (let message of messages) {
            if (type === 'debug' && typeof message === 'string') {
                message = `%c${message}`;
                console.debug(message, `color: ${style}`);
            } else {
                console[type](message);
            }
        }
        console.groupEnd();
    }

    static generateHexFromString(string = "") {
        let hash = 0, len = string.length;
        for (let i = 0; i < len; i++) {
            string.charCodeAt(i)
            hash = ((hash << 5) - hash) + string.charCodeAt(i);
            hash |= 0; // to 32bit integer
        }
        return Math.abs(hash).toString(16).slice(0, 6);
    }

    static hexToRgb(hex) {
        let bigint = parseInt(hex, 16);
        let r = (bigint >> 16) & 255;
        let g = (bigint >> 8) & 255;
        let b = bigint & 255;
        return [r, g, b];
    }

    static rgbToHex(r, g, b) {
        return "#" + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1);
    }
}
