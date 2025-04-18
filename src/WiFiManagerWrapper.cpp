#include "WiFiManagerWrapper.h"

WiFiManagerWrapper::WiFiManagerWrapper() 
    : customHostname("hostname", "This device's hostname", "awtrixstream", 50),
      awtrixHostname("awtrixHostname", "Host AWTRIX hostname or IP", "192.168.178.123", 50),
      awtrixAuthUser("awtrixAuthUser", "", "", 50, " placeholder=\"AWTRIX Auth User (optional)\""),
      awtrixAuthPass("awtrixAuthPass", "", "", 50, " placeholder=\"AWTRIX Auth Password (optional)\""),
      mqttBroker("mqttBroker", "MQTT Broker", "", 50, " placeholder=\"my.broker.local\""),
      mqttPort("mqttPort", "MQTT Port", "1883", 5, " placeholder=\"1883\""),
      mqttUsername("mqttUsername", "MQTT Username", "", 50),
      mqttPassword("mqttPassword", "MQTT Password", "", 50),
      mqttTopic("mqttTopic", "MQTT Topic", "awtrixstream", 50) {
}

void WiFiManagerWrapper::setupWiFi(const char* apName, const char* apPassword, std::function<void()> onSaveConfig) {

    WiFiManagerParameter stepDivider("<hr />");
    
    WiFiManagerParameter headlineBasicSetup("<h2>Basic setup</h2>");
    wm.addParameter(&headlineBasicSetup);

    wm.addParameter(&awtrixHostname);
    wm.addParameter(&customHostname);

    wm.addParameter(&stepDivider);

    WiFiManagerParameter headlineAwtrixAuth("<h2>AWTRIX Authentication</h2>");
    WiFiManagerParameter hintAwtrixAuth("<p><i>Only if you configured authentication on your Host AWTRIX</i></p>");
    wm.addParameter(&headlineAwtrixAuth);
    wm.addParameter(&hintAwtrixAuth);

    wm.addParameter(&awtrixAuthUser);
    wm.addParameter(&awtrixAuthPass);

    wm.addParameter(&stepDivider);

    WiFiManagerParameter headlineMqttSetup("<h2>MQTT configuration</h2>");
    WiFiManagerParameter hintMqttSetup("<p><i>Optional: Leave as is if you don't want to use MQTT</i></p>");
    wm.addParameter(&headlineMqttSetup);
    wm.addParameter(&hintMqttSetup);

    wm.addParameter(&mqttBroker);
    wm.addParameter(&mqttPort);
    wm.addParameter(&mqttTopic);
    wm.addParameter(&mqttUsername);
    wm.addParameter(&mqttPassword);

    wm.setClass("invert");
    wm.setScanDispPerc(true);
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

const WiFiManagerParameter& WiFiManagerWrapper::getAwtrixAuthUser() const {
    return awtrixAuthUser;
}

const WiFiManagerParameter& WiFiManagerWrapper::getAwtrixAuthPass() const {
    return awtrixAuthPass;
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