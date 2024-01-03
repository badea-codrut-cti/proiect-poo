import type { CanvasDevice, CursorType } from "./lib/types/entity";
import type { ConnectTuple, Connections, Device, Devices } from "./lib/types/device";

type devSettingMode = {
    editMode: "device",
    hostname?: string
} | {
    editMode: "interface",
    interfaceIndex: number,
    ip?: {
        address?: string,
        subnetMask?: {
            slashNotation?: number,
            dotNotation?: string
        }
    },
    isOn?: boolean,
    speed?: number,
    macAddress?: string
}

type devAction = {
    action: "ping" | "pingv6",
    destination: string
}

declare global {
    interface Window {
        wGetDeviceData: () => Promise<Device>,
        wChangeDeviceSettings: (dev: devSettingMode) => Promise<{success: boolean}>,
        wDeviceUpdateListener: () => void,
        wPerformDeviceAction: (devAction) => Promise<{success: boolean, result: any}>
    }
}