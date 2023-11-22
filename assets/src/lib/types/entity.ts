import type { Devices } from "./device";

export enum CursorType {
    SELECT,
    DELETE,
    TEXT,
    DEVICE,
    COPY,
    CONNECT,
    DEFAULT
};

export enum ObjectTypes {
    TEXTBOX = "textbox",
    DEVICE = "device",
    DEFAULT = "default"
};

export type CanvasObject = {
    id: number,
    type: ObjectTypes,
    // TODO: make these relative coordinates to canvas size (0 to 1)
    position:  {
        x: number,
        y: number
    }
};

export type CanvasDevice = {
    /** 
    *  Device name in the editor, not the hostname.
    */
    name: string 
} & ({
    /**
     * The index in the `devices` array. 
     * This should be used when copying a device for instance.
     */
    deviceIndex: number
} | {
    /**
     * This should be used when creating a new device for instance.
     */
    deviceType: Devices
})