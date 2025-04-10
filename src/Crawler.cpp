#include "Crawler.h"

Crawler::Crawler() : httpClient(new HTTPClient()), wifiClient() {
}

Crawler::~Crawler() {
    delete httpClient;
}

bool Crawler::crawl(const String& apiUrl, JsonDocument& response) {
    if (!httpClient->begin(wifiClient, apiUrl)) {
        Serial.println("Failed to connect to the server.");
        return false;
    }

    httpClient->setTimeout(1500);

    int httpCode = httpClient->GET();
    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("HTTP GET failed, error: %d\n", httpCode);
        httpClient->end();
        return false;
    }

    WiFiClient& stream = httpClient->getStream();
    DeserializationError error = deserializeJson(response, stream);
    httpClient->end();

    if (error) {
        Serial.printf("JSON deserialization failed: %s\n", error.c_str());
        return false;
    }

    return true;
}
