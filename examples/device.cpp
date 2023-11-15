#include "../src/device/devices/end_device.h"
#include <cassert>
#include <iostream>

void arp() {
    EndDevice a, b;
    a.connect(&(EthernetInterface&)b);

    a.setIpAddress(SubnetAddress(IPv4Address("192.168.1.2"), 24));
    b.setIpAddress(SubnetAddress(IPv4Address("192.168.1.3"), 24));

    b.turnOff();
    a.sendARPRequest(IPv4Address("192.168.1.3"), false);

    assert(a.getARPCache().empty());

    b.turnOn();
    a.sendARPRequest(IPv4Address("192.168.1.3"), false);

    for (const auto& entry : a.getARPCache()) {
        assert(entry.first == "192.168.1.3");
    }   
}

void clone() {
    EndDevice a;
    a.setHostname("Bobita");
    a.setDefaultGateway(IPv4Address("192.168.1.1"));

    auto* b = dynamic_cast<EndDevice*>(a.clone());
    assert(b->getDefaultGateway() == a.getDefaultGateway()); 
    assert(b->getHostname() == a.getHostname());
    delete b;
}