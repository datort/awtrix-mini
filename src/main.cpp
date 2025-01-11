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

bool startMirroring(void *) {
  bool success = true;

  renderer.tft.fillScreen(TFT_BLACK);

  JsonDocument doc;
  JsonArray array = doc.to<JsonArray>();

  if (crawler.crawl(awtrixHost, array)) {
    Serial.println("Crawl successful! Retrieved data:");
    /*for (JsonVariant v : array) {
      Serial.println(v.as<int>());
    }*/
  } else {
    Serial.println("Crawl failed.");
  }

  /*while (success) {

  }*/

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

  awtrixHost = wifiManagerWrapper.getAwtrixHostname().getValue();

  timer.in(2000, startMirroring);
}

void loop() {
  timer.tick();
}
