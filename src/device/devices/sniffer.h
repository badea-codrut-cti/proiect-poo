#include "../device.h"
#include <vector>
#include <string>

const std::string DEFAULT_SNIFFER_HOSTNAME = "Sniffer";

class TrafficSniffer : Device {
    std::vector<std::string> sniffedData;

    protected:
        bool interfaceCallback(const DataLinkLayer&, uint8_t) override;

    public:
        TrafficSniffer();
};