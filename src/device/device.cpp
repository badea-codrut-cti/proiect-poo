#include "./device.h"
#include "./adapter.h"
#include "../date/osi/network.h"
#include "../protocoale/arp.h"
#include <stdexcept>
#include <utility>
#include <iostream>

Device::Device(uint8_t interfaceCount, std::string _hostname): 
adapter(*this, interfaceCount), hostname(std::move(_hostname)) {

}

MACAddress Device::getArpEntryOrBroadcast(const IPv4Address& ip) {
    auto it = arpCache.find(ip);
    if (it == arpCache.end())
        return MACAddress::broadcastAddress;
    return it->second;
}

Device::Device(): adapter(*this, 1), hostname(DEFAULT_DEVICE_HOSTNAME) {

}

void Device::setHostname(const std::string& str) {
    hostname = str;
}

void Device::turnOn() {
    isOn = true;
}

void Device::turnOff() { 
    arpCache.clear();
    isOn = false;
}

bool Device::getState() const { 
    return isOn;
}

bool Device::interfaceCallback([[maybe_unused]]DataLinkLayer& _data, [[maybe_unused]]uint8_t index) {
    return true;
}

bool Device::receiveData(DataLinkLayer& _data, EthernetInterface& _interface) {
    if (!isOn)
        return false;
    if (_data.getL2Type() == DataLinkLayer::ARP) {
        try {
            auto payload = dynamic_cast<ARPPayload&>(_data.getPayload());

            std::array<uint8_t, MACADDRESS_SIZE> hwSrc{};
            std::array<uint8_t, MACADDRESS_SIZE> hwDest{};

            for (uint8_t i = 0; i < MACADDRESS_SIZE; i++) {
                hwSrc[i] = payload.getSourceHardwareAddress()[i];
                hwDest[i] = payload.getDestinationHardwareAddress()[i];
            }

            std::array<uint8_t, IPV4_SIZE> protoSrc{};
            std::array<uint8_t, IPV4_SIZE> protoDest{};

            for (uint8_t i = 0; i < IPV4_SIZE; i++) {
                protoSrc[i] = payload.getSourceProtocolAddress()[i];
                protoDest[i] = payload.getDestinationProtocolAddress()[i];
            }

            if (payload.getOperation() == ARPPayload::REQUEST) {
                try {
                    int fIndex = adapter.getIntefaceIndex(protoDest);
                    EthernetInterface& interface = adapter[fIndex];
                    ARPPayload payload(ARPPayload::REPLY, interface.getMacAddress().getOctets(),
                     hwSrc, protoDest, protoSrc);
                    DataLinkLayer l2(interface.getMacAddress(), {hwSrc}, payload, DataLinkLayer::ARP);
                    NetworkLayer l3(l2, {protoDest}, {protoSrc});
                    return interface.sendData(l3);
                } catch(const std::out_of_range& e) {
                    
                }   
            } else {
                arpCache.insert(std::make_pair(protoSrc, hwSrc));
            }
        } catch(const std::bad_cast& e) {

        }
    }
    try {
        int fIndex = adapter.getIntefaceIndex(_interface.getMacAddress());
        return interfaceCallback(_data, fIndex);
    } catch(const std::out_of_range& e) {
        throw std::invalid_argument("Interface does not belong in the network adapter.");
    }
}

const std::map<IPv4Address, MACAddress>& Device::getARPCache() const {
    return arpCache;
}

const NetworkAdapter& Device::getNetworkAdapter() const {
    return adapter;
}

const std::string& Device::getHostname() const {
    return hostname;
}


std::ostream& operator<<(std::ostream& os, const Device& device) {
    os << "Status: " << (device.getState() ? "on" : "off") << "\r\n";
    os << "Hostname: " << device.getHostname() << "\r\n";
    return os;
}