#include "Renderer.h"
#include "Constants.h"

#define ORANGE 0xfc00
#define BLUE 0x05b9
#define WHITE 0xffff
#define BLACK 0x0000
#define RED 0xc800
#define DARKGREY 0x39c7

#define RECT_SIZE 8
#define RECT_SPACING 1
#define AWTRIX_WIDTH 32
#define DATA_LENGTH 256
#define HORIZONTAL_OFFSET 16
#define VERTICAL_OFFSET 50

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

void Renderer::drawAwtrixScreen(JsonDocument& json) {
  unsigned long startTime = millis();
  if (!json.is<JsonArray>()) {
      Serial.println("Invalid color data: Expected a JSON array.");
      return;
  }

  JsonArray colors = json.as<JsonArray>();

  uint16 x = HORIZONTAL_OFFSET;
  uint8 y = VERTICAL_OFFSET;

  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint16_t rgb565;
  uint32_t color;
  const uint8_t stepSize = RECT_SIZE + RECT_SPACING;

  for (size_t i = 0; i < DATA_LENGTH; ++i) {
    color = colors[i];

    red = (color >> 16) & 0xFF;
    green = (color >> 8) & 0xFF;
    blue = color & 0xFF;
    rgb565 = (red & 0xF8) << 8 | (green & 0xFC) << 3 | blue >> 3;

    tft.fillRect(x, y, RECT_SIZE, RECT_SIZE, rgb565);

    x += stepSize;

    if (i % AWTRIX_WIDTH == 31) {
      x = HORIZONTAL_OFFSET;
      y += stepSize;
    }
  }

  Serial.print(millis() - startTime);
  Serial.println(" ms");
}

void Renderer::alert(String message, uint32_t color) {
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 157, 320, 16, color);
  tft.loadFont(FONT_DEFAULT);
  tft.drawString(message, 0, 86, 1);
  tft.unloadFont();
}
