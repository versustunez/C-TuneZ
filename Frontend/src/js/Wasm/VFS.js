class VFS {
    static Instance = null;

    get raw() {
        return VFS.Instance;
    }

    static mkdir(path) {
        if (!VFS.Exists(path)) {
            VFS.Instance.mkdir(path);
        }
    }

    static WriteFile(path, data) {
        VFS.Instance.writeFile(path, data);
    }

    static ReadFile(path, encoding = 'utf8') {
        return VFS.Instance.readFile(path, {encoding: encoding});
    }

    static Exists(path) {
        return VFS.Instance.analyzePath(path)?.exists ?? false;
    }
}