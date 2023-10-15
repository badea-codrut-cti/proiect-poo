#include "../ipv4.h"
#include "./datalink.h"

class NetworkLayer: public DataLinkLayer {
    private:
        IPv4Address source, destination;
        unsigned long TTL;

    public:
        NetworkLayer(DataLinkLayer&, const IPv4Address&, const IPv4Address&, unsigned long _ttl);

        NetworkLayer(const NetworkLayer&);

        IPv4Address getIPSource() const;

        IPv4Address getIPDestination() const;

        friend std::ostream& operator<<(std::ostream&, const NetworkLayer&);
};