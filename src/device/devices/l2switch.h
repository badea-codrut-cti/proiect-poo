#include "../device.h"

class L2Switch: public Device {
    private:
        std::map<MACAddress, uint8_t> arpRouteCache;

    protected:
        bool interfaceCallback(DataLinkLayer&, uint8_t) override;
    
    public:
        L2Switch();
};