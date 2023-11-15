export type UIDevice = {
    label: string,
    icon: string
};

export type UIDeviceType = {
    label: string,
    icon: string,
    devices: UIDevice[]
}

enum InterfaceTypes {
    ETHERNET,
    CONSOLE
}

export type Interface = {
    type: InterfaceTypes,
    name: string
};

export type Device = {
    name: string,
    isOn: boolean,
    
}