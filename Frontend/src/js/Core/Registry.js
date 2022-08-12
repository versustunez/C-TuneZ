class Registry {
    constructor() {
        this.items = {};
        return new Proxy(this, {
            get(target, prop, _) {
                if (prop === "get" || prop === "set") {
                    return target[prop];
                }
                return target.items[prop];
            },
            set(obj, prop, value) {
                obj.items[prop] = value;
                return true;
            }
        })
    }

    get(name) {
        return this[name];
    }

    set(name, item) {
        this[name] = item;
    }
}
