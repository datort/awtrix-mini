#ifndef CRAWLER_H
#define CRAWLER_H

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>

class Crawler {
public:
    Crawler();
    ~Crawler();

    bool crawl(const String& apiUrl, JsonDocument& response);

private:
    HTTPClient* httpClient;
    WiFiClient wifiClient;
};

#endif
