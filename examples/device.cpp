#include "../src/device/devices/end_device.h"
#include <cassert>
#include <stdexcept>

void arp() {
    EndDevice a, b;
    a.connect(&(EthernetInterface&)b);

    a.setIPv4Address(SubnetAddressV4(IPv4Address("192.168.1.2"), 24));
    b.setIPv4Address(SubnetAddressV4(IPv4Address("192.168.1.3"), 24));

    b.turnOff();
    a.sendARPRequest(IPv4Address("192.168.1.3"), false);

    assert(a.getARPCache().empty());

    b.turnOn();
    a.sendARPRequest(IPv4Address("192.168.1.3"), false);

    for (const auto& entry : a.getARPCache()) {
        assert(entry.first == IPv4Address("192.168.1.3"));
    }   

    try {
        a.getNetworkAdapter()[0].setMacAddress(MACAddress("01:00:5E:0A:00:02"));
        assert(false); // This should error out.
    } catch([[maybe_unused]] const std::invalid_argument&) {

    }
}

void clone() {
    EndDevice a;
    a.setHostname("Bobita");
    a.setDefaultGateway(IPv4Address("192.168.1.1"));

    auto* b = dynamic_cast<EndDevice*>(a.clone());
    assert(b->getIPv4DefaultGateway() == a.getIPv4DefaultGateway()); 
    assert(b->getHostname() == a.getHostname());
    delete b;
}