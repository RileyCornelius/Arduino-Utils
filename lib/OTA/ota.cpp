#include "ota.h"
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>
#include <ArduinoOTA.h>

#ifndef ENABLE_OTA
#define ENABLE_OTA 1
#endif

#define HOSTNAME ""
#define WIFI_SSID ""
#define WIFI_PASS ""

#if ENABLE_OTA
#include <map>
static const std::map<ota_error_t, const char *> errorMessages = {
    {OTA_AUTH_ERROR, "Auth Failed"},
    {OTA_BEGIN_ERROR, "Begin Failed"},
    {OTA_CONNECT_ERROR, "Connect Failed"},
    {OTA_RECEIVE_ERROR, "Receive Failed"},
    {OTA_END_ERROR, "End Failed"}};
#endif

bool OTA::connectWifi(const char *ssid, const char *pass)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);

    Serial.println("WiFi connecting..");
    uint16_t timeout = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
        timeout++;
        if (timeout > 50)
        {
            Serial.println("WiFi connection failed!");
            return false;
        }
    }

    Serial.println("Connected to: " + WiFi.SSID());
    Serial.println("IP address: " + WiFi.localIP().toString());

    return true;
}

void OTA::begin(bool connect)
{
#if ENABLE_OTA
    if (connect)
    {
        if (!connectWifi(WIFI_SSID, WIFI_PASS))
            return;
    }

    ArduinoOTA.setHostname(HOSTNAME)
        .onStart([]()
                 { Serial.println("OTA Start"); })
        .onEnd([]()
               { Serial.println("OTA End"); })
        .onProgress([](unsigned int progress, unsigned int total)
                    { Serial.printf("Progress: %u%%\r", (progress / (total / 100))) })
        .onError([](ota_error_t error)
                 { Serial.printf("Error[%u]: %s\n", error, errorMessages[error]); });

    Serial.println("Hostname: " + ArduinoOTA.getHostname());

    ArduinoOTA.begin();
#endif
}

void OTA::handle()
{
#if ENABLE_OTA
    if (WiFi.status() == WL_CONNECTED)
        ArduinoOTA.handle();
#endif
}
#endif