#ifndef CRAWLER_H
#define CRAWLER_H

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

class Crawler {
public:
    Crawler();
    ~Crawler();

    bool crawl(const String& apiUrl, JsonDocument& response, const char* authUser, const char* authPass);

private:
    HTTPClient* httpClient;
    WiFiClient wifiClient;
    String currentUrl;
    bool isConnected;
    
    bool ensureConnection(const String& apiUrl, const char* authUser, const char* authPass);
};

#endif
