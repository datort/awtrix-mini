#ifndef CRAWLER_H
#define CRAWLER_H

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

class Crawler {
public:
    // Method to crawl and fetch color data from the given hostname
    bool crawl(const String& hostname, JsonArray& outputArray);

private:
    WiFiClient client; // WiFi client for HTTP requests
};

#endif