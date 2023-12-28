export enum Devices {
    ROUTER = "router",
    L2SWITCH = "l2switch",
    DESKTOP = "pc"
};

export enum Connections {
    STRAIGHT_THROUGH = "straight_through",
    CROSSOVER = "crossover",
    CONSOLE = "console",
}

export type EthernetInterface = {
    ip: {
        address: string,
        subnetMask: {
            dotNotation: string,
            slashNotation: number
        },
        defaultGateway: string
    },
    ipv6: {
        gua: {
            address: string,
            prefix: number
        }[],
        defaultGateway: string,
        linkLocalAddress: string
    },
    isOn: boolean,
    isUnnumbered: boolean,
    macAddress: string,
    /**
     * The burn-in address to fall back to in case the MAC address was overridden and reset.
     * The logic is still handled on the backend, however. 
     * For the most part, this information is redundant on the frontend.
     */
    hardwareAddress: string, 
    maxSpeed: number,
    speed: number
};

export type Device = {
    deviceIndex: number,
    hostname: string,
    interfaces: EthernetInterface[],
    isOn: boolean
};

export type ConnectTuple = {
    deviceIndex: number,
    interfaceIndex: number
}