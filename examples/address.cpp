#include "../src/date/macaddress.h"
#include "../src/device/devices/end_device.h"
#include "../src/protocoale/icmp.h"
#include "../src/date/osi/osiexcept.h"
#include "../src/date/ipv6.h"
#include <cassert>
#include <stdexcept>

void test_macaddress() {
    MACAddress mac("00:00:00:00:00:01");
    assert(mac == MACAddress("00:00:00:00:00:01"));

    mac += 1;
    assert (mac == MACAddress("00:00:00:00:00:02"));

    mac += 2196;
    assert (mac == MACAddress("00:00:00:00:08:96"));

    mac -= 2196;
    assert (mac == MACAddress("00:00:00:00:00:02"));

    try {
        MACAddress mac3("some:wrong:format");
        assert(false); // Last line should have errored out.
    } catch(...) {
        // Constructor threw an exception.
    }
}

void test_ipv4() {
    IPv4Address add1("10.10.10.1");
    
    SubnetAddressV4 add2(IPv4Address("10.10.10.2"), 22);

    assert(add2.getNetworkAddress() == IPv4Address("10.10.8.0"));
    assert(add2.getBroadcastAddress() == IPv4Address("10.10.11.255"));

    assert(add2.getMaskDotNotation() == IPv4Address("255.255.252.0"));
    assert(add2.isInSameSubnet(add1));

    // Automatic subnet mask notation from class
    SubnetAddressV4 add3(IPv4Address{"192.168.1.1"});
    assert(add3.getMaskDotNotation() == IPv4Address("255.255.255.0"));

    assert(add3.getSubnetMask() == 24);

    int slashMask = SubnetAddressV4::dotMaskToCIDR(IPv4Address("255.255.255.192"));
    assert(slashMask == 26);

    try {
        SubnetAddressV4::dotMaskToCIDR(IPv4Address("255.255.255.193"));
        assert(false);
    } catch(const std::invalid_argument&) {

    }
}

void test_ipv6() {
    IPv6Address add1("FE80::1");

    assert(
        add1.getOctets()[0] == 0xFE && 
        add1.getOctets()[1] == 0x80 &&
        add1.getOctets()[IPV6_SIZE-1] == 0x01);

    for (uint8_t i = 2; i < IPV6_SIZE-1; i++)
        assert(add1.getOctets()[i] == 0);

    assert(add1.toString() == "FE80::1");

    IPv6Address add2("AA:1::");

    assert(
        add2.getOctets()[0] == 0x00 &&
        add2.getOctets()[1] == 0xAA &&
        add2.getOctets()[2] == 0x00 && 
        add2.getOctets()[3] == 0x01
    );
    
    for (uint8_t i = 4; i < IPV6_SIZE; i++)
        assert(add2.getOctets()[i] == 0);

    assert(add2.toString() == "AA:1::");

    try {
        IPv6Address iptest("FE80::1::2");
        assert(false);
    } catch(const std::invalid_argument&) {}
}

void test_device() {
    EndDevice a, b;
    a.setIPv4Address(SubnetAddressV4(IPv4Address("192.168.1.1"), 24));
    b.setIPv4Address(SubnetAddressV4(IPv4Address("192.168.1.2"), 24));
    a.connect(&(EthernetInterface&) b);
    ICMPPayload pl(ICMPPayload::ECHO_REQUEST, 0);
    try {
        a.sendData(pl, IPv4Address("192.168.1.0"));
        assert(false);
    } catch(const InvalidPacketException&) {
        
    }
}