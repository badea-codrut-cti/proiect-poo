#include "../src/device/devices/end_device.h"
#include <cassert>
#include <iostream>

void arp() {
    EndDevice a, b;
    a.connect(&(EthernetInterface&)b);

    a.setIpAddress(SubnetAddress(IPv4Address("192.168.1.2"), 24));
    b.setIpAddress(SubnetAddress(IPv4Address("192.168.1.3"), 24));
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