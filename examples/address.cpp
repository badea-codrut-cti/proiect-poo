#include "../src/date/macaddress.h"
#include "../src/date/subnetaddress.h"
#include "../src/device/devices/end_device.h"
#include "../src/protocoale/icmp.h"
#include "../src/date/osi/osiexcept.h"
#include <cassert>
#include <stdexcept>

void test_macaddress() {
    MACAddress mac("00:00:00:00:00:01");
    assert(mac == "00:00:00:00:00:01");

    mac += 1;
    assert (mac == "00:00:00:00:00:02");

    mac += 2196;
    assert (mac == "00:00:00:00:08:96");

    mac -= 2196;
    assert (mac == "00:00:00:00:00:02");

    try {
        MACAddress mac3("some:wrong:format");
        assert(false); // Last line should have errored out.
    } catch(...) {
        // Constructor threw an exception.
    }
}

void test_ipv4() {
    IPv4Address add1("10.10.10.1");
    SubnetAddress add2(IPv4Address{"10.10.10.2"}, 22);

    assert(add2.getNetworkAddress() == "10.10.8.0");
    assert(add2.getBroadcastAddress() == "10.10.11.255");

    assert(add2.getMaskDotNotation() == "255.255.252.0");
    assert(add2.isInSameSubnet(add1));

    // Automatic subnet mask notation from class
    SubnetAddress add3(IPv4Address{"192.168.1.1"});
    assert(add3.getMaskDotNotation() == "255.255.255.0");

    assert(add3.getMaskSlashNotation() == 24);

    assert(SubnetAddress::dotMaskToCIDR(IPv4Address("255.255.255.192")) == 26);

    try {
        SubnetAddress::dotMaskToCIDR(IPv4Address("255.255.255.193"));
        assert(false);
    } catch(const std::invalid_argument&) {

    }
}

void test_device() {
    EndDevice a, b;
    a.setIpAddress(SubnetAddress(IPv4Address("192.168.1.1"), 24));
    b.setIpAddress(SubnetAddress(IPv4Address("192.168.1.2"), 24));
    a.connect(&(EthernetInterface&) b);
    ICMPPayload pl(ICMPPayload::ECHO_REQUEST, 0);
    try {
        a.sendData(pl, IPv4Address("192.168.1.0"));
        assert(false);
    } catch(const InvalidPacketException&) {
        
    }
}