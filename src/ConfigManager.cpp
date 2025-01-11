#include "ConfigManager.h"

ConfigManager::ConfigManager() {
    strcpy(hostname, "awtrixmini");
    strcpy(awtrixHostname, "192.168.1.100");
}

bool ConfigManager::loadConfig() {
    if (LittleFS.begin()) {
        if (LittleFS.exists("/config.json")) {
            File configFile = LittleFS.open("/config.json", "r");
            if (configFile) {
                size_t size = configFile.size();
                std::unique_ptr<char[]> buf(new char[size]);
                configFile.readBytes(buf.get(), size);

                JsonDocument json;
                auto deserializeError = deserializeJson(json, buf.get());
                if (!deserializeError) {
                    strcpy(hostname, json["hostname"]);
                    strcpy(awtrixHostname, json["awtrixHostname"]);
                    configFile.close();
                    return true;
                }
            }
        }
    }
    return false;
}

void ConfigManager::saveConfig(const char* updatedHostname, const char* updatedAwtrixHostname) {
    JsonDocument json;
    json["hostname"] = updatedHostname;
    json["awtrixHostname"] = updatedAwtrixHostname;

    File configFile = LittleFS.open("/config.json", "w");
    serializeJson(json, configFile);
    configFile.close();

    strcpy(hostname, updatedHostname);
    strcpy(awtrixHostname, updatedAwtrixHostname);
}

const char* ConfigManager::getHostname() {
    return hostname;
}

const char* ConfigManager::getAwtrixHostname() {
    return awtrixHostname;
}
