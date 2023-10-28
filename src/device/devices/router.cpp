#include "./router.h"
#include "../../date/osi/network.h"
#include <stdexcept>
#include <typeinfo>

Router::Router(): Device(ROUTER_INT_COUNT, false, DEFAULT_ROUTER_HOSTNAME) {
    for (uint8_t i = 0; i < ROUTER_INT_COUNT; i++)
        adapter[i].setSpeed(1000);
}

IPv4Address Router::findRoute(const IPv4Address& add) {
    if (adapter.hasInterfaceInSubnet(add))
        return add;

    for (const auto& route: routes) {
        if (!route.first.isInSameSubnet(add)) 
            continue;
        
        return findRoute(route.second);
    }
    
    return {};
}

bool Router::interfaceCallback(DataLinkLayer& data, [[maybe_unused]] uint8_t fIndex) {
    try {
        auto frame = dynamic_cast<NetworkLayer&>(data);

        if (!adapter.findInterface(data.getMACDestination()))
            return false;
        
        if (!frame.getTTL())
            return false;

        IPv4Address dest = findRoute(frame.getIPDestination());
        if (dest == "127.0.0.1")
            return false;


        sendARPRequest(dest, false);  

        DataLinkLayer l2copy(data.getMACDestination(), getArpEntryOrBroadcast(dest), data.getPayload(), data.getL2Type());
        NetworkLayer l3copy(l2copy, frame.getIPSource(), frame.getIPDestination(), frame.getTTL());

        
        return adapter[adapter.findInSubnet(dest)].sendData(l3copy);
    } catch ([[maybe_unused]] std::bad_cast& e) {
        // Avem de a face cu trafic L2 
        return false;
    }
    
    return false;
}   

bool Router::addStaticRoute(const SubnetAddress& subnet, const IPv4Address& ip) {
    routes.insert(std::make_pair(subnet, ip));
    return true;
}