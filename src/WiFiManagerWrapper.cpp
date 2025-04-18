#include "WiFiManagerWrapper.h"

WiFiManagerWrapper::WiFiManagerWrapper() 
    : customHostname("hostname", "This device's hostname", "awtrixmini", 50),
      awtrixHostname("awtrixHostname", "AWTRIX Hostname/IP", "192.168.178.123", 50),
      mqttBroker("mqttBroker", "MQTT Broker", "", 50),
      mqttPort("mqttPort", "MQTT Port", "1883", 5),
      mqttUsername("mqttUsername", "MQTT Username", "", 50),
      mqttPassword("mqttPassword", "MQTT Password", "", 50),
      mqttTopic("mqttTopic", "MQTT Topic", "awtrixmini", 50) {
    wm.addParameter(&customHostname);
    wm.addParameter(&awtrixHostname);
    wm.addParameter(&mqttBroker);
    wm.addParameter(&mqttPort);
    wm.addParameter(&mqttTopic);
    wm.addParameter(&mqttUsername);
    wm.addParameter(&mqttPassword);

    wm.setClass("invert");
    wm.setScanDispPerc(true);
}

void WiFiManagerWrapper::setupWiFi(const char* apName, const char* apPassword, std::function<void()> onSaveConfig) {
    wm.setSaveConfigCallback(onSaveConfig);

    if (!wm.autoConnect(apName, apPassword)) {
        delay(5000);
        ESP.restart();
    }
}

bool WiFiManagerWrapper::isSetupComplete() {
    return wm.getWiFiIsSaved();
}

String WiFiManagerWrapper::getSSID() {
    return wm.getWiFiSSID();
}

const WiFiManagerParameter& WiFiManagerWrapper::getCustomHostname() const {
    return customHostname;
}

const WiFiManagerParameter& WiFiManagerWrapper::getAwtrixHostname() const {
    return awtrixHostname;
}

const WiFiManagerParameter& WiFiManagerWrapper::getMqttBroker() const {
    return mqttBroker;
}

const WiFiManagerParameter& WiFiManagerWrapper::getMqttPort() const {
    return mqttPort;
}

const WiFiManagerParameter& WiFiManagerWrapper::getMqttUsername() const {
    return mqttUsername;
}

const WiFiManagerParameter& WiFiManagerWrapper::getMqttPassword() const {
    return mqttPassword;
}

const WiFiManagerParameter& WiFiManagerWrapper::getMqttTopic() const {
    return mqttTopic;
}

void WiFiManagerWrapper::eraseConfig() {
    wm.resetSettings();
}