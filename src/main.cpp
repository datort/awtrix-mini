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

  if (crawler.crawl(awtrixApiUrl, json)) {
    errorCount = 0;

    renderer.drawAwtrixScreen(json);
    timer.in(0, updateScreen);
  } else if (errorCount >= 5) {
    Serial.println("Crawl failed.");
    renderer.alert("Awtrix: No HTTP response.\nRetrying in 5s", 0xf800);
    timer.in(4800, blankScreen);
    timer.in(5000, updateScreen);
  } else {
    errorCount += 1;
    timer.in(0, updateScreen);
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

  awtrixApiUrl = String("http://") + configManager.getAwtrixHostname() + "/api/screen";

  timer.in(100, blankScreen);
  timer.in(200, updateScreen);
}

void loop() {
  timer.tick();
}
