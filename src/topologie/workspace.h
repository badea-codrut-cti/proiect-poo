#include <vector>
#include "../device/device.h"

class Workspace {
    std::vector<Device*> devices;
    static Workspace workspace;

    private:
        Workspace() = default;
    
    public:
        Workspace(const Workspace&) = delete;
        Workspace& operator=(const Workspace&) = delete;
        ~Workspace();

        unsigned int addDevice(Device*);

        std::string WDeviceAddParser(std::string);

        Workspace& getWorkspace();
};
