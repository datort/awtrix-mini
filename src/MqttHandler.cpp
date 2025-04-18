#include "MqttHandler.h"
#include "Constants.h"

MqttHandler::MqttHandler(WiFiClient& wifiClient, Renderer& renderer, ConfigManager& configManager, WiFiManagerWrapper& wifiManagerWrapper) 
    : wifiClient(wifiClient), renderer(renderer), configManager(configManager), wifiManagerWrapper(wifiManagerWrapper) {
}

MqttHandler::~MqttHandler() {
}

bool MqttHandler::begin(const char* host, int port) {
    mqttClient.begin(host, port, wifiClient);
    return true;
}

bool MqttHandler::connect(const char* clientId) {
    const char* username = configManager.getMqttUsername();
    const char* password = configManager.getMqttPassword();
    
    if (strlen(username) > 0 && strlen(password) > 0) {
        return mqttClient.connect(clientId, username, password);
    } else {
        return mqttClient.connect(clientId);
    }
}

void MqttHandler::subscribe(const char* prefix) {
    mqttClient.subscribe(String(prefix) + "/#");
    mqttClient.onMessage([this](String &topic, String &payload) {
        String strippedTopic = topic.substring(topic.indexOf('/') + 1);
        this->onMessage(strippedTopic, payload);
    });
}

void MqttHandler::loop() {
    mqttClient.loop();
}

void MqttHandler::onMessage(String &task, String &payload) {
    Serial.print("MQTT Message - Task: ");
    Serial.println(task);
    Serial.print("Payload: ");
    Serial.println(payload);

    if (task == "erase") {
        handleEraseConfig();
    } else {
        Serial.println("Unknown topic");
    }
}

void MqttHandler::handleEraseConfig() {
    wifiManagerWrapper.eraseConfig();
    configManager.resetConfig();
    
    renderer.alert("Erase complete.\nRebooting now...", LIGHTGREY);
    Serial.println("Erase complete. Rebooting now...");

    delay(2500);
    ESP.restart();
}