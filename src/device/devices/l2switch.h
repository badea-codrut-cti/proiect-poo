#include "../device.h"

#ifndef L2SWITCH_H
#define L2SWITCH_H

const uint8_t SWITCH_INT_COUNT = 26;

const std::string DEFAULT_SWITCH_HOSTNAME = "Switch";

class L2Switch: public Device {
    private:
        std::map<MACAddress, uint8_t> arpRouteCache;

    protected:
        bool interfaceCallback(const DataLinkLayer&, uint8_t) override;
    
    public:
        L2Switch();

        void turnOff() override;
};

#endif