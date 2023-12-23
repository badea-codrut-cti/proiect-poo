#include "../device.h"

// "Daca mai intrii in help, usa de la intrare ti-e default gateway."
class EndDevice : public Device {
    public:
        EndDevice();
        EndDevice(const EndDevice&);

        bool connect(EthernetInterface*);

        bool setDefaultGateway(const IPv4Address&);
        bool setDefaultGateway(const IPv6Address&);

        bool setIPv4Address(const SubnetAddressV4&);

        bool sendData(L2Payload&, const IPv4Address& = IPv4Address("127.0.0.1"));
        bool sendPing(const IPv4Address&);

        explicit operator EthernetInterface&();

        [[nodiscard]] Device* clone() const override;

        [[nodiscard]] SubnetAddressV4 getIPv4Address() const;

        [[nodiscard]] IPv4Address getIPv4DefaultGateway() const;
        [[nodiscard]] IPv4Address getIPv6DefaultGateway() const;
};  