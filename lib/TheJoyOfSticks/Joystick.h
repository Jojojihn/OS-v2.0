/*
  Joystick.h - Easy to use library for getting input from an analog stick
  Copyright (C)2022-2022 IDK Studios, Jatoxo. All right reserved
*/


#ifndef AnalogSticks_h
#define AnalogSticks_h

#include <Arduino.h>


class Joystick {
    public: 
        /**
         * Construct a new Joystick (Analog Stick) object.
         * Make sure to use pins capable of reading analog input for the Axes.
         * Avoid using pin 13 (Built-in LED) for the button, as there are complications with PULLUP
         * 
         * @param axisX Pin for the X-Axis of the stick
         * @param axisY Pin for the Y-Axis of the stick
         * @param button Pin for the stick button (Connected to ground)
         */
        Joystick(byte axisX, byte axisY, byte button, float deadzone = 0.15);

        /**
         * Contains x and y values for the joystick axes
         * Values range between -1 and 1
         * Left/Down being -1 and Up/Right being 1
      	 */
        struct Axes {
          float x;
          float y;
        };

        /**
         * Whether the button has been pressed (Off -> On)
         */
        bool pressed();

        /**
         * Whether the button has been released (On -> Off)
         */
        bool released();

        /**
         * Whether the button has changed state (On - Off or Off -> On)
         */
        bool toggled();

        /**
         * Whether the button is currently pressed 
         */
        bool isButtonPressed();


        /**
         * Get the axes of the Analog Stick
         * 
         * @return For each axis -1..0 if right/down, 0..1 if left/up
         */
        Axes getAxes();

        /**
         * Set the highest amplitude of the stick under which it does not report values.
         * This is useful to compensate for stick drift.
         * 
         * 
         * @param deadzone Deadzone between 0 and 1 
         */
        void setDeadzone(float deadzone);

        /**
         * Get the deadzone amplitude
         */
        float getDeadzone();

    private:
        Axes axes;

        byte axisX;
        byte axisY;
        byte button;

        float deadzone;

        bool buttonState;
        bool hasChanged;

        void updateButtonState();   

        void updateAxes();

        

        Joystick();
  
};



#endif