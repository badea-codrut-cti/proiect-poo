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

void hookWarningWindow(const std::string& str) {
    webview::webview warning_window(false, nullptr);
    warning_window.set_title("Warning");
    warning_window.set_html(str);
    warning_window.run();
    warning_window.terminate();
}

std::string windowDeviceAdd(std::string str) {
    try {
        json data = json::parse(std::move(str));
        json ret = Workspace::getWorkspace().WDeviceAddParser(data);
        return ret;
    } catch(const UIParameterException& ex) {
        std::string message = ex.what();
        hookWarningWindow(message);

        json errReturn = json::object();
        errReturn["success"] = false;
        errReturn["error"] = message;

        return errReturn.dump(); 
    } catch(const UIException& ex) {
        std::cout << ex.what();
        exit(-1);
    }
}

std::string windowOpenDeviceSettings(std::string str) {
    try {
        json data = json::parse(std::move(str));
        json ret = Workspace::getWorkspace().WOpenDeviceSettings(data);
        return ret;
    } catch(const UIException& ex) {
        std::cout << ex.what();
        exit(-1);
    }
}

std::string windowDeviceConnect(std::string str) {
    json data = json::parse(std::move(str));
    return Workspace::getWorkspace().WDeviceConnectParser(data);
}

std::string windowToggleDeviceState(std::string str) {
    try {
        json data = json::parse(std::move(str));
        json ret = Workspace::getWorkspace().WToggleDeviceState(data);
        return ret;
    } catch(const UIParameterException& ex) {
        std::string message = ex.what();
        hookWarningWindow(message);

        json errReturn = json::object();
        errReturn["success"] = false;
        errReturn["error"] = message;

        return errReturn.dump(); 
    }
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