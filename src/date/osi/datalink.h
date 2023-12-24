#include "../macaddress.h"
#include "../../protocoale/payload.h"

#ifndef DATALINK_H
#define DATALINK_H

class DataLinkLayer {
    public:
        // https://www.iana.org/assignments/ieee-802-numbers/ieee-802-numbers.xhtml
        // FIXME: This is an oversimplification, Dot1Q has multiple assigned numbers
        enum L2TypeField : uint16_t {
            IPV4         =  2048,
            ARP          =  2054,
            VLAN_CTAG    =  33024,
            DOT1X        =  34958,
            DOT1Q        =  34984,
            MPLS         =  33767,
            LLDP         =  35020,
            IPV6         =  34525
        };

    private:
        MACAddress source, destination;
        L2Payload* payload;
        L2TypeField l2type;

    public:
        DataLinkLayer(const MACAddress&, const MACAddress&, L2Payload&, L2TypeField);

        DataLinkLayer(const DataLinkLayer&);

        virtual ~DataLinkLayer();

        bool setMACSource(const MACAddress&);

        bool setMACDestination(const MACAddress&);

        [[nodiscard]] MACAddress getMACSource() const;

        [[nodiscard]] MACAddress getMACDestination() const;

        [[nodiscard]] const L2Payload* getPayload() const;

        [[nodiscard]] virtual DataLinkLayer* clone() const;

        [[nodiscard]] L2TypeField getL2Type() const;

        friend std::ostream& operator<<(std::ostream&, const DataLinkLayer&);
};

std::ostream& operator<<(std::ostream&, const DataLinkLayer::L2TypeField&);

#endif
