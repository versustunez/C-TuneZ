class Network {
    static getStaticUrl(path) {
        if (!path.startsWith('/')) {
            path = '/' + path;
        }
        return path;
    }

    static async requestUrl(path) {
        let url = this.getStaticUrl(path);
        let raw = await fetch(url)
        if (!raw.ok) {
            return Promise.reject(`Failed to load: ${url}`);
        }
        return await raw.text();
    }

    static loadAll(array, richUrl) {
        const promises = []
        for (const name of array) {
            promises.push(this.requestUrl(richUrl + name));
        }
        return Promise.allSettled(promises);
    }
}
