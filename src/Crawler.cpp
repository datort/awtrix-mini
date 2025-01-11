#include "Crawler.h"

bool Crawler::crawl(const String& hostname, JsonArray& outputArray) {
    String url = "http://" + hostname + "/api/screen";
    Serial.println(url);

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

    JsonDocument jsonDoc;
    DeserializationError error = deserializeJson(jsonDoc, payload);

    if (error) {
        Serial.printf("JSON deserialization failed: %s\n", error.c_str());
        return false;
    }

    if (!jsonDoc.is<JsonArray>()) {
        Serial.println("Invalid JSON format: Expected an array.");
        return false;
    }

    JsonArray array = jsonDoc.as<JsonArray>();
    for (JsonVariant v : array) {
        outputArray.add(v);
    }

    return true;
}
