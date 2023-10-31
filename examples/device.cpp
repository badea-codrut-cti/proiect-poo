#include "../src/device/devices/end_device.h"
#include <cassert>

void arp() {
    EndDevice a, b;
    a.connect(&b.getNetworkAdapter()[0]);

    a.setIpAddress(SubnetAddress(IPv4Address("192.168.1.2"), 24));
    b.setIpAddress(SubnetAddress(IPv4Address("192.168.1.3"), 24));

    std::string new_mac = "01:02:03:04:05:06";
    a.turnOff();
    a.getNetworkAdapter()[0].setMacAddress(MACAddress{new_mac});
    // Won't work if device is turned off
    assert(a.getNetworkAdapter()[0].getMacAddress() != MACAddress{new_mac});

    a.turnOn();
    a.getNetworkAdapter()[0].setMacAddress(MACAddress{new_mac});
    assert(a.getNetworkAdapter()[0].getMacAddress() == MACAddress{new_mac});

    a.sendARPRequest(IPv4Address("192.168.1.3"), false);
}