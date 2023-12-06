import fs from "fs";
import path from "path";
import { build } from 'vite';

const endpoints = fs.readdirSync("src/endpoints");

let index = 0;

if (fs.existsSync("build"))
    fs.rmSync("build", {
        recursive: true,
        force: true
    });

fs.mkdirSync("build");

function func() {
    if (index >= endpoints.length)
        return;

    let endpointPath = path.join("src/endpoints/", endpoints[index]); 
    fs.cpSync(path.join(endpointPath, "/main.ts"), "src/main.ts");
    fs.cpSync(path.join(endpointPath, "/global.d.ts"), "src/global.d.ts");

    build({
    root: '.', // project root directory
    logLevel: 'info', // logs level. It can be 'info', 'warn', 'error', or 'silent'.
    }).then(() => {
        fs.cpSync("dist/index.html", "build/" + endpoints[index] + ".html");
        index++;
        func();
    })
    .catch((error) => console.error(`Build failed with error: ${error}`));
}

func();