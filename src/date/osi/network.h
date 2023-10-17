#include "../ipv4.h"
#include "./datalink.h"

class NetworkLayer: public DataLinkLayer {
    private:
        IPv4Address source, destination;
        unsigned long TTL;

    public:
        NetworkLayer(DataLinkLayer&, const IPv4Address&, const IPv4Address&, unsigned long _ttl);

        NetworkLayer(const NetworkLayer&);

        [[nodiscard]] IPv4Address getIPSource() const;

        [[nodiscard]] IPv4Address getIPDestination() const;

        [[nodiscard]] unsigned long getTTL() const;

        friend std::ostream& operator<<(std::ostream&, const NetworkLayer&);
};