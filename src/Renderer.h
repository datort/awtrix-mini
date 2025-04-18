#ifndef RENDERER_H
#define RENDERER_H

#include <TFT_eSPI.h>
#include <SPI.h>
#include <ArduinoJson.h>

class Renderer {
public:
  static Renderer& getInstance();
  TFT_eSPI tft;

  void drawBootScreen();
  void addSetupNote();
  void addConnectingInfo(String ssid);
  void drawAwtrixScreen(JsonDocument& json);
  void alert(String message, uint32_t color);
  void hint(String message, uint32_t color);

private:
  Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
};

#endif
