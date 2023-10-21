#include "./router.h"
#include "../../date/osi/network.h"
#include <stdexcept>
#include <typeinfo>

Router::Router(): Device(ROUTER_INT_COUNT) {
    for (uint8_t i = 0; i < ROUTER_INT_COUNT; i++)
        adapter[i].setSpeed(1000);
}

int Router::findRoute(const IPv4Address& add) {
    for (uint8_t i = 0; i < adapter.interfaceCount(); i++) {
        if (adapter[i].getAddress().isInSameSubnet(add))
            return i;
    }

    for (const auto& route: routes) {
        if (!route.first.isInSameSubnet(add)) 
            continue;
        
        try {
            int index = adapter.getIntefaceIndex(route.second);
            return index;
        } catch(const std::out_of_range& e) {
            return findRoute(route.second);
        }
    }
    
    return -1;
}

bool Router::interfaceCallback(DataLinkLayer& data, [[maybe_unused]] uint8_t fIndex) {
    try {
        auto frame = dynamic_cast<NetworkLayer&>(data);
        if (!frame.getTTL())
            return false;

        int index = findRoute(frame.getIPDestination());
        if (index == -1)
            return false;

        frame.age();
        adapter[index].sendData(frame);
    } catch (std::bad_cast& e) {
        // Avem de a face cu trafic L2 
        return false;
    }
    
    return false;
}   

bool Router::addStaticRoute(const SubnetAddress& subnet, const IPv4Address& ip) {
    routes.insert(std::make_pair(subnet, ip));
    return true;
}