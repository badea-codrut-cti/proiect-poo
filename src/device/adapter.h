#include <cstdint>
#include <functional>
#include "./ethernet.h"

#ifndef ADAPTER_H
#define ADAPTER_H

class Device;

class NetworkAdapter {
    private:
        uint8_t numInterfaces;
        EthernetInterface** interfaces;

        void allocate();

    public:
        NetworkAdapter(Device&, uint8_t, bool=false);

        NetworkAdapter(Device&, uint8_t, std::function<EthernetInterface*(Device&, uint8_t)>);

        NetworkAdapter(const NetworkAdapter&) = delete;

        NetworkAdapter& operator=(const NetworkAdapter&) = delete;

        bool copy(Device&, const NetworkAdapter&);

        ~NetworkAdapter();

        [[nodiscard]] uint8_t interfaceCount() const;

        [[nodiscard]] uint8_t getIntefaceIndex(const MACAddress&) const;

        [[nodiscard]] uint8_t getIntefaceIndex(const IPv4Address&) const;

        [[nodiscard]] uint8_t findInSubnet(const IPv4Address&) const;

        [[nodiscard]] bool hasInterface(const MACAddress&) const;

        [[nodiscard]] bool hasInterface(const IPv4Address&) const;

        [[nodiscard]] bool hasInterfaceInSubnet(const IPv4Address&) const;

        EthernetInterface& operator[](uint8_t index) const;
        
        friend std::ostream& operator<<(std::ostream& o, const NetworkAdapter&);
};

#endif