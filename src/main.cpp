#include <Arduino.h>
#include <SPI.h>
#include "ConfigManager.h"
#include "WiFiManagerWrapper.h"
#include "Renderer.h"
#include "Crawler.h"
#include <arduino-timer.h>
#include <ArduinoJson.h>
#include "Constants.h"


ConfigManager configManager;
WiFiManagerWrapper wifiManagerWrapper;
Crawler crawler;
Renderer& renderer = Renderer::getInstance();

String awtrixHost;

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

  if (crawler.crawl(awtrixHost, json)) {
    renderer.drawAwtrixScreen(json);
    timer.in(0, updateScreen);
  } else {
    Serial.println("Crawl failed.");
  }

  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("AwtrixMINI starting");

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
      wifiManagerWrapper.getAwtrixHostname().getValue()
    );
  }

  awtrixHost = configManager.getAwtrixHostname();

  timer.in(1800, blankScreen);
  timer.in(2000, updateScreen);
}

void loop() {
  timer.tick();
}
