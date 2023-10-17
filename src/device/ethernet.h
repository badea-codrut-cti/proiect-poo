#include "../date/subnetaddress.h"
#include "../date/osi/datalink.h"

#ifndef ETHERNET_H
#define ETHERNET_H

enum Duplex {
    FULL, HALF, AUTO
};
    
class Device;

const unsigned long REGULAR_ETHERNET = 10;
const unsigned long FAST_ETHERNET = 100;
const unsigned long GIGABIT_ETHERNET = 1000;

class EthernetInterface {
    private:
        static MACAddress publicMACCounter;

        Device& device;
        EthernetInterface* link{nullptr};

        SubnetAddress address;
        MACAddress macAddress;
        bool isOn{false};
        Duplex duplex{AUTO};

        bool receiveData(DataLinkLayer&);

        unsigned long speed{FAST_ETHERNET}, bandwidth{FAST_ETHERNET};

        // TODO: arp type, cdp, crypto, delay, descriere
        // queue, MTU, pppoe, QoS, HSRP

    public:
        explicit EthernetInterface(Device&);

        ~EthernetInterface();

        bool connect(EthernetInterface*);

        bool setIpAddress(const SubnetAddress&);

        bool setMacAddress(const MACAddress&);

        bool setSpeed(unsigned long);

        bool setDuplex(Duplex);

        bool turnOn();
        
        bool turnOff();

        bool sendData(DataLinkLayer&);

        bool disconnect();

        [[nodiscard]] unsigned long getSpeed() const;

        [[nodiscard]] MACAddress getMacAddress() const;

        [[nodiscard]] bool getState() const;

        friend std::ostream& operator<<(std::ostream&, const EthernetInterface&);
};

#endif