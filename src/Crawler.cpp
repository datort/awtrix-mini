#include "Crawler.h"

Crawler::Crawler() : httpClient(new HTTPClient()), wifiClient(), isConnected(false) {
}

Crawler::~Crawler() {
    if (isConnected) {
        httpClient->end();
    }
    delete httpClient;
}

bool Crawler::ensureConnection(const String& apiUrl, const char* authUser, const char* authPass) {
    if (isConnected && currentUrl == apiUrl) {
        return true;
    }
    
    if (isConnected) {
        httpClient->end();
        isConnected = false;
    }

    if (authUser && authPass && strlen(authUser) > 0 && strlen(authPass) > 0) {
        httpClient->setAuthorization(authUser, authPass);
    }
    
    if (!httpClient->begin(wifiClient, apiUrl)) {
        Serial.println("Failed to connect to the server.");
        return false;
    }
    
    httpClient->setTimeout(2000);
    currentUrl = apiUrl;
    isConnected = true;
    return true;
}

bool Crawler::crawl(const String& apiUrl, JsonDocument& response, const char* authUser, const char* authPass) {
    if (!ensureConnection(apiUrl, authUser, authPass)) {
        return false;
    }

    int httpCode = httpClient->GET();
    if (httpCode != HTTP_CODE_OK) {
        Serial.printf("HTTP GET failed, error: %d\n", httpCode);
        isConnected = false;
        currentUrl = "";
        httpClient->end();
        return false;
    }

    WiFiClient& stream = httpClient->getStream();
    DeserializationError error = deserializeJson(response, stream);

    if (error) {
        Serial.printf("JSON deserialization failed: %s\n", error.c_str());
        return false;
    }

    return true;
}
