#include "../macaddress.h"
#include "../../protocoale/payload.h"

#ifndef DATALINK_H
#define DATALINK_H

class DataLinkLayer {
    public:
        enum L2TypeField : uint16_t {
            IPV4    =  2048,
            ARP     =  2054,
            DOT1Q   =  33024,
            MPLS    =  33767,
            LLDP    =  35020,
            IPV6    =  34525
        };

    private:
        MACAddress source, destination;
        L2Payload& payload;
        L2TypeField l2type;

    public:
        DataLinkLayer(const MACAddress&, const MACAddress&, L2Payload&, L2TypeField);

        DataLinkLayer(const DataLinkLayer&);

        virtual ~DataLinkLayer();

        [[nodiscard]] MACAddress getMACSource() const;

        [[nodiscard]] MACAddress getMACDestination() const;

        [[nodiscard]] L2Payload& getPayload() const;

        [[nodiscard]] L2TypeField getL2Type() const;

        friend std::ostream& operator<<(std::ostream&, const DataLinkLayer&);
};

#endif
