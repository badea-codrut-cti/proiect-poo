#include "uihook.h"
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include "workspace.h"
#include "uiexcept.h"
#include "../../build/main.html.h"
#include "../../build/pc.html.h"
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
        return ret.dump();
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
        uint64_t devIndex = Workspace::getWorkspace().WOpenDeviceSettings(data);
        UIWindow::getInstance().hookSettingsWindow(devIndex);
        return "{success: true}";
    } catch(const UIException& ex) {
        std::cout << ex.what();
        exit(-1);
    }
}

std::string windowDeviceConnect(std::string str) {
    json data = json::parse(std::move(str));
    Workspace::getWorkspace().WDeviceConnectParser(data);
    return "{success: true}";
}

std::string windowToggleDeviceState(std::string str) {
    try {
        json data = json::parse(std::move(str));
        Workspace::getWorkspace().WToggleDeviceState(data);
        return "{success: true}";
    } catch(const UIParameterException& ex) {
        std::string message = ex.what();
        hookWarningWindow(message);

        json errReturn = json::object();
        errReturn["success"] = false;
        errReturn["error"] = message;

        return errReturn.dump(); 
    }
}

UIWindow::UIWindow() {
    window = new webview::webview(false, nullptr);
    window->set_title("Autodragan");
    window->set_size(1200, 900, 0);
    std::string content(reinterpret_cast<char*>(main_html), main_html_len);
    window->set_html(content);
    window->bind("wDeviceAdd",windowDeviceAdd);
    window->bind("wDeviceConnect", windowDeviceConnect);
    window->bind("wOpenDeviceSettings", windowOpenDeviceSettings);
    window->run();
    window->terminate();
}

UIWindow UIWindow::instance{};

UIWindow& UIWindow::getInstance() {
    return instance;
}

void UIWindow::sendDeviceUpdateNotice(uint64_t devId) {
    window->eval("alert('" + std::to_string(devId) + "')");
}

void UIWindow::hookSettingsWindow(uint64_t index) {
    Device* dev = Workspace::getWorkspace().getDevice(index);
    json device = deviceToJson(dev);
    webview::webview alter_window(false, nullptr);
    alter_window.set_title(dev->getHostname());
    alter_window.set_size(720, 960, 0);
    std::string content(reinterpret_cast<char*>(pc_html), pc_html_len);
    alter_window.set_html(content);

    auto deviceDataCallback = [dev](const std::string&) {
        return std::string(deviceToJson(dev).dump());
    };

    auto deviceUpdateCallback = [&alter_window, index](const std::string& set) {
        try {
            json obj = json::parse(set);
            Workspace::getWorkspace().changeDeviceSettings(index, obj[0]);
            //std::cout << deviceToJson(Workspace::getWorkspace().getDevice(index));
            alter_window.eval("window.wDeviceUpdateListener()");
            return std::string("{success: true}");
        } catch (const UIParameterException& e) {
            json ret = json::object();
            ret["success"] = false;
            ret["reason"] = e.what();
            hookWarningWindow(e.what());
            return std::string(ret.dump());
        } catch(const UIException& e) {
            std::cout << e.what();
            exit(-1);
        }
    };

    alter_window.bind("wGetDeviceData", deviceDataCallback);
    alter_window.bind("wChangeDeviceSettings", deviceUpdateCallback);

    alter_window.run();
    alter_window.terminate();
}