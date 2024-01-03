#include "./router.h"
#include "../../date/osi/network.h"
#include <memory>
#include "../../protocoale/icmp.h"
#include <stdexcept>
#include <typeinfo>

Router::Router(): Device(ROUTER_INT_COUNT, false, DEFAULT_ROUTER_HOSTNAME) {
    for (uint8_t i = 0; i < ROUTER_INT_COUNT; i++)
        adapter[i].setSpeed(1000);
}

IPv4Address Router::findRoute(const IPv4Address& dest) const {
    if (adapter.hasInterfaceInSubnet(dest))
        return dest;

    for (const auto& route: routes) {
        if (!route.first.isInSameSubnet(dest)) 
            continue;
        
        return findRoute(route.second);
    }
    
    return IPv4Address("127.0.0.1");
}

IPv6Address Router::findRoute(const IPv6Address& dest) const {
    if (adapter.hasInterfaceInSubnet(dest))
        return dest;

    for (const auto& route: routesV6) {
        if (!route.first.isInSameSubnet(dest)) 
            continue;
        
        return findRoute(route.second);
    }
    
    return IPv6Address("0::1");
}

bool Router::interfaceCallback(const DataLinkLayer& data, [[maybe_unused]] uint8_t fIndex) {
    if (!adapter.hasInterface(data.getMACDestination()) && data.getMACDestination() != MACAddress(MACAddress::broadcastAddress))
        return false;

    if (data.getL2Type() == DataLinkLayer::IPV6) {
        try {
            auto& frame = dynamic_cast<const NetworkLayerV6&>(data);

            if (!frame.getHopLimit())
                return false;

            if (frame.getIPSource().isLinkLocalAddress())
                return false;

            IPv6Address dest = findRoute(frame.getIPDestination());

            if (dest == IPv6Address("0::1")) {
                if (frame.getL3Protocol() == NetworkLayerV6::ICMPv6) {
                    IPv6Address icmpReturn = findRoute(frame.getIPSource());
                    if (icmpReturn == IPv6Address("0::1"))
                        return false;

                    sendNDPRequest(icmpReturn, false);

                    ICMPPayload pl(ICMPPayload::DEST_UNREACHABLE, 0);
                    DataLinkLayer l2(adapter[fIndex].getMacAddress(), getNDPEntryOrMulticast(icmpReturn), pl, DataLinkLayer::IPV6);
                    NetworkLayerV6 l3(l2, adapter[fIndex].getGlobalUnicastAddresses()[0], frame.getIPSource(), DEFAULT_TTL, NetworkLayerV6::ICMPv6);
                    adapter[fIndex].sendData(l3);
                }
                return false;
            }

            sendNDPRequest(dest, false);

            auto pClone = frame.clone();

            pClone->age();

            EthernetInterface& sender = adapter[adapter.findInSubnet(dest)];

            pClone->setMACSource(sender.getMacAddress());
            pClone->setMACDestination(getNDPEntryOrMulticast(dest));

            std::unique_ptr<DataLinkLayer> clone(pClone);
            std::reference_wrapper<DataLinkLayer> ref = *clone;

            sender.sendData(ref);
            return true;
        } catch(const std::bad_cast&) {
            return false;
        }
    } else {
        try {
            auto& frame = dynamic_cast<const NetworkLayerV4&>(data);
            
            if (!frame.getTTL())
                return false;

            IPv4Address dest = findRoute(frame.getIPDestination());
            
            if (dest == IPv4Address("127.0.0.1")) {
                if (frame.getL3Protocol() == NetworkLayerV4::ICMP) {
                    IPv4Address icmpReturn = findRoute(frame.getIPSource());
                    if (icmpReturn == IPv4Address("127.0.0.1"))
                        return false;

                    sendARPRequest(icmpReturn, false);

                    ICMPPayload pl(ICMPPayload::DEST_UNREACHABLE, 0);
                    DataLinkLayer l2(adapter[fIndex].getMacAddress(), getArpEntryOrBroadcast(icmpReturn), pl, DataLinkLayer::IPV4);
                    NetworkLayerV4 l3(l2, adapter[fIndex].getIPv4Address(), frame.getIPSource(), DEFAULT_TTL, NetworkLayerV4::ICMP);
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
    
}   

bool Router::addStaticRoute(const SubnetAddressV4& subnet, const IPv4Address& ip) {
    if (subnet.getNetworkAddress() != subnet)
        throw std::invalid_argument("Route should point to the network address.");

    routes.insert(std::make_pair(subnet, ip));
    return true;
}

bool Router::addStaticRoute(const SubnetAddressV6& subnet, const IPv6Address& ip) {
    if (ip.isLinkLocalAddress() || ip.isLoopbackAddress() || ip.isMulticastAddress())
        throw std::invalid_argument("Route should point to a global unicast address.");

    routesV6.insert(std::make_pair(subnet, ip));
    return true;
}

bool Router::sendICMPRequest(const IPv4Address& target) {
    IPv4Address dest = findRoute(target);

    if (dest == IPv4Address("127.0.0.1"))
        return false;

    EthernetInterface& sender = adapter[adapter.findInSubnet(dest)];

    ICMPPayload pl(ICMPPayload::ECHO_REQUEST, 0);

    DataLinkLayer l2(sender.getMacAddress(), getArpEntryOrBroadcast(dest), pl, DataLinkLayer::IPV4);

    NetworkLayerV4 l3(l2, sender.getIPv4Address(), target, DEFAULT_TTL, NetworkLayerV4::ICMP);
    return sender.sendData(l3);
}