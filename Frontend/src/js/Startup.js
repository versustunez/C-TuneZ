new LoadingScreen();
ModuleLoader.RegisterModules(['Startup']);
ModuleLoader.LoadOtherJsFiles([
    {Src: '~/wasm.min.js', Name: 'WASMJS'},
    {Src: '~/app.min.js', Name: 'App'},
]).then(() => {
    PrettyConsole.log("ModuleLoader", "All JS Modules loaded");
    ModuleLoader.Ignite();
}).catch((err) => {
    PrettyConsole.error("ModuleLoader", err);
});
