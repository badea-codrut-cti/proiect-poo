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

        virtual bool handleARPRequest(const DataLinkLayer&, const MACAddress&);

        virtual bool handlePingRequest(const DataLinkLayer&, const MACAddress&);

        virtual bool interfaceCallback(const DataLinkLayer&, uint8_t);

        explicit Device(uint8_t, bool, std::string=DEFAULT_DEVICE_HOSTNAME);

    public:
        Device();

        Device(const Device&);

        virtual ~Device() = default;

        virtual void turnOn();

        virtual void turnOff();

        void setHostname(const std::string&);

        virtual bool sendARPRequest(const IPv4Address&, bool);

        [[nodiscard]] virtual Device* clone() const;

        [[nodiscard]] bool getState() const;

        [[nodiscard]] const NetworkAdapter& getNetworkAdapter() const;

        [[nodiscard]] const std::string& getHostname() const;

        [[nodiscard]] const std::map<IPv4Address, MACAddress>& getARPCache() const;

        virtual bool receiveData(const DataLinkLayer&, EthernetInterface&);

        friend std::ostream& operator<<(std::ostream&, const Device&);
};

#endif
