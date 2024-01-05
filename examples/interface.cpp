#include "../src/device/device.h"
#include <cassert>
#include <stdexcept>

void ethernet_interface() {
    Device dev;
    EthernetInterface& inter = dev.getNetworkAdapter()[0];

    assert(inter.getMacAddress() == inter.getBurntInAddress());

    try {
        inter.setMacAddress(MACAddress("11:22:33:44:55:66")); // This is multicast
        assert(false);
    } catch(const std::invalid_argument&) {
        // It failed as expected
    }

    inter.setMacAddress(MACAddress("00:00:00:00:00:1f"));

    assert(inter.getMacAddress() != inter.getBurntInAddress());

    try {
        dev.getNetworkAdapter()[0].setSpeed(100000);
        assert(false);
    } catch (const std::invalid_argument&) {

    }

    assert(dev.getNetworkAdapter()[0].getSpeed() == dev.getNetworkAdapter()[0].getMaxSpeed());

    dev.getNetworkAdapter()[0]
    .addGlobalUnicastAddress(SubnetAddressV6(IPv6Address("2001:ABCD:ACAD::1"), 64));

    dev.getNetworkAdapter()[0]
    .removeGlobalUnicastAddress(SubnetAddressV6(IPv6Address("2001:ABCD:ACAD::1"), 64));
}