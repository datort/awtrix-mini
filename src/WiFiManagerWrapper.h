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
    const WiFiManagerParameter& getAwtrixAuthUser() const;
    const WiFiManagerParameter& getAwtrixAuthPass() const;
    const WiFiManagerParameter& getMqttBroker() const;
    const WiFiManagerParameter& getMqttPort() const;
    const WiFiManagerParameter& getMqttUsername() const;
    const WiFiManagerParameter& getMqttPassword() const;
    const WiFiManagerParameter& getMqttTopic() const;
    void eraseConfig();
private:
    WiFiManager wm;
    WiFiManagerParameter customHostname;
    WiFiManagerParameter awtrixHostname;
    WiFiManagerParameter awtrixAuthUser;
    WiFiManagerParameter awtrixAuthPass;
    WiFiManagerParameter mqttBroker;
    WiFiManagerParameter mqttPort;
    WiFiManagerParameter mqttUsername;
    WiFiManagerParameter mqttPassword;
    WiFiManagerParameter mqttTopic;
};

#endif