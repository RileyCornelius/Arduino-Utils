#pragma once
#include <Arduino.h>
#include <unordered_map>
#include <memory>
#include <functional>
#include <type_traits>

class ServiceLocator
{
private:
    std::unordered_map<int, std::shared_ptr<void>> instances;
    std::unordered_map<int, std::function<std::shared_ptr<void>()>> servicesFactory;

public:
    ServiceLocator() {}
    ~ServiceLocator() { clear(); }

    template <typename T>
    bool registerService()
    {
        int hash = static_cast<int>(typeid(T).hash_code());
        std::shared_ptr<T> instance = std::make_shared<T>();
        instances.emplace(hash, instance);
    };

    template <typename T, typename... Args>
    void registerService(Args... args)
    {
        int hash = static_cast<int>(typeid(T).hash_code());
        std::shared_ptr<T> instance = std::make_shared<T>(args...);
        instances.emplace(hash, instance);
    }

    template <typename T>
    bool registerServiceFactory()
    {
        std::function<std::shared_ptr<T>()> factory = []()
        { return std::make_shared<T>(); };

        int hash = static_cast<int>(typeid(T).hash_code());
        servicesFactory.emplace(hash, factory);
    }

    template <typename T>
    std::shared_ptr<T> get() const
    {
        int hash = static_cast<int>(typeid(T).hash_code());
        auto itr1 = instances.find(hash);
        if (itr1 != instances.end())
            return std::static_pointer_cast<T>(itr1->second);

        auto itr2 = servicesFactory.find(hash);
        if (itr2 != servicesFactory.end())
            return std::static_pointer_cast<T>(itr2->second());

        return nullptr;
    }

    template <typename T>
    inline T &getRef() const
    {
        return *get<T>();
    }

    void clear()
    {
        instances.clear();
        servicesFactory.clear();
    };
};

// Example

ServiceLocator serviceLocator = ServiceLocator();
#define DI serviceLocator

class ABC
{
public:
    void add()
    {
        Serial.println("ABC::add");
    }
};

class Parm
{
public:
    Parm(int a)
    {
        Serial.println(a);
    }
    void add()
    {
        Serial.println("ABC::add");
    }
};
void main()
{
    DI.registerService<ABC>();
    DI.registerService<Parm>(12);

    auto abc = DI.get<ABC>();
    abc->add();

    auto parm = DI.get<Parm>();
    parm->add();

    ABC ref = DI.getRef<ABC>();
    ref.add();
}