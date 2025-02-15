#include <Arduino.h>
#define STATE_MACHINE_DEBUG_PRINT 1
#include <SimpleStateMachine.h>

enum LedEnumState
{
    LedOn,
    LedOff,
};

enum class LedClassState
{
    On,
    Off,
};

using LedStringState = const char *;
LedStringState ledOn = "Led On";
LedStringState ledOff = "Led Off";

SimpleStateMachine<LedEnumState> ledEnumState(LedOff);
SimpleStateMachine<LedClassState> ledClassState(LedClassState::Off);
SimpleStateMachine<LedStringState> ledStringState(ledOff);

void ledStringCallback(LedStringState from, LedStringState to)
{
    Serial.printf("LedStringState: %s => %s\n", from, to);
}
SimpleStateMachine<LedStringState> ledStringCallbackState(ledOff, ledStringCallback);

void ledEnumCallback(LedEnumState from, LedEnumState to)
{
    static const char *ledEnumStateNames[] = {
        "LedOn",
        "LedOff",
    };

    Serial.printf("LedEnumState: %s => %s\n", ledEnumStateNames[from], ledEnumStateNames[to]);
}
SimpleStateMachine<LedEnumState> ledEnumStateCallbackState(LedOff, ledEnumCallback);

void simpleStateMachineSetup()
{
}

void simpleStateMachineLoop()
{
    ledEnumState.set(LedOn);
    ledClassState.set(LedClassState::On);
    ledStringState.set(ledOn);
    ledStringCallbackState.set(ledOn);
    ledEnumStateCallbackState.set(LedOn);
    delay(1000);
    ledEnumState.set(LedOff);
    ledClassState.set(LedClassState::Off);
    ledStringState.set(ledOff);
    ledStringCallbackState.set(ledOff);
    ledEnumStateCallbackState.set(LedOff);
    delay(1000);
}
