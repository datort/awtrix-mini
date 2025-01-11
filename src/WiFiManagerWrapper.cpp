#include "WiFiManagerWrapper.h"

WiFiManagerWrapper::WiFiManagerWrapper() 
    : customHostname("hostname", "This device's hostname", "awtrixmini", 50),
      awtrixHostname("awtrixHostname", "AWTRIX Hostname/IP", "192.168.1.100", 50) {
    wm.addParameter(&customHostname);
    wm.addParameter(&awtrixHostname);
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