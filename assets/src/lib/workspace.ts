export const pseudoDevice = {
    deviceIndex: 0,
    hostname: "Ceva",
    interfaces: [{
        ip: {
            address: "192.168.1.5",
            subnetMask: {
                dotNotation: "255.255.255.0",
                slashNotation: 24
            },
            defaultGateway: "192.168.1.1"
        },
        ipv6: {
            gua: [
                {
                    address: '2001:0db8:85a3:0000:0000:8a2e:0370:7334',
                    prefix: 64
                }
            ],
            defaultGateway: 'fe80::1',
            linkLocalAddress: 'fe80::20c:29ff:fe9c:4091/10'
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

if (import.meta.env.DEV) {
    window["wDeviceAdd"] = (device) => {
        return new Promise(res => { 
            res({success: true, device: pseudoDevice});
        });
    }

    window["wDeviceConnect"] = (a, b) => {
        return new Promise(res => {
            res({success: true});
        });
    }

    window["wGetDeviceData"] = () => {
        return new Promise(res => {
            res(pseudoDevice);
        }); 
    } 
}