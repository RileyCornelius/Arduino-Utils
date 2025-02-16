#include <Arduino.h>
#include <Button.h>
#include <SimpleButton.h>
#include <CallbackButton.h>

static Button button;
static SimpleButton simpleButton;
static CallbackButton callbackButton;

void buttonTest()
{
    button.check();

    if (button.released())
    {
        Serial.println("Button released");
    }

    if (button.pressed())
    {
        Serial.println("Button pressed");
    }

    if (button.clicked())
    {
        Serial.println("Button clicked 1");
    }

    if (button.doubleClicked())
    {
        Serial.println("Button clicked 2");
    }

    if (button.tripleClicked())
    {
        Serial.println("Button clicked 3");
    }

    if (button.longPressed())
    {
        Serial.println("Button long pressed");
    }

    if (button.longPressReleased())
    {
        Serial.println("Button long press released");
    }
}

void simpleButtonTest()
{
    if (simpleButton.pressed())
    {
        Serial.println("SimpleButton pressed");
    }
}

void buttonSetup()
{
    button.init(2);
    simpleButton.init(3);
    callbackButton.init(4);

    callbackButton.setClickedCallback([]()
                                      { Serial.println("Button clicked"); });
    callbackButton.setDoubleClickedCallback([]()
                                            { Serial.println("Button double clicked"); });
    callbackButton.setTripleClickedCallback([]()
                                            { Serial.println("Button triple clicked"); });
    callbackButton.setLongPressedCallback([]()
                                          { Serial.println("Button long pressed"); });
    callbackButton.setLongPressReleasedCallback([]()
                                                { Serial.println("Button long press released"); });
    callbackButton.setPressedCallback([]()
                                      { Serial.println("Button pressed"); });
    callbackButton.setReleasedCallback([]()
                                       { Serial.println("Button released"); });
}

void buttonLoop()
{
    buttonTest();
    simpleButtonTest();
    callbackButton.check();
}