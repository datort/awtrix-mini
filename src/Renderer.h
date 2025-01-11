#ifndef RENDERER_H
#define RENDERER_H

#include <TFT_eSPI.h>
#include <SPI.h>

class Renderer {
public:
  static Renderer& getInstance();
  TFT_eSPI tft;

  void drawLayout();
  void drawBootScreen();
  void addSetupNote();
  void addConnectingInfo(String ssid);
  void alert(String message, uint32_t color, uint8_t duration);

private:
  Renderer();

  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
};

#endif
