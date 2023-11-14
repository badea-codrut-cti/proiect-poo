#include "./router.h"
#include "../../date/osi/network.h"
#include <memory>
#include <stdexcept>
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

        auto frame = dynamic_cast<const NetworkLayer&>(data);
        
        if (!frame.getTTL())
            return false;

        IPv4Address dest = findRoute(frame.getIPDestination());
        if (dest == "127.0.0.1")
            return false;

        sendARPRequest(dest, false);  

        std::unique_ptr<DataLinkLayer> clone(data.clone());
        std::reference_wrapper<DataLinkLayer> ref = *clone;

        return adapter[adapter.findInSubnet(dest)].sendData(ref);
    } catch ([[maybe_unused]] std::bad_cast& e) {
        // Avem de a face cu trafic L2 
        return false;
    }
}   

bool Router::addStaticRoute(const SubnetAddress& subnet, const IPv4Address& ip) {
    routes.insert(std::make_pair(subnet, ip));
    return true;
}