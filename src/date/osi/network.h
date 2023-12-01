#include "../ipv4.h"
#include "./datalink.h"
#include <iostream>

#ifndef NETWORKLAYER_H
#define NETWORKLAYER_H

const uint8_t DEFAULT_TTL = 64;

class NetworkLayer: public DataLinkLayer {
    public:
        // https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
        enum IPProtocolType : uint8_t {
            ICMP    = 1,
            IPV4    = 4,
            TCP     = 6,
            UDP     = 17,
            GRE     = 47,
            ESP     = 50,
            AH      = 51,
            OSPF    = 89,
            SCTP    = 132
        };

    private:
        IPv4Address source, destination;
        uint8_t TTL;
        IPProtocolType proto;
        //uint16_t headerChecksum, identification, fragmentOffset, totalLength;

    public:
        NetworkLayer(DataLinkLayer&, const IPv4Address&, const IPv4Address&, uint8_t=DEFAULT_TTL, IPProtocolType=TCP);

        NetworkLayer(const NetworkLayer&);

        void age();

        [[nodiscard]] IPv4Address getIPSource() const;

        [[nodiscard]] IPv4Address getIPDestination() const;

        [[nodiscard]] uint8_t getTTL() const;

        [[nodiscard]] NetworkLayer* clone() const override;

        [[nodiscard]] IPProtocolType getL3Protocol() const;

        friend std::ostream& operator<<(std::ostream&, const NetworkLayer&);
};

std::ostream& operator<<(std::ostream&, const NetworkLayer::IPProtocolType&);

#endif