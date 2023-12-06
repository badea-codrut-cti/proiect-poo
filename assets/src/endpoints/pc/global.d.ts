import type { CanvasDevice, CursorType } from "./lib/types/entity";
import type { ConnectTuple, Connections, Device, Devices } from "./lib/types/device";

declare global {
    interface Window {
        wGetDeviceData: () => Promise<Device>
    }
}