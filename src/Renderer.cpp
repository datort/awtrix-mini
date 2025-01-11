#include "Renderer.h"
#include "Constants.h"

#define ORANGE 0xfc00
#define BLUE 0x05b9
#define WHITE 0xffff
#define BLACK 0x0000
#define RED 0xc800
#define DARKGREY 0x39c7

Renderer& Renderer::getInstance() {
  static Renderer instance;
  return instance;
}

Renderer::Renderer() : tft() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
}

void Renderer::drawBootScreen() {  
  tft.fillScreen(TFT_BLACK);

  tft.loadFont(FONT_LARGE);
  tft.setTextColor(WHITE);
  tft.drawString("Awtrix", 5, 20);
  tft.setTextColor(RED);
  tft.drawString("MINI", 86, 20);
  tft.unloadFont();
  tft.loadFont(FONT_DEFAULT);
  tft.setTextColor(WHITE);
  tft.drawString(APP_VERSION, 5, 48);
  tft.unloadFont();
}

void Renderer::addSetupNote() {
  tft.loadFont(FONT_DEFAULT);
  tft.setTextColor(WHITE);
  tft.drawString("Wifi connect to:", 5, 90);
  tft.drawString(AP_NAME, 5, 120);
  tft.drawString(AP_PASSWORD, 5, 145);
}

void Renderer::addConnectingInfo(String ssid) {
  tft.loadFont(FONT_DEFAULT);
  tft.setTextColor(WHITE);
  tft.drawString("Connecting to Wifi", 5, 90);
  tft.drawString(ssid, 5, 120);
}

void Renderer::drawLayout() {  
  tft.fillScreen(TFT_BLACK);

  // Clock & Date Background
  tft.fillRect(0, 0, 240, 54, DARKGREY);
  tft.fillRect(0, 54, 240, 26, ORANGE);
}

void Renderer::alert(String message, uint32_t color, uint8_t duration) {
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 0, 240, 40, color);
  tft.setTextDatum(MC_DATUM);
  tft.loadFont(FONT_DEFAULT);
  tft.drawString(message, 120, 120, 1);
  tft.unloadFont();

  delay(duration * 1000);
}
