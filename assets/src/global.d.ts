import type { CanvasDevice, CursorType } from "./lib/types/entity";
import type { Connections, Device } from "./lib/types/device";

declare global {
    interface Window {
        wDeviceAdd: (device: CanvasDevice) => Promise<{success: boolean, device?: Device, error?: string}>
    }
}