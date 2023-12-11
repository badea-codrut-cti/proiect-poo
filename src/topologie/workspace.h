#include <vector>
#include <nlohmann/json.hpp>
#include "../device/device.h"

class Workspace {
    std::vector<Device*> devices{};
    static Workspace workspace;

    private:
        Workspace() = default;
    
    public:
        Workspace(const Workspace&) = delete;
        Workspace& operator=(const Workspace&) = delete;
        ~Workspace();

        uint64_t addDevice(Device*);

        Device* getDevice(uint64_t);

        nlohmann::json WDeviceAddParser(nlohmann::json);

        nlohmann::json WDeviceConnectParser(nlohmann::json);

        nlohmann::json WOpenDeviceSettings(nlohmann::json);

        nlohmann::json WToggleDeviceState(nlohmann::json);

        nlohmann::json changeDeviceSettings(uint64_t, nlohmann::json);
        
        static Workspace& getWorkspace();
};