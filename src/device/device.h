#include "./ethernet.h"
#include "./adapter.h"
#include <map>

#ifndef DEVICE_H
#define DEVICE_H

const std::string DEFAULT_DEVICE_HOSTNAME = "Device";

class Device {
    private:
        std::map<IPv4Address, MACAddress> arpCache{};

    protected:
        bool isOn{true};

        NetworkAdapter adapter;

        std::string hostname;

        MACAddress getArpEntryOrBroadcast(const IPv4Address&);

        bool handleARPRequest(DataLinkLayer&, uint8_t);

        virtual bool interfaceCallback(DataLinkLayer&, uint8_t);

        explicit Device(uint8_t, bool, std::string=DEFAULT_DEVICE_HOSTNAME);

    public:
        Device();

        virtual void turnOn();

        virtual void turnOff();

        void setHostname(const std::string&);

        bool sendARPRequest(const IPv4Address&);

        [[nodiscard]] bool getState() const;

        [[nodiscard]] const NetworkAdapter& getNetworkAdapter() const;

        [[nodiscard]] const std::string& getHostname() const;

        [[nodiscard]] const std::map<IPv4Address, MACAddress>& getARPCache() const;

        virtual bool receiveData(DataLinkLayer&, EthernetInterface&);

        friend std::ostream& operator<<(std::ostream&, const Device&);
};

#endif
