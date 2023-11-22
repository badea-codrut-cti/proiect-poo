import { Connections, Devices } from "../../lib/types/device";

export type UIDevice = {
    label: string,
    icon: string,
    device: Devices
};

export type UIDeviceCategory = {
    label: string,
    icon: string,
    devices: UIDevice[]
}; 

export const deviceTypes: UIDeviceCategory[] = [
    {
        label: "Network Devices",
        icon: "router",
        devices: [
            {
                label: "Router",
                icon: "router",
                device: Devices.ROUTER
            },
            {
                label: "Switch",
                icon: "l2switch",
                device: Devices.L2SWITCH
            },
            {
                label: "Wireless<br>Router",
                icon: "wireless-router",
                // TODO: Implement Wireless routers
                device: Devices.ROUTER
            }
        ]
    },
    {
        label: "End Devices",
        icon: "pc",
        devices: [
            {
                label: "PC",
                icon: "pc",
                device: Devices.DESKTOP
            },
            {
                label: "Laptop",
                icon: "laptop",
                // TODO: Implement Laptops
                device: Devices.DESKTOP
            }
        ]
    }
];

export const connectionTypes = [
    {
        label: "Console",
        icon: "console",
        connection: Connections.CONSOLE
    },
    {
        label: "Copper Straight-Through",
        icon: "straight-through",
        connection: Connections.STRAIGHT_THROUGH
    },
    {
        label: "Copper Cross-Over",
        icon: "crossover",
        connection: Connections.CROSSOVER
    }
];