#include "./l2switch.h"
#include "../adapter.h"
#include <cstdint>
#include <iostream>
#include <utility>


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

    uint8_t intId = it->second;
    adapter[intId].sendData(_data);
    return true;
}

L2Switch::L2Switch(): Device(24) {
    adapter[0].setSpeed(1000);
    adapter[12].setSpeed(1000);
}