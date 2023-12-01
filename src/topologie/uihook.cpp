#include "uihook.h"
#include <nlohmann/json.hpp>
#include <utility>
#include "workspace.h"
#include "uiexcept.h"
#include <webview.h>
#include "../../build/main.ts.html.h"
#include "../../build/pc.ts.html.h"
#include "jsonify_data.h"

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

std::string windowOpenDeviceSettings(std::string str) {
    try {
        return Workspace::getWorkspace().WOpenDeviceSettings(std::move(str));
    } catch(const UIException& ex) {
        std::cout << ex.what();
        exit(-1);
    }
}

std::string windowDeviceConnect(std::string str) {
    return Workspace::getWorkspace().WDeviceConnectParser(std::move(str));
}

void hookMainWindow() {
    webview::webview window(false, nullptr);
    window.set_title("Autodragan");
    window.set_size(1200, 900, 0);
    std::string content(reinterpret_cast<char*>(main_ts_html), main_ts_html_len);
    window.set_html(content);
    window.bind("wDeviceAdd",windowDeviceAdd);
    window.bind("wDeviceConnect", windowDeviceConnect);
    window.bind("wOpenDeviceSettings", windowOpenDeviceSettings);
    window.run();
}

void hookSettingsWindow(Device* dev) {
    json device = deviceToJson(dev);
    webview::webview alter_window(false, nullptr);
    alter_window.set_title(dev->getHostname());
    alter_window.set_size(720, 960, 0);
    std::string content(reinterpret_cast<char*>(pc_ts_html), pc_ts_html_len);
    alter_window.set_html(content);
    alter_window.run();
    alter_window.terminate();
}