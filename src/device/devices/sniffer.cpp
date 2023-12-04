#include "sniffer.h"
#include <cstdint>
#include <sstream>

TrafficSniffer::TrafficSniffer() : Device(2, true, DEFAULT_SNIFFER_HOSTNAME) {}

bool TrafficSniffer::interfaceCallback(const DataLinkLayer& data, uint8_t interface) {
    std::ostringstream os;
    os << *data.getPayload();
    sniffedData.push_back(os.str());

    if (interface == 0)
        adapter[1].sendData(data);
    else
        adapter[0].sendData(data);

    return true;
}

void TrafficSniffer::turnOff() {
    sniffedData.clear();
    Device::turnOff();
}