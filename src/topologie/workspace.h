#include <vector>
#include <nlohmann/json.hpp>
#include "../device/device.h"
#include "nlohmann/json_fwd.hpp"

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

        nlohmann::json WDeviceAddParser(nlohmann::json);

        nlohmann::json WDeviceConnectParser(nlohmann::json);

        nlohmann::json WOpenDeviceSettings(nlohmann::json);

        nlohmann::json WToggleDeviceState(nlohmann::json);
        
        static Workspace& getWorkspace();
};