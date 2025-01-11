#ifndef WIFI_MANAGER_WRAPPER_H
#define WIFI_MANAGER_WRAPPER_H

#include <WiFiManager.h>

class WiFiManagerWrapper {
public:
    WiFiManagerWrapper();
    void setupWiFi(const char* apName, const char* apPassword, std::function<void()> onSaveConfig);
    bool isSetupComplete();
    String getSSID();
    const WiFiManagerParameter& getCustomHostname() const;
    const WiFiManagerParameter& getAwtrixHostname() const;

private:
    WiFiManager wm;
    WiFiManagerParameter customHostname;
    WiFiManagerParameter awtrixHostname;
};

#endif