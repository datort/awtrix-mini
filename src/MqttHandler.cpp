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

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);
    
    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        renderer.alert("Invalid JSON via MQTT", RED);
        delay(2000);
        return;
    }

    if (task == "erase") {
        handleEraseConfig();
    } else if (task == "setting") {
        handleSettingUpdate(doc);
    } else if (task == "display") {
        toggleDisplay(doc);
    } else if (task == "reboot") {
        renderer.alert("Rebooting device...", ORANGE);
        delay(2500);
        ESP.restart();
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

void MqttHandler::handleSettingUpdate(JsonDocument &json) {
    const char* keys[] = {
        "hostname",
        "awtrixHostname",
        "awtrixAuthUser",
        "awtrixAuthPass",
        "mqttBroker",
        "mqttPort",
        "mqttUsername",
        "mqttPassword",
        "mqttTopic"
    };

    const size_t numKeys = sizeof(keys) / sizeof(keys[0]);

    for (size_t i = 0; i < numKeys; i++) {
        const char* key = keys[i];
        if (json[key].is<const char*>()) {
            configManager.updateSetting(key, json[key]);
        }
    }

    renderer.alert("Setting saved.\nConsider rebooting.", GREEN);
    delay(2500);
    renderer.tft.fillScreen(BLACK);
}

void MqttHandler::toggleDisplay(JsonDocument &json) {
    if (json["on"].is<bool>()) {
        renderer.tft.writecommand(json["on"] == false ? 0x28 : 0x29);
        digitalWrite(D6, json["on"] == false ? LOW : HIGH);
    }
}