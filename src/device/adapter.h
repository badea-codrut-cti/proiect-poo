#include <cstdint>
#include <functional>
#include "./ethernet.h"

#ifndef ADAPTER_H
#define ADAPTER_H

class Device;

class NetworkAdapter {
    private:
        size_t numInterfaces;
        EthernetInterface** interfaces;

        void allocate();

    public:
        NetworkAdapter(Device&, size_t, bool=false);

        NetworkAdapter(Device&, size_t, std::function<EthernetInterface*(Device&, size_t)>);

        NetworkAdapter(const NetworkAdapter&) = delete;

        NetworkAdapter& operator=(const NetworkAdapter&) = delete;

        bool copy(Device&, const NetworkAdapter&);

        ~NetworkAdapter();

        [[nodiscard]] size_t interfaceCount() const;

        [[nodiscard]] size_t getIntefaceIndex(const MACAddress&) const;
        [[nodiscard]] size_t getIntefaceIndex(const IPv4Address&) const;
        [[nodiscard]] size_t getIntefaceIndex(const IPv6Address&) const;

        [[nodiscard]] size_t findInSubnet(const IPv4Address&) const;
        [[nodiscard]] size_t findInSubnet(const IPv6Address&) const;

        [[nodiscard]] bool hasInterface(const MACAddress&) const;
        [[nodiscard]] bool hasInterface(const IPv4Address&) const;
        [[nodiscard]] bool hasInterface(const IPv6Address&) const;

        [[nodiscard]] bool hasInterfaceInSubnet(const IPv4Address&) const;
        [[nodiscard]] bool hasInterfaceInSubnet(const IPv6Address&) const;

        EthernetInterface& operator[](size_t index) const;
        
        friend std::ostream& operator<<(std::ostream& o, const NetworkAdapter&);
};

#endif