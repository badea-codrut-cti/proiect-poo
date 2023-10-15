#include "../macaddress.h"

#ifndef DATALINK_H
#define DATALINK_H

class PhysicalLayer {
    public:
        virtual ~PhysicalLayer() {}
};

class DataLinkLayer: public PhysicalLayer {
    private:
        MACAddress source, destination;

    public:
        DataLinkLayer(const MACAddress&, const MACAddress&);

        DataLinkLayer(const DataLinkLayer&);

        MACAddress getMACSource() const;

        MACAddress getMACDestination() const;

        friend std::ostream& operator<<(std::ostream&, const DataLinkLayer&);
};

#endif