#include "./l2switch.h"
#include "../adapter.h"

// "Ai control complet asupra echipamentului?"
bool L2Switch::interfaceCallback(DataLinkLayer& _data, uint8_t fIndex) {
    auto it = arpRouteCache.find(_data.getMACDestination());
    if (_data.getMACDestination() == "FF:FF:FF:FF:FF:FF" || it == arpRouteCache.end()) {
        for (uint8_t i = 0; i < adapter.interfaceCount(); i++) {
            if (i == fIndex)
                continue;
            adapter[i].sendData(_data);
        }
        return true;
    }

    arpRouteCache.insert(std::make_pair(_data.getMACSource(), fIndex));

    uint8_t intId = it->second;
    adapter[intId].sendData(_data);
    return true;
}

L2Switch::L2Switch(): Device(SWITCH_INT_COUNT, DEFAULT_SWITCH_HOSTNAME) {
    adapter[0].setSpeed(1000);
    adapter[SWITCH_INT_COUNT/2].setSpeed(1000);
}