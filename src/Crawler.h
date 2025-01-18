#ifndef CRAWLER_H
#define CRAWLER_H

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

class Crawler {
public:
    bool crawl(const String& hostname, JsonDocument& response);

private:
    WiFiClient client;
};

#endif
