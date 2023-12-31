/*

#include "./network.h"
#include <iostream>

class TransportLayer: public NetworkLayer {
    private:
        uint16_t sourcePort, destinationPort;

    public:
        TransportLayer(NetworkLayer&, uint16_t, uint16_t);

        TransportLayer(const TransportLayer&);

        [[nodiscard]] uint16_t getSourcePort() const;

        [[nodiscard]] uint16_t getDestinationPort() const;

        [[nodiscard]] DataLinkLayer* clone() const override;

        friend std::ostream& operator<<(std::ostream&, const TransportLayer&);
};

*/