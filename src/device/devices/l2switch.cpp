#include "./l2switch.h"
#include "../adapter.h"

// "Ai control complet asupra echipamentului?"
bool L2Switch::interfaceCallback(const DataLinkLayer& data, uint8_t fIndex) {
    auto it = arpRouteCache.find(data.getMACDestination());
    if (data.getMACDestination() == "FF:FF:FF:FF:FF:FF" || it == arpRouteCache.end()) {
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

L2Switch::L2Switch(): Device(SWITCH_INT_COUNT, true, DEFAULT_SWITCH_HOSTNAME) {
    adapter[0].setSpeed(1000);
    adapter[SWITCH_INT_COUNT/2].setSpeed(1000);
}