#ifndef MQTT_HANDLER_H
#define MQTT_HANDLER_H

#include <MQTT.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include "Renderer.h"
#include "ConfigManager.h"
#include "WiFiManagerWrapper.h"

class MqttHandler {
public:
    MqttHandler(WiFiClient& wifiClient, Renderer& renderer, ConfigManager& configManager, WiFiManagerWrapper& wifiManagerWrapper);
    ~MqttHandler();
    
    bool begin(const char* host, int port);
    bool connect(const char* clientId);
    void subscribe(const char* prefix);
    void loop();
    
private:
    MQTTClient mqttClient;
    WiFiClient& wifiClient;
    Renderer& renderer;
    ConfigManager& configManager;
    WiFiManagerWrapper& wifiManagerWrapper;
    
    void onMessage(String &topic, String &payload);
    void handleEraseConfig();
};

#endif 