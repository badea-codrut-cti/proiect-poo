#include "../date/ipv4.h"
#include "../date/ipv6.h"
#include "../date/osi/datalink.h"

#ifndef ETHERNET_H
#define ETHERNET_H
    
class Device;

const unsigned long REGULAR_ETHERNET = 10;
const unsigned long FAST_ETHERNET = 100;
const unsigned long GIGABIT_ETHERNET = 1000;

class EthernetInterface {
    private:
        static MACAddress publicMACCounter;

        Device& device;
        EthernetInterface* link{nullptr};

        SubnetAddressV4 addressV4;

        MACAddress macAddress;
        const MACAddress burnAddress;
        bool isOn{true}, unnumbered;

        bool receiveData(const DataLinkLayer&);

        unsigned long long speed, maxSpeed, bandwidth;

        // TODO: arp type, cdp, crypto, delay, descriere
        // queue, MTU, pppoe, QoS, HSRP

    public:
        explicit EthernetInterface(Device&, unsigned long long, bool);

        EthernetInterface(const EthernetInterface&) = delete;

        EthernetInterface& operator=(const EthernetInterface&) = delete;

        EthernetInterface* copy(Device&) const;

        ~EthernetInterface();

        bool connect(EthernetInterface*);

        bool setIpAddress(const SubnetAddressV4&);

        bool setMacAddress(const MACAddress&);

        bool setSpeed(unsigned long);

        bool turnOn();
        
        bool turnOff();

        bool sendData(const DataLinkLayer&);

        bool disconnect();

        [[nodiscard]] unsigned long long getMaxSpeed() const;

        [[nodiscard]] unsigned long long getSpeed() const;

        [[nodiscard]] MACAddress getMacAddress() const;

        [[nodiscard]] MACAddress getBurntInAddress() const;

        [[nodiscard]] bool getState() const;

        [[nodiscard]] SubnetAddressV4 getIPv4Address() const;

        [[nodiscard]] bool isUnnumbered() const;

        friend std::ostream& operator<<(std::ostream&, const EthernetInterface&);
};

#endif