#include "../device/device.h"

class DeviceFactory {
    public:
        enum DeviceType {
            L2Switch,
            Router,
            EndDevice,
            TrafficSniffer
        };
    
    private:
        DeviceType devType;

    public:
        DeviceFactory();

        DeviceFactory& setType(DeviceType);

        Device* create();
};

std::ostream& operator<<(std::ostream&, DeviceFactory::DeviceType);

DeviceFactory::DeviceType deviceTypeFromString(const std::string&);