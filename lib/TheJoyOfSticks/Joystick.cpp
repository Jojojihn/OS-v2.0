#include "Joystick.h"


Joystick::Joystick(byte axisX, byte axisY, byte button, float deadzone) :
    axes{512,512},
    axisX(axisX),
    axisY(axisY),
    button(button),
    deadzone(deadzone),
    buttonState(false),
    hasChanged(false)
{
    pinMode(button, INPUT_PULLUP);

    deadzone = max(min(0, deadzone), 0.95);
}

bool Joystick::pressed() {
    updateButtonState();

    return hasChanged && buttonState;
}

bool Joystick::released() {
    updateButtonState();
    
    return hasChanged && !buttonState;
}

bool Joystick::toggled() {
    updateButtonState();

    return hasChanged;
}

bool Joystick::isButtonPressed() {
    updateButtonState();
    return buttonState;
}

void Joystick::updateButtonState() {
    bool newState = digitalRead(button) == LOW;
    hasChanged = newState != buttonState;
    
    buttonState = newState;
}

Joystick::Axes Joystick::getAxes() {
    updateAxes();
    return axes;
}

void Joystick::setDeadzone(float deadzone) {
    this->deadzone = deadzone;
}

float Joystick::getDeadzone() {
    return deadzone;
}


///Updates the axes variable with the latest data (Respects deadzone)
void Joystick::updateAxes() {
    Axes tempAxes{0,0};

    int x = analogRead(axisX);
    int y = analogRead(axisY);

    //Get Value between 0 and 511 for each direction
    if(x >= 512) {
        x -= 512;
    } else if(x <= 511) {
        x = 511 - x;
        x = -x;
    }
    //Value between -1 and 1 of how much right / left respectively
    tempAxes.x = (x / 511.0);

    if(y >= 512) {
        y -= 512;
    } else if(y <= 511) {
        y = 511 - y;
        y = -y;
    }
    //Value between -1 and 1 of how much down / up respectively
    tempAxes.y = (y / 511.0);

    //Calculate amplitude
    float amplitude = sqrt(square(tempAxes.x) + square(tempAxes.y));
    
    if(amplitude <= deadzone) {
        tempAxes = {0,0};
    }

    axes = tempAxes;

}




