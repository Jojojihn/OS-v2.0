#include "NoSwitches.h"



Button::Button(int pin) {
    initialized = false;
    buttonPin = pin;
    activeState = LOW;
    buttonState = false;
    hasChanged = false;
}


void Button::init() {
    initialized = true;
    if(buttonPin == LED_BUILTIN) {
        Serial.print(F("[NoSwitches](WARNING): Pin "));
        Serial.print(buttonPin);
        Serial.println(F(" is used for internal LED. Button will always be LOW"));
    }

    pinMode(buttonPin, INPUT_PULLUP);
}

bool Button::justPressed() {
    updateButtonState();
    return buttonState && hasChanged;
}

bool Button::justReleased() {
    updateButtonState();
    return !buttonState && hasChanged;
}

bool Button::toggled() {
    updateButtonState();
    return hasChanged;
}

bool Button::isPressed() {
    updateButtonState();
    return buttonState;
}

void Button::updateButtonState() {
    if(!initialized) {
        init();
    }
    bool newState = digitalRead(buttonPin) == activeState;
    hasChanged = newState != buttonState;

    buttonState = newState;
}