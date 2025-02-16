#pragma once

class OTA
{
private:
    bool connectWifi(const char *ssid, const char *pass);

public:
    void begin(bool connect = true);
    void handle();
};
