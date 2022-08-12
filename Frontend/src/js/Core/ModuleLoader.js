const EVENT_MODULE_LOADED = "module-loaded";

class ModuleLoaderClass {
    constructor() {
        this.modules = {};
        this._cb = null;
        this._errCb = null;
        this.isErrored = false;
        this.isAllLoadedFired = false;
        this.ignitionCallbacks = [];
        this.waitingPromises = [];
        document.addEventListener(EVENT_MODULE_LOADED, this._ModuleLoadedEvent.bind(this));
        PrettyConsole.setColour("App", "#33ffbf")
        window.onerror = this._OnWindowError;
        window.onunhandledrejection = this._OnWindowError;
    }

    _OnWindowError(err) {
        if (!ModuleLoader.isAllLoadedFired) {
            ModuleLoader.ModuleErrored("App", err.reason);
        }
        PrettyConsole.error("Window Error", err);
    }

    set callback(value) {
        this._cb = value;
    }

    set errorCallback(value) {
        this._errCb = value;
    }

    OnIgnition(cb) {
        this.ignitionCallbacks.push(cb);
    }

    RegisterModules(modules) {
        modules.forEach(module => {
            this.modules[module] = false
        });
    }

    FinishModule(name) {
        this.modules[name] = !0;
        PrettyConsole.debug(name, "Module Loading Done");
        document.dispatchEvent(new CustomEvent(EVENT_MODULE_LOADED, {detail: name}));
    }

    ModuleErrored(name, error) {
        if (this._errCb) {
            this._errCb(name, error);
        }
        this.isErrored = true;
        document.dispatchEvent(new CustomEvent(`module-errored`, {
            detail: {name, error}
        }));
    }

    ModulesLoaded(names) {
        if (typeof names === 'string') {
            names = [names];
        }
        let pendingModules = names.filter(module => !this.modules[module]);
        return new Promise((resolve, reject) => {
            if (pendingModules.length === 0)
                return resolve();
            this.waitingPromises.push({
                resolve,
                reject,
                names
            });
        });
    }

    RegisterModule(name) {
        this.modules[name] = false;
    }

    _ModuleLoadedEvent() {
        if (this.isErrored || this.isAllLoadedFired) {
            return;
        }
        this.waitingPromises.forEach((waitingPromise) => {
            let pendingModules = waitingPromise.names.filter(module => !this.modules[module]);
            if (pendingModules.length === 0) {
                this.waitingPromises = this.waitingPromises.filter(promise => promise !== waitingPromise);
                waitingPromise.resolve();
            }
        })
        if (Object.keys(this.modules).filter(module => !this.modules[module]).length) {
            return;
        }
        this.isAllLoadedFired = true;
        if (this._cb) {
            this._cb();
        }
        document.dispatchEvent(new CustomEvent('startupFin'));
        return true;
    }

    Ignite() {
        document.dispatchEvent(new CustomEvent('ignite'));
        this.ignitionCallbacks.forEach((cb) => cb());
    }

    RegisterInit(onInit) {
        document.addEventListener('ignite', onInit);
    }

    LoadOtherJsFiles(files) {
        for (let file of files) {
            ModuleLoader.RegisterModule(file.Name);
        }
        return Promise.allSettled(files.map((file) => this.LoadJSFile(file))).then((data) => {
            const rejectedOnes = data.filter((res) => res.status === 'rejected');
            if (rejectedOnes.length > 0) {
                return Promise.reject(rejectedOnes);
            }
            return data;
        });
    }

    LoadJSFile(file) {
        return new Promise((resolve, reject) => {
            let script = document.createElement('script');
            script.onload = () => {
                resolve(file.Name);
                ModuleLoader.FinishModule(file.Name);
            }
            script.onerror = (err) => {
                reject(err);
                ModuleLoader.ModuleErrored(file.Name, err);
            }
            script.src = file.Src.replace("~/", "/out/js/");
            document.body.appendChild(script);
        });
    }
}

const ModuleLoader = new ModuleLoaderClass();