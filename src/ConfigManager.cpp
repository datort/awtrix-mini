#include "ConfigManager.h"

ConfigManager::ConfigManager() {
  strcpy(hostname, "awtrixmini");
  strcpy(awtrixHostname, "192.168.178.123");
  strcpy(awtrixAuthUser, "");
  strcpy(awtrixAuthPass, "");
  strcpy(mqttBroker, "");
  strcpy(mqttPort, "1883");
  strcpy(mqttUsername, "");
  strcpy(mqttPassword, "");
  strcpy(mqttTopic, "awtrixmini");
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

          Serial.print("read config.json contents: ");
          serializeJson(json, Serial);
          Serial.println();

          strcpy(hostname, json["hostname"]);
          strcpy(awtrixHostname, json["awtrixHostname"]);
          strcpy(awtrixAuthUser, json["awtrixAuthUser"]);
          strcpy(awtrixAuthPass, json["awtrixAuthPass"]);
          
          if (json.containsKey("mqttBroker")) {
            strcpy(mqttBroker, json["mqttBroker"]);
          }
          if (json.containsKey("mqttPort")) {
            strcpy(mqttPort, json["mqttPort"]);
          }
          if (json.containsKey("mqttUsername")) {
            strcpy(mqttUsername, json["mqttUsername"]);
          }
          if (json.containsKey("mqttPassword")) {
            strcpy(mqttPassword, json["mqttPassword"]);
          }
          if (json.containsKey("mqttTopic")) {
            strcpy(mqttTopic, json["mqttTopic"]);
          }
          
          configFile.close();
          return true;
        }
      }
    } else {
      Serial.println("config.json not found");
    }
  } else {
    Serial.println("Couldn't init LittleFS");
  }
  return false;
}

void ConfigManager::saveConfig(const char* updatedHostname, const char* updatedAwtrixHostname, 
                             const char* updatedAwtrixAuthUser, const char* updatedAwtrixAuthPass,
                             const char* updatedMqttBroker, const char* updatedMqttPort,
                             const char* updatedMqttUsername, const char* updatedMqttPassword,
                             const char* updatedMqttTopic) {
  JsonDocument json;
  json["hostname"] = updatedHostname;
  json["awtrixHostname"] = updatedAwtrixHostname;
  json["awtrixAuthUser"] = updatedAwtrixAuthUser;
  json["awtrixAuthPass"] = updatedAwtrixAuthPass;
  json["mqttBroker"] = updatedMqttBroker;
  json["mqttPort"] = updatedMqttPort;
  json["mqttUsername"] = updatedMqttUsername;
  json["mqttPassword"] = updatedMqttPassword;
  json["mqttTopic"] = updatedMqttTopic;

  File configFile = LittleFS.open("/config.json", "w");
  serializeJson(json, configFile);
  configFile.close();

  strcpy(hostname, updatedHostname);
  strcpy(awtrixHostname, updatedAwtrixHostname);
  strcpy(awtrixAuthUser, updatedAwtrixAuthUser);
  strcpy(awtrixAuthPass, updatedAwtrixAuthPass);
  strcpy(mqttBroker, updatedMqttBroker);
  strcpy(mqttPort, updatedMqttPort);
  strcpy(mqttUsername, updatedMqttUsername);
  strcpy(mqttPassword, updatedMqttPassword);
  strcpy(mqttTopic, updatedMqttTopic);
}

void ConfigManager::resetConfig() {
  if (LittleFS.begin()) {
    if (LittleFS.exists("/config.json")) {
      LittleFS.remove("/config.json");
      Serial.println("Config file deleted");
    }
  }
}

const char* ConfigManager::getHostname() {
  return hostname;
}

const char* ConfigManager::getAwtrixHostname() {
  return awtrixHostname;
}

const char* ConfigManager::getAwtrixAuthUser() {
  return awtrixAuthUser;
}

const char* ConfigManager::getAwtrixAuthPass() {
  return awtrixAuthPass;
}

const char* ConfigManager::getMqttBroker() {
  return mqttBroker;
}

const char* ConfigManager::getMqttPort() {
  return mqttPort;
}

const char* ConfigManager::getMqttUsername() {
  return mqttUsername;
}

const char* ConfigManager::getMqttPassword() {
  return mqttPassword;
}

const char* ConfigManager::getMqttTopic() {
  return mqttTopic;
}

void ConfigManager::updateSetting(const char* key, const char* value) {
  JsonDocument json;
  
  if (LittleFS.exists("/config.json")) {
    File configFile = LittleFS.open("/config.json", "r");
    if (configFile) {
      DeserializationError error = deserializeJson(json, configFile);
      configFile.close();
      if (error) {
        Serial.println("Failed to read config file, using empty config");
      }
    }
  }

  json[key] = value;

  File configFile = LittleFS.open("/config.json", "w");
  if (configFile) {
    serializeJson(json, configFile);
    configFile.close();
  } else {
    Serial.println("Failed to open config file for writing");
  }
}
