#include <cstring>
#include <iostream>
#include "src/device/device.h"
#include "src/device/devices/l2switch.h"
#include "src/date/subnetaddress.h"
#include "src/date/osi/network.h"
#include "src/protocoale/arp.h"
#include "src/protocoale/payload.h"
#include "src/device/devices/router.h"
#include "src/device/devices/end_device.h"

using namespace std;

int main() {
    EndDevice brasov, bon;
    L2Switch sw_brasov, sw_bon;
    Router r_brasov, r_bon;
    brasov.setIpAddress(SubnetAddress(IPv4Address{"173.32.32.81"}, 21));
    brasov.setDefaultGateway(IPv4Address{"173.32.32.1"});
    brasov.connect(&sw_brasov.getNetworkAdapter()[0]);

    sw_brasov.getNetworkAdapter()[1].connect(&r_brasov.getNetworkAdapter()[0]);
    r_brasov.getNetworkAdapter()[0].setIpAddress(SubnetAddress(IPv4Address{"173.32.32.1"}, 21));
    brasov.sendARPRequest(brasov.getDefaultGateway());

    r_brasov.getNetworkAdapter()[1].setIpAddress(SubnetAddress(IPv4Address{"10.10.10.5"}, 30));
    r_bon.getNetworkAdapter()[1].setIpAddress(SubnetAddress(IPv4Address{"10.10.10.6"}, 30));

    r_brasov.addStaticRoute(SubnetAddress(IPv4Address{"188.26.2.0"}, 24), IPv4Address{"10.10.10.6"});
    r_bon.addStaticRoute(SubnetAddress(IPv4Address{"173.32.32.0"}, 21), IPv4Address{"10.10.10.5"});

    bon.setIpAddress(SubnetAddress(IPv4Address{"188.26.2.35"}, 24));
    bon.setDefaultGateway(SubnetAddress(IPv4Address{"188.26.2.1"}, 24));
    bon.connect(&sw_bon.getNetworkAdapter()[0]);

    sw_bon.getNetworkAdapter()[1].connect(&r_bon.getNetworkAdapter()[0]);
    r_bon.getNetworkAdapter()[0].setIpAddress(SubnetAddress(IPv4Address{"188.26.2.1"}, 24));
    bon.sendARPRequest(bon.getDefaultGateway());
    return 0;
}