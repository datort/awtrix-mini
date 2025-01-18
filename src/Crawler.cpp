#include "Crawler.h"

Crawler::Crawler() : wifiClient(), httpClient(new HTTPClient()) {
    // Constructor
}

Crawler::~Crawler() {
    delete httpClient;
}

bool Crawler::crawl(const String& apiUrl, JsonDocument& response) {
    if (!httpClient->begin(wifiClient, apiUrl)) { // Use member wifiClient
        Serial.println("Failed to connect to the server.");
        return false;
    }

    int httpCode = httpClient->GET();
    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("HTTP GET failed, error: %d\n", httpCode);
        httpClient->end();
        return false;
    }

    String payload = httpClient->getString();
    httpClient->end();

    DeserializationError error = deserializeJson(response, payload);

    if (error) {
        Serial.printf("JSON deserialization failed: %s\n", error.c_str());
        return false;
    }

    return true;
}
