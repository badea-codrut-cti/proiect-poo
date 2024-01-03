#include "./l2switch.h"
#include "../adapter.h"

bool L2Switch::interfaceCallback(const DataLinkLayer& data, uint8_t fIndex) {
    auto it = arpRouteCache.find(data.getMACDestination());
    if (data.getMACDestination() == MACAddress(MACAddress::broadcastAddress)
     || it == arpRouteCache.end()) {
        for (uint8_t i = 0; i < adapter.interfaceCount(); i++) {
            if (i == fIndex)
                continue;
            adapter[i].sendData(data);
        }
    } else {
        uint8_t intId = it->second;
        adapter[intId].sendData(data);
    }

    arpRouteCache.insert(std::make_pair(data.getMACSource(), fIndex));
    return true;
}

L2Switch::L2Switch(): Device(1, true, DEFAULT_SWITCH_HOSTNAME) {
    NetworkAdapter copyAdapter(*this, SWITCH_INT_COUNT, 
        std::function<EthernetInterface*(Device&, uint8_t)>([](Device& dev, uint8_t index) {
            return new EthernetInterface(dev, index > 1 ? FAST_ETHERNET : GIGABIT_ETHERNET, true);
        })
    );

    adapter.copy(*this, copyAdapter);
}

void L2Switch::turnOff() {
    arpRouteCache.clear();
    Device::turnOff();
}