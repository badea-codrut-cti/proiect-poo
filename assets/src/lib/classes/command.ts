import type { Device } from "../types/device";

export class Command {
    #name: string;

    constructor(name: string) {
        this.#name = name;
    }

    get name() {
        return this.#name;
    }

    handleCommand(argv: string[], dev: Device) {
        return "";
    }
}