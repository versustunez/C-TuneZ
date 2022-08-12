class ShaderLoader {
    constructor(preloadList) {
        this.shaders = preloadList;
    }

    async LoadAll() {
        this.data = (await Network.loadAll(this.shaders, "/out/shader/")).map((item) => item.value);
    }

    UploadShaders() {
        VFS.mkdir("/shaders/");
        for (let i = 0; i < this.shaders.length; i++) {
            VFS.WriteFile('/shaders/' + this.shaders[i], this.data[i]);
        }
    }
}