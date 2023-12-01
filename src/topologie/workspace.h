#include <vector>
#include <nlohmann/json.hpp>
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

        std::string WDeviceConnectParser(std::string);

        std::string WOpenDeviceSettings(std::string);
        
        static Workspace& getWorkspace();
};