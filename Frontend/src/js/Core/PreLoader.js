class PreLoader {
    static get name() { return "PreLoader"};
    constructor() {
        this.preloadData = {};
        ModuleLoader.RegisterModule(PreLoader.name);
        Network.requestUrl('/out/preload.json').then(data => {
            this.preloadData = JSON.parse(data);
            PrettyConsole.debug(PreLoader.name, this.preloadData)
            ModuleLoader.FinishModule(PreLoader.name)
        }).catch(err => {
            ModuleLoader.ModuleErrored(PreLoader.name, err);
            PrettyConsole.error(PreLoader.name, err);
        })
    }

    getData(name) {
        return this.preloadData[name] || [];
    }
}
