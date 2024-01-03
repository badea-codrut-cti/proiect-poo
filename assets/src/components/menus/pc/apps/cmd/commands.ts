import { Command } from "../../../../../lib/classes/command";
import type { Device } from "../../../../../lib/types/device";

export class IPConfig extends Command {
    constructor() {
        super("ipconfig");
    }

    async handleCommand(argv: string[], dev: Device, ) {
        return dev.interfaces.filter(intf => !intf.isUnnumbered).map(intf => {
            return `
            Link-local IPv6 Address.........: ${intf.ipv6.linkLocalAddress} 
            IPv6 Address....................: ${(intf.ipv6.gua[0] || {address: "::"}).address}
            IPv4 Address....................: ${intf.ip.address}
            Subnet Mask.....................: ${intf.ip.subnetMask.dotNotation} 
            Default Gateway.................: ${intf.ipv6.defaultGateway}
            ${intf.ip.defaultGateway}`;
        }).join("\n");
    }
}

export class Ping extends Command {
    constructor() {
        super("ping");
    }

    async handleCommand(argv: string[], dev: Device) {
        let res = await window.wPerformDeviceAction({
            action: "ping",
            destination: argv[1]
        });

        return JSON.stringify(res);
    }
}