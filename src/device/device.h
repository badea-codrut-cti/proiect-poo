#include "./ethernet.h"
#include "./adapter.h"
#include <map>

#ifndef DEVICE_H
#define DEVICE_H

class Device {
    private:
        std::map<IPv4Address, MACAddress> arpCache{};

    protected:
        bool isOn{false};

        NetworkAdapter adapter;

        MACAddress getArpEntryOrBroadcast(const IPv4Address&);

        virtual bool interfaceCallback(DataLinkLayer&, uint8_t);

        explicit Device(uint8_t);

    public:
        Device();

        virtual void turnOn();

        virtual void turnOff();

        [[nodiscard]] bool getState() const;

        virtual bool receiveData(DataLinkLayer&, EthernetInterface&);

        friend std::ostream& operator<<(std::ostream&, const Device&);
};

#endif
