#include <Arduino.h>
#include <SimpleButton.h>
#include <StateMachine.h>
#include <FSM2.h>

void ledOn();
void ledOff();
void ledBlink();
bool buttonPressed();

void ledOn()
{
    // digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LED ON");
}

void ledOff()
{
    // digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LED OFF");
}

void ledBlink()
{
    static unsigned long lastTime = 0;
    static bool ledState = false;

    if (millis() - lastTime > 500)
    {
        lastTime = millis();
        ledState = !ledState;
        // digitalWrite(LED_BUILTIN, ledState);
        Serial.println("LED BLINKING: " + String(ledState));
    }
}

bool buttonPressed()
{
    static SimpleButton button(2);
    return button.pressed();
}

FsmState LedOn(ledOn, NO_HANDLE, NO_EXIT);
FsmState LedOff(ledOff, NO_HANDLE, NO_EXIT);
FsmState LedBlinking(ledBlink);

StateMachine ledStateMachine(LedOff);

void fsmInitTransitions()
{
    ledStateMachine.addTimedTransition(LedOff, LedOn, 1000);
    ledStateMachine.addTimedTransition(LedOn, LedBlinking, 1000);
    ledStateMachine.addTimedTransition(LedBlinking, LedOff, 5000, buttonPressed);
    // ledStateMachine.addTransition(LedBlinking, LedOff, buttonPressed);
}

enum PlayerEvents
{
    PLAY,
    PAUSE,
    RESUME,
    STOP
};

FSM2::FsmState Idle([]()
                    { Serial.println("idle"); }, NO_HANDLE, NO_EXIT);
FSM2::FsmState Playing(NO_ENTER, []()
                       { Serial.println("playing"); }, NO_EXIT);
FSM2::FsmState Paused(ledBlink);

FSM2::FSM2 playerFSM(Idle);

void playerInitTransitions()

{
    playerFSM.addEventTransition(Idle, Playing, PLAY);
    playerFSM.addEventTransition(Playing, Paused, PAUSE);
    playerFSM.addEventTransition(Playing, Idle, STOP);
    playerFSM.addEventTransition(Paused, Playing, RESUME);
    playerFSM.addEventTransition(Paused, Idle, STOP);
}