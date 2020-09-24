/*
   Copyright (c) 2020 Gabriel Froes. All right reserved.

   InstagramApi - An Arduino wrapper for the Instagram API

 */

#include <ArduinoJson.h> // https://github.com/bblanchon/ArduinoJson
#include <ESP8266HTTPClient.h>
#include "InstagramApi.h"

InstagramApi::InstagramApi(String url, Client &client)
{
    int strLen = url.length() + 1;
    char tempStr[strLen];
    url.toCharArray(tempStr, strLen);

    InstagramApi(tempStr, client);
}

InstagramApi::InstagramApi(char *url, Client &client)
{
    _url = url;
    this->client = &client;
}

bool InstagramApi::getAccountStatistics()
{
    bool wasSuccessful = false;

    HTTPClient http;
    http.begin(_url);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.GET();

    if (httpCode >= 200 and httpCode <= 299)
    {
        // Get from https://arduinojson.org/v6/assistant/
        const size_t capacity = JSON_OBJECT_SIZE(5) + 100;
        DynamicJsonDocument doc(capacity);

        DeserializationError error = deserializeJson(doc, http.getString());
        if (!error)
        {
            wasSuccessful = true;
            accountStats.followersCount = doc["followed_by"].as<long>();
        }
        else
        {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
        }
    }
    else
    {
        Serial.print("Unexpected HTTP Status Code: ");
        Serial.println(httpCode);
    }
    http.end();
    closeClient();

    return wasSuccessful;
}

void InstagramApi::closeClient()
{
    if (client->connected())
    {
        Serial.println(F("Closing client"));
        client->stop();
    }
}