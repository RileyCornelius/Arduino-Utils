#if ESP32
#include <Preferences.h>

template <typename T>
class Config
{
private:
    const char *fileName = "default";

public:
    Config(){};
    Config(const char *name) : fileName(name)
    {
    }
    bool save(T config)
    {
        Preferences prefs;
        if (prefs.begin(fileName, false))
        {
            prefs.putBytes("config", (const void *)config, sizeof(config));
            return true;
        }
        else
        {
            return false;
        }
    }
    T load()
    {
        Preferences prefs;
        if (prefs.begin(fileName, true))
        {
            T config;
            memset(&config, 0, sizeof(config));
            prefs.getBytes("config", &config, sizeof(config));
            return config;
        }
    }
};
#endif