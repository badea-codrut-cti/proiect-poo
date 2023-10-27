#include "../device.h"

// "Daca mai intrii in help, usa de la intrare ti-e default gateway."
class EndDevice : public Device {
    private:
        IPv4Address defaultGateway;

    public:
        bool connect(EthernetInterface*);
        bool setIpAddress(const SubnetAddress&);
        bool setDefaultGateway(const IPv4Address&);
        bool sendData(DataLinkLayer&);

        [[nodiscard]] SubnetAddress getAddress() const;
        [[nodiscard]] IPv4Address getDefaultGateway() const;
};  