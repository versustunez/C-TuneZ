class WasmHandler {
    constructor() {
        this.module = null;
        this.functions = {};
        this.whiteList = [
            'module',
            'Init',
            'UploadSize',
            'functions'
        ]
        const proxy = new Proxy(this, WasmHandler.getProxy());
        window.addEventListener('beforeunload', this.BeforeUnload.bind(proxy));
        window.addEventListener('keydown', this.SetInputHandle.bind(proxy));
        document.addEventListener('pointerlockchange', this.PointerLockChange.bind(proxy));
        return proxy;
    }

    async Init() {
        canvas.addEventListener('contextmenu', (ev) => {
            ev.preventDefault();
            ev.stopPropagation();
            return false;
        })
        this.module = await TuneZ({
            locateFile: function (path) {
                return '/out/ws/' + path;
            },
            canvas
        }).then(Module => Module.ready);
        VFS.Instance = wasm.module.FS;
    }

    UploadConfig() {
        const data = localStorage.getItem('app.config');
        try {
            // sorry have to make sure that the json is valid, so we are not passing invalid data to nlohmann::json that will crash :(
            JSON.parse(data);
            this.module._app_restore_config(data);
        } catch (_) {
        }
    }

    BeforeUnload() {
        const shutdownCB = this.shutdown;
        if (shutdownCB) {
            shutdownCB();
        }
        this.app_save_config();
        if (VFS.Exists("/config.json")) {
            localStorage.setItem('app.config', VFS.ReadFile("/config.json"));
        }
    }

    SetInputHandle(ev) {
        if (ev.key === 'Alt') {
            !document.pointerLockElement ? canvas.requestPointerLock() : document.exitPointerLock();
            ev.preventDefault();
            ev.stopPropagation();
        } else if (ev.ctrlKey && ev.key === 'g') {
            this.app_toggle_render_gui();
            ev.preventDefault();
            ev.stopPropagation();
        }
    }

    PointerLockChange() {
        const lockState = !!document.pointerLockElement;
        this.app_set_lock(lockState);
    }

    UploadSize() {
        this.module._app_set_dimensions(canvas.width, canvas.height);
    }

    static getProxy() {
        return {
            get: function (target, prop, _) {
                if (target.whiteList.includes(prop)) {
                    return target[prop];
                }
                return target['functions'][prop] ?? target['module'][`_${prop}`];
            },
            set: function (target, prop, value) {
                if (prop === "module") {
                    target[prop] = value;
                    return true;
                }
                return false;
            },
        }
    }
}

ModuleLoader.ModulesLoaded(['PreLoader']).then(async () => {
    const wasm = new WasmHandler();
    window.wasm = registry.wasm = wasm;
    const shaderLoader = registry.shaderLoader = new ShaderLoader(preLoader.getData("shader"));
    await shaderLoader.LoadAll();
    await wasm.Init();
    shaderLoader.UploadShaders();
    ModuleLoader.FinishModule("WASM");
});