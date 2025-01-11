#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <ArduinoJson.h>
#include <LittleFS.h>

class ConfigManager {
public:
    ConfigManager();
    bool loadConfig();
    void saveConfig(const char* hostname, const char* awtrixHostname);
    const char* getHostname();
    const char* getAwtrixHostname();
    
private:
    char hostname[50];
    char awtrixHostname[50];
};

#endif
