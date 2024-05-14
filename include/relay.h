#pragma once
#include <Arduino.h>
#include "timer.h"

// pentru bascularea merge intervalul 750

class Relay
{
public:
    Relay(byte pin, int period = 750) : __pin(pin) 
    {
        pinMode(__pin, OUTPUT);
        digitalWrite(__pin, true);
        tmr.setPeriod(period);
    }

    void setPeriod(uint16_t prd) {
      tmr.setPeriod(prd);
    }

    uint16_t getPeriod() {
      return (tmr.getPeriod());
    }

    bool getState() {
      return !__state;
    }

    void blink() {
      if (!__state) {
        if (tmr.ready()) {
          __flag = !__flag;
          digitalWrite(__pin, __flag);
        }
      } else {
        if (__flag) digitalWrite(__pin, true);
      }
    }

    void toggle()
    {
      __state = !__state;
      digitalWrite(__pin, __state);
    }

    void toggleoff()
    {
      __state = true;
      digitalWrite(__pin, __state);
    }

    void toggleon()
    {
      __state = false;
      digitalWrite(__pin, __state);
    }

private:
    const byte __pin;
    bool __flag = false, __state = true;
    Timer tmr;
};