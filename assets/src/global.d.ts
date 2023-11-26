import type { CanvasDevice, CursorType } from "./lib/types/entity";
import type { ConnectTuple, Connections, Device } from "./lib/types/device";

declare global {
    interface Window {
        wDeviceAdd: (device: CanvasDevice) => Promise<{success: boolean, device?: Device, error?: string}>,
        wDeviceConnect: (a: ConnectTuple, b: ConnectTuple) => Promise<{success: boolean, error?: string}>
    }
}