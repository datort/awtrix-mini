#include <Arduino.h>
#include <SPI.h>
#include "ConfigManager.h"
#include "WiFiManagerWrapper.h"
#include "Renderer.h"
#include "Crawler.h"
#include "MqttHandler.h"
#include <arduino-timer.h>
#include <ArduinoJson.h>
#include "Constants.h"


ConfigManager configManager;
WiFiManagerWrapper wifiManagerWrapper;
Crawler crawler;
Renderer& renderer = Renderer::getInstance();
WiFiClient wifiClient;
MqttHandler mqttHandler(wifiClient, renderer, configManager, wifiManagerWrapper);

String awtrixApiUrl;
uint8_t errorCount = 0;

auto timer = timer_create_default();
bool saveConfig = false;

void saveConfigCallback () {
  saveConfig = true;
}

bool blankScreen(void *) {
  renderer.tft.fillScreen(TFT_BLACK);
  return true;
}

bool updateScreen(void *) {
  JsonDocument json;

  if (crawler.crawl(awtrixApiUrl, json, configManager.getAwtrixAuthUser(), configManager.getAwtrixAuthPass())) {
    if (errorCount > 0) {
      renderer.tft.fillScreen(TFT_BLACK);
      errorCount = 0;
    }

    renderer.drawAwtrixScreen(json);
    timer.in(20, updateScreen);
  } else if (errorCount >= 5) {
    errorCount = 0;
    Serial.println("Crawl failed.");
    renderer.alert("Awtrix: No HTTP response.\nRetrying in 5s", RED);
    timer.in(4800, blankScreen);
    timer.in(5000, updateScreen);
  } else {
    if (errorCount == 0) renderer.hint("Trying " + String(configManager.getAwtrixHostname()), ORANGE);

    errorCount += 1;
    timer.in(0, updateScreen);
  }

  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Awtrix Mini starting");

  renderer.drawBootScreen();
  delay(1000);

  if (!configManager.loadConfig()) {
    Serial.println("Failed to load config, using defaults");
  }

  wifiManagerWrapper.isSetupComplete() ? renderer.addConnectingInfo(wifiManagerWrapper.getSSID()) : renderer.addSetupNote();
  
  wifiManagerWrapper.setupWiFi(AP_NAME, AP_PASSWORD, saveConfigCallback);
  if (saveConfig) {
    configManager.saveConfig(
      wifiManagerWrapper.getCustomHostname().getValue(),
      wifiManagerWrapper.getAwtrixHostname().getValue(),
      wifiManagerWrapper.getAwtrixAuthUser().getValue(),
      wifiManagerWrapper.getAwtrixAuthPass().getValue(),
      wifiManagerWrapper.getMqttBroker().getValue(),
      wifiManagerWrapper.getMqttPort().getValue(),
      wifiManagerWrapper.getMqttUsername().getValue(),
      wifiManagerWrapper.getMqttPassword().getValue(),
      wifiManagerWrapper.getMqttTopic().getValue()
    );
  }

  if (strlen(configManager.getMqttBroker()) > 0) {
    mqttHandler.begin(configManager.getMqttBroker(), atoi(configManager.getMqttPort()));
    while (!mqttHandler.connect(configManager.getAwtrixHostname())) {
      Serial.print(".");
      delay(500);
    }
    Serial.println("Connected to MQTT");
    renderer.alert("MQTT connected.\nTopic: " + String(configManager.getMqttTopic()), GREEN);
    delay(2500);

    String topic = String(configManager.getMqttTopic());
    mqttHandler.subscribe(topic.c_str());
  } else {
    Serial.println("No MQTT broker configured, skipping MQTT setup");
  }

  awtrixApiUrl = String("http://") + configManager.getAwtrixHostname() + "/api/screen";

  timer.in(100, blankScreen);
  timer.in(200, updateScreen);
}

void loop() {
  timer.tick();
  mqttHandler.loop();
}
