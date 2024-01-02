#include "../device.h"

#ifndef ROUTER_H
#define ROUTER_H

const uint8_t ROUTER_INT_COUNT = 4;

const std::string DEFAULT_ROUTER_HOSTNAME = "Router";

class Router: public Device {
    private:
        std::map<SubnetAddressV4, IPv4Address> routes{};
        std::map<SubnetAddressV6, IPv6Address> routesV6{};

        [[nodiscard]] IPv4Address findRoute(const IPv4Address&) const;
        [[nodiscard]] IPv6Address findRoute(const IPv6Address&) const;

    public:
        Router();

        bool sendICMPRequest(const IPv4Address&) override;

        bool interfaceCallback(const DataLinkLayer&, uint8_t) override;

        bool addStaticRoute(const SubnetAddressV4&, const IPv4Address&);
        bool addStaticRoute(const SubnetAddressV6&, const IPv6Address&);

        bool removeStaticRoute(const SubnetAddressV4&, const IPv4Address&);
        bool removeStaticRoute(const SubnetAddressV6&, const IPv6Address&);
};

#endif