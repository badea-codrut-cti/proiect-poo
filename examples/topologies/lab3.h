#include "../../src/device/devices/end_device.h"
#include "../../src/device/devices/l2switch.h"
#include "../../src/device/devices/router.h"
#include "../../src/date/osi/network.h"
#include "../../src/protocoale/icmp.h"

bool onICMPReceive(const DataLinkLayer& data, const MACAddress&) {
    if (data.getL2Type() != DataLinkLayer::IPV4)
        return false;

    try {
        auto& packet = dynamic_cast<const NetworkLayerV4&>(data);
        if (packet.getL3Protocol() != NetworkLayerV4::ICMP)
            return false;

        auto payload = dynamic_cast<const ICMPPayload*>(packet.getPayload());

        std::cout << "Bon received reply " << (int) payload->getType() << " with code " << (int) payload->getCode() << "\n";
        return true;
    } catch(const std::bad_cast&) {
        return false;
    }
}

void lab3() {
    EndDevice brasov, bon;
    L2Switch sw_brasov, sw_bon;
    Router r_brasov, r_bon;
    brasov.setIPv4Address(SubnetAddressV4(IPv4Address("173.32.32.81"), 21));
    brasov.setDefaultGateway(IPv4Address("173.32.32.1"));
    brasov.connect(&sw_brasov.getNetworkAdapter()[0]);
    brasov.setHostname("Brasov");

    sw_brasov.getNetworkAdapter()[1].connect(&r_brasov.getNetworkAdapter()[0]);
    sw_brasov.setHostname("SW-BRASOV");
    r_brasov.getNetworkAdapter()[0].setIpAddress(SubnetAddressV4(IPv4Address("173.32.32.1"), 21));
    r_brasov.setHostname("R-BRASOV");

    r_brasov.getNetworkAdapter()[1].setIpAddress(SubnetAddressV4(IPv4Address("10.10.10.5"), 30));
    r_bon.getNetworkAdapter()[1].setIpAddress(SubnetAddressV4(IPv4Address("10.10.10.6"), 30));
    r_bon.setHostname("R-BON");

    r_brasov.getNetworkAdapter()[1].connect(&r_bon.getNetworkAdapter()[1]);

    r_brasov.addStaticRoute(SubnetAddressV4(IPv4Address("188.26.2.0"), 24), IPv4Address("10.10.10.6"));
    r_bon.addStaticRoute(SubnetAddressV4(IPv4Address("173.32.32.0"), 21), IPv4Address("10.10.10.5"));

    bon.setIPv4Address(SubnetAddressV4(IPv4Address("188.26.2.35"), 24));
    bon.setDefaultGateway(IPv4Address("188.26.2.1"));
    bon.connect(&sw_bon.getNetworkAdapter()[0]);
    bon.setHostname("Bon");

    sw_bon.getNetworkAdapter()[1].connect(&r_bon.getNetworkAdapter()[0]);
    sw_bon.setHostname("SW-BON");
    r_bon.getNetworkAdapter()[0].setIpAddress(SubnetAddressV4(IPv4Address("188.26.2.1"), 24));

    bon.registerFuncListener(onICMPReceive);

    bon.sendPing(brasov.getIPv4Address());
}