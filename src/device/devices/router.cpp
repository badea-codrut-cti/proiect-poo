#include "./router.h"
#include "../../date/osi/network.h"
#include <memory>
#include "../../protocoale/icmp.h"
#include <typeinfo>

Router::Router(): Device(ROUTER_INT_COUNT, false, DEFAULT_ROUTER_HOSTNAME) {
    for (uint8_t i = 0; i < ROUTER_INT_COUNT; i++)
        adapter[i].setSpeed(1000);
}

IPv4Address Router::findRoute(const IPv4Address& dest) {
    if (adapter.hasInterfaceInSubnet(dest))
        return dest;

    for (const auto& route: routes) {
        if (!route.first.isInSameSubnet(dest)) 
            continue;
        
        return findRoute(route.second);
    }
    
    return {};
}

bool Router::interfaceCallback(const DataLinkLayer& data, [[maybe_unused]] uint8_t fIndex) {
    try {
        if (!adapter.hasInterface(data.getMACDestination()))
            return false;

        auto& frame = dynamic_cast<const NetworkLayer&>(data);
        
        if (!frame.getTTL())
            return false;

        IPv4Address dest = findRoute(frame.getIPDestination());
        if (dest == IPv4Address("127.0.0.1")) {
            if (frame.getL3Protocol() == NetworkLayer::ICMP) {
                IPv4Address icmpReturn = findRoute(frame.getIPSource());
                if (icmpReturn == IPv4Address("127.0.0.1"))
                    return false;

                sendARPRequest(icmpReturn, false);

                ICMPPayload pl(ICMPPayload::DEST_UNREACHABLE, 0);
                DataLinkLayer l2(adapter[fIndex].getMacAddress(), getArpEntryOrBroadcast(icmpReturn), pl, DataLinkLayer::IPV4);
                NetworkLayer l3(l2, adapter[fIndex].getIPv4Address(), frame.getIPSource(), DEFAULT_TTL, NetworkLayer::ICMP);
                adapter[fIndex].sendData(l3);
            }
            return false;
        }

        sendARPRequest(dest, false);  

        auto pClone = frame.clone();
        pClone->age();

        EthernetInterface& sender = adapter[adapter.findInSubnet(dest)];

        pClone->setMACSource(sender.getMacAddress());
        pClone->setMACDestination(getArpEntryOrBroadcast(dest));

        std::unique_ptr<DataLinkLayer> clone(pClone);
        std::reference_wrapper<DataLinkLayer> ref = *clone;

        sender.sendData(ref);
        return true;
    } catch ([[maybe_unused]] const std::bad_cast&) {
        // Avem de a face cu trafic L2 
        return false;
    }
}   

bool Router::addStaticRoute(const SubnetAddressV4& subnet, const IPv4Address& ip) {
    routes.insert(std::make_pair(subnet, ip));
    return true;
}