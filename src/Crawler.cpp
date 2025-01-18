#include "Crawler.h"

bool Crawler::crawl(const String& hostname, JsonDocument& response) {
  String url = "http://" + hostname + "/api/screen";

  HTTPClient http;

  if (!http.begin(client, url)) {
    Serial.println("Failed to connect to the server.");
    return false;
  }

  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("HTTP GET failed, error: %d\n", httpCode);
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  JsonDocument json;
  DeserializationError error = deserializeJson(json, payload);

  if (error) {
    Serial.printf("JSON deserialization failed: %s\n", error.c_str());
    return false;
  }

  response = json;
  return true;
}

