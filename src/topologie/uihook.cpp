#include "uihook.h"
#include <nlohmann/json.hpp>
#include <utility>
#include "workspace.h"
#include "uiexcept.h"

using json = nlohmann::json;

std::string windowDeviceAdd(std::string str) {
    try {
        return Workspace::getWorkspace().WDeviceAddParser(std::move(str));
    } catch(const UIParameterException& ex) {
        json errReturn = json::object();
        errReturn["success"] = false;
        errReturn["error"] = ex.what();

        return errReturn.dump(); 
    } catch(const UIException& ex) {
        std::cout << ex.what();
        exit(-1);
    }
}

std::string windowDeviceConnect(std::string str) {
    return Workspace::getWorkspace().WDeviceConnectParser(std::move(str));
}

void hookMainWindow(webview::webview& window) {
    window.bind("wDeviceAdd",windowDeviceAdd);
    window.bind("wDeviceConnect", windowDeviceConnect);
};