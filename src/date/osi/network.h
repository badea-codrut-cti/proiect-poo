#include "../ipv4.h"
#include "../ipv6.h"
#include "./datalink.h"
#include <iostream>

#ifndef NETWORKLAYER_H
#define NETWORKLAYER_H

const uint8_t DEFAULT_TTL = 64;

class NetworkLayerV4: public DataLinkLayer {
    public:
        // https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
        enum IPv4ProtocolType : uint8_t {
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
        IPv4ProtocolType proto;
        //uint16_t headerChecksum, identification, fragmentOffset, totalLength;

    public:
        NetworkLayerV4(DataLinkLayer&, const IPv4Address&, const IPv4Address&, uint8_t=DEFAULT_TTL, IPv4ProtocolType=TCP);

        NetworkLayerV4(const NetworkLayerV4&);

        void age();

        [[nodiscard]] IPv4Address getIPSource() const;

        [[nodiscard]] IPv4Address getIPDestination() const;

        [[nodiscard]] uint8_t getTTL() const;

        [[nodiscard]] NetworkLayerV4* clone() const override;

        [[nodiscard]] IPv4ProtocolType getL3Protocol() const;

        friend std::ostream& operator<<(std::ostream&, const NetworkLayerV4&);
};

const uint8_t DEFAULT_HOP_LIMIT = 64;
class NetworkLayerV6: public DataLinkLayer {
    public:
        // https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml
        enum IPv6ProtocolType : uint8_t {
            ICMPv6  = 58,
            IPV6    = 41,
            TCP     = 6,
            UDP     = 17
        };

    private:
        IPv6Address source, destination;
        uint8_t hopLimit;
        IPv6ProtocolType proto;
        //uint16_t headerChecksum, identification, fragmentOffset, payloadLength;

    public:
        NetworkLayerV6(DataLinkLayer&, const IPv6Address&, const IPv6Address&, uint8_t=DEFAULT_HOP_LIMIT, IPv6ProtocolType=TCP);

        NetworkLayerV6(const NetworkLayerV6&);

        void age();

        [[nodiscard]] IPv6Address getIPSource() const;

        [[nodiscard]] IPv6Address getIPDestination() const;

        [[nodiscard]] uint8_t getHopLimit() const;

        [[nodiscard]] NetworkLayerV6* clone() const override;

        [[nodiscard]] IPv6ProtocolType getL3Protocol() const;

        friend std::ostream& operator<<(std::ostream&, const NetworkLayerV6&);
};

std::ostream& operator<<(std::ostream&, const NetworkLayerV4::IPv4ProtocolType&);

#endif