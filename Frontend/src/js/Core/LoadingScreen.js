class LoadingScreen {

    constructor() {
        document.addEventListener(EVENT_MODULE_LOADED, this._ModuleLoadedEvent.bind(this));
        document.addEventListener('module-errored', this._OnErrored.bind(this));
        document.addEventListener('startupFin', this._StartupDone.bind(this));
        this.loader = $('v-loader');
    }

    _ModuleLoadedEvent() {
        // Hmm, MAYBE DO SOMETHING? xD
    }

    _OnErrored(ev) {
        const data = ev.detail;
        this.loader.setAttribute('errored', "1");
        $('.text-small-errored', this.loader).innerHTML = `Module: "${data.name}" is Errored! <br>Cause: <br>${data.error ?? 'Unknown'}`;
    }

    async _StartupDone() {
        // Hide Loading Screen :>
        await sleep(100);
        this.loader.setAttribute('success', "1");
        await sleep(2100);
        document.body.removeChild(this.loader);
    }
}