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

        bool WDeviceConnectParser(nlohmann::json);

        bool WOpenDeviceSettings(nlohmann::json);

        bool WToggleDeviceState(nlohmann::json);

        bool changeDeviceSettings(uint64_t, nlohmann::json);
        
        static Workspace& getWorkspace();
};