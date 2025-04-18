#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <ArduinoJson.h>
#include <LittleFS.h>

class ConfigManager {
public:
    ConfigManager();
    bool loadConfig();
    void saveConfig(const char* hostname, const char* awtrixHostname, 
                   const char* mqttBroker, const char* mqttPort,
                   const char* mqttUsername, const char* mqttPassword,
                   const char* mqttTopic);
    void resetConfig();
    const char* getHostname();
    const char* getAwtrixHostname();
    const char* getMqttBroker();
    const char* getMqttPort();
    const char* getMqttUsername();
    const char* getMqttPassword();
    const char* getMqttTopic();
    void updateSetting(const char* key, const char* value);
    
private:
    char hostname[50];
    char awtrixHostname[50];
    char mqttBroker[50];
    char mqttPort[10];
    char mqttUsername[50];
    char mqttPassword[50];
    char mqttTopic[50];
};

#endif
