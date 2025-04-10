#include "Renderer.h"
#include "Constants.h"

Renderer& Renderer::getInstance() {
  static Renderer instance;
  return instance;
}

Renderer::Renderer() : tft() {
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
}

void Renderer::drawBootScreen() {  
  tft.fillScreen(TFT_BLACK);

  tft.loadFont(FONT_LARGE);
  tft.setTextColor(WHITE);
  tft.drawString("Awtrix", 5, 20);
  tft.setTextColor(RED);
  tft.drawString("Mini", 89, 20);
  tft.unloadFont();
  tft.loadFont(FONT_DEFAULT);
  tft.setTextColor(WHITE);
  tft.drawString(APP_VERSION, 5, 48);
  tft.unloadFont();
}

void Renderer::addSetupNote() {
  tft.loadFont(FONT_DEFAULT);
  tft.setTextColor(WHITE);
  tft.drawString("Ready, connect to Wifi:", 5, 90);
  tft.drawString(AP_NAME, 5, 120);
  tft.drawString(AP_PASSWORD, 5, 145);
}

void Renderer::addConnectingInfo(String ssid) {
  tft.loadFont(FONT_DEFAULT);
  tft.setTextColor(WHITE);
  tft.drawString("Connecting to Wifi", 5, 90);
  tft.setTextColor(LIGHTBLUE);
  tft.drawString(ssid, 5, 120);
}

void Renderer::hint(String message, uint32_t color) {
  tft.fillRect(0, 0, DISPLAY_WIDTH, 50, BLACK);
  tft.fillRect(0, 0, DISPLAY_WIDTH, 14, color);
  tft.loadFont(FONT_DEFAULT);
  tft.setTextColor(WHITE);
  tft.drawString(message, 0, 20, 1);
  tft.unloadFont();
}

void Renderer::drawAwtrixScreen(JsonDocument& json) {
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
}

void Renderer::alert(String message, uint32_t color) {
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 158, DISPLAY_WIDTH, 16, color);
  tft.loadFont(FONT_DEFAULT);
  tft.drawString(message, 0, 96, 1);
  tft.unloadFont();
}
