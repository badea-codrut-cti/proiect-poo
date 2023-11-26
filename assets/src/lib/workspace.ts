if (import.meta.env.DEV) {
    window["wDeviceAdd"] = (device) => {
        return new Promise(res => 
        { 
            res({
                success: true,
                device: {
                    deviceIndex: 0,
                    hostname: "Ceva",
                    interfaces: [{
                        ip: {
                            address: "192.168.1.5",
                            subnetMask: {
                                dotNotation: "255.255.255.0",
                                slashNotation: 24
                            }
                        },
                        isOn: true,
                        isUnnumbered: false, 
                        macAddress: "00:00:00:00:00:05",
                        hardwareAddress: "00:00:00:00:00:01",
                        speed: 100,
                        maxSpeed: 1000
                    }],
                    isOn: true
                }
            });
        });
    }

    window["wDeviceConnect"] = (a, b) => {
        return new Promise(res => {
            res({success: true});
        });
    }
}