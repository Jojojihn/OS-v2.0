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

float Joystick::Axes::getAmplitude() {
    return sqrt(square(x) + square(y));
}

Joystick::Axes Joystick::Axes::normalized() {
    float amplitude = getAmplitude();
    return Axes{x / amplitude, y / amplitude};
}

const Joystick::Axes UP = Joystick::Axes{0,1};
const Joystick::Axes DOWN = Joystick::Axes{0,-1};
const Joystick::Axes LEFT = Joystick::Axes{1,0};
const Joystick::Axes RIGHT = Joystick::Axes{-1,0};

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

bool Joystick::isRoughly(Joystick::Axes direction, float maxAngle) {
    Joystick::Axes vec1 = direction.normalized();
    Joystick::Axes vec2 = getAxes();

    float dotProduct = vec1.x * vec2.x + vec1.y * vec2.y;
    float angle = degrees(acos(dotProduct));

    return angle <= maxAngle;
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
    } else if(amplitude > 1) {
        //Stick sometimes exceeds amplitude of 1 somehow, in that case shorten to amplitude 1
        tempAxes.x = tempAxes.x / amplitude;
        tempAxes.y = tempAxes.y / amplitude;
    }

    axes = tempAxes;

}




