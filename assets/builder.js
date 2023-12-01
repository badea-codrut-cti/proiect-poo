import fs from "fs";
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

    fs.cpSync("src/endpoints/" + endpoints[index], "src/main.ts");

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