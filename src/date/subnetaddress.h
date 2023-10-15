#include "./ipv4.h"

#ifndef SUBNETADDRESS_H
#define SUBNETADDRESS_H

class SubnetAddress : public IPv4Address {
    private:
        uint8_t subnetMask;

    public:
        static uint8_t getClassSubnetMask(const IPv4Address&);

        SubnetAddress(const IPv4Address&, uint8_t);

        SubnetAddress(const IPv4Address&);

        SubnetAddress(const SubnetAddress&);

        SubnetAddress();

        std::string toString() const;

        IPv4Address getNetworkAddress() const;

        IPv4Address getBroadcastAddress() const;

        bool isMulticastAddress() const;

        bool isInSameSubnet(SubnetAddress) const;

        friend std::ostream& operator<<(std::ostream&, const SubnetAddress&);
};

#endif