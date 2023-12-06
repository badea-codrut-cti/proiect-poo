import type { CanvasDevice, CursorType } from "./lib/types/entity";
import type { ConnectTuple, Connections, Device, Devices } from "./lib/types/device";

declare global {
    interface Window {
        wDeviceAdd: (device: CanvasDevice) => Promise<{success: boolean, device?: Device, error?: string}>,
        wDeviceConnect: (a: ConnectTuple, b: ConnectTuple) => Promise<{success: boolean, error?: string}>,
        wOpenDeviceSettings: ({deviceIndex: number, deviceType: Devices}) => Promise<void>,
        wToggleDeviceState: (deviceIndex: number) => Promise<{success: boolean, isOn?: boolean, error?: string}>
    }
}