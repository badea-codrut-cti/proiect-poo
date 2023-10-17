#include "./ipv4.h"

#ifndef SUBNETADDR_H
#define SUBNETADDR_H

class SubnetAddress : public IPv4Address {
    private:
        uint8_t subnetMask;

    public:
        static uint8_t getClassSubnetMask(const IPv4Address&);

        SubnetAddress(const IPv4Address&, uint8_t);

        SubnetAddress(const IPv4Address&);

        SubnetAddress(const SubnetAddress&);

        SubnetAddress();

        [[nodiscard]] std::string toString() const;

        [[nodiscard]] IPv4Address getNetworkAddress() const;

        [[nodiscard]] IPv4Address getBroadcastAddress() const;

        [[nodiscard]] bool isMulticastAddress() const;

        [[nodiscard]] bool isInSameSubnet(SubnetAddress) const;

        SubnetAddress& operator=(const SubnetAddress&);

        friend std::ostream& operator<<(std::ostream&, const SubnetAddress&);
};

#endif
