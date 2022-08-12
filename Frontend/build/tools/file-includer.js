const fs = require('fs');

function findFiles(dir, file, helper) {
    file = helper.replaceVariables(file);
    const d = fs.readFileSync(file).toString("UTF8");
    const split = d.split("\n");
    let files = [];
    for (let file of split) {
        if (file.startsWith("--") || file.trim() === "") {
            continue;
        }
        if (file.startsWith("@import")) {
            let iFile = file.split("@import")[1].trim();
            files.push(...this.findFiles(dir, `${dir}/${iFile}.path`, helper));
        } else {
            files.push(`${dir}/${helper.replaceVariables(file)}.js`);
        }
    }
    return files;
}

module.exports = {
    findFiles
};
