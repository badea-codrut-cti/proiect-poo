#include "../device/device.h"

class DeviceFactory {
    public:
        enum DeviceType {
            l2Switch,
            router,
            endDevice,
            trafficSniffer
        };
    
    private:
        DeviceType devType{endDevice};

    public:
        DeviceFactory() = default;

        DeviceFactory& setType(DeviceType);

        Device* create();
};

std::ostream& operator<<(std::ostream&, DeviceFactory::DeviceType);

DeviceFactory::DeviceType deviceTypeFromString(const std::string&);