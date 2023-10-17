#include "../macaddress.h"

#ifndef DATALINK_H
#define DATALINK_H

class DataLinkLayer {
    private:
        MACAddress source, destination;

    public:
        DataLinkLayer(const MACAddress&, const MACAddress&);

        DataLinkLayer(const DataLinkLayer&);

        virtual ~DataLinkLayer() = default;

        [[nodiscard]] MACAddress getMACSource() const;

        [[nodiscard]] MACAddress getMACDestination() const;

        friend std::ostream& operator<<(std::ostream&, const DataLinkLayer&);
};

#endif
