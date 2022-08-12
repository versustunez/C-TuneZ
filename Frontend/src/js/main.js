class App {
    constructor() {
        window.app = this;
    }

    async ignite() {
        //await sleep(1000);
    }

    finish() {
        canvas.width = innerWidth;
        canvas.height = innerHeight;
        window.wasm.UploadSize();
        registry.wasm.run();
    }
}

ModuleLoader.OnIgnition(async () => {
    window.registry = new Registry();
    window.canvas = $('#main-canvas');
    ModuleLoader.RegisterModule('WASM');
    window.preLoader = new PreLoader();
    const app = new App();
    ModuleLoader.callback = app.finish.bind(app);
    await app.ignite();
    ModuleLoader.FinishModule("Startup");
});

