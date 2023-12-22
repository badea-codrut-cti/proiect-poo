#ifndef UIWINDOW_H
#define UIWINDOW_H

#include "../device/device.h"
#include <webview.h>

class UIWindow {
    static UIWindow instance;

    webview::webview* window;

    UIWindow();

    public:
        UIWindow(const UIWindow&) = delete;

        UIWindow& operator=(const UIWindow&) = delete;

        ~UIWindow();

        static UIWindow& getInstance();

        void hookSettingsWindow(uint64_t);

        void sendDeviceUpdateNotice(uint64_t devId);
};

#endif