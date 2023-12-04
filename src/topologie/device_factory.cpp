#include "device_factory.h"
#include "../device/devices/l2switch.h"
#include "../device/devices/end_device.h"
#include "../device/devices/router.h"
#include "../device/devices/sniffer.h"
#include "uiexcept.h"
#include <sstream>

DeviceFactory::DeviceFactory() : devType(EndDevice) {}

DeviceFactory& DeviceFactory::setType(DeviceFactory::DeviceType devType) {
    this->devType = devType;
    return *this;
}

Device* DeviceFactory::create() {
    Device* dev;

    switch (devType) {
        case DeviceFactory::L2Switch: {
            dev = new class L2Switch();
            break;
        }

        case DeviceFactory::EndDevice: {
            dev = new class EndDevice();
            break;
        }

        case DeviceFactory::Router: {
            dev = new class Router();
            break;
        }

        case DeviceFactory::TrafficSniffer: {
            dev = new class TrafficSniffer();
            break;
        }
    }

    return dev;
}

std::ostream& operator<<(std::ostream& os, DeviceFactory::DeviceType devType) {
    switch (devType) {
        case DeviceFactory::L2Switch: {
            os << "l2switch";
            break;
        }

        case DeviceFactory::EndDevice: {
            os << "pc";
            break;
        }

        case DeviceFactory::Router: {
            os << "router";
            break;
        }

        case DeviceFactory::TrafficSniffer: {
            os << "sniffer";
            break;
        }
    }

    return os;
}

DeviceFactory::DeviceType deviceTypeFromString(const std::string& str) {
    DeviceFactory::DeviceType types[] = {DeviceFactory::L2Switch, DeviceFactory::EndDevice, 
    DeviceFactory::Router, DeviceFactory::TrafficSniffer};

    for (auto type : types) {
        std::ostringstream os;
        os << type;
        if (os.str() == str)
            return type;
    }

    throw UIParameterException("deviceType");
}