#pragma once
#include <Arduino.h>
#include "timer.h"
#define _BTN_DEB_TIME 100

class ManetaDigital {
  public:
    ManetaDigital (byte pin) : _pin(pin) {
      pinMode(_pin, INPUT_PULLUP);
    }
    
    void setPeriod(uint16_t prd) 
    {
      tmr.setPeriod(prd);
    }

    uint16_t getPeriod() 
    {
      return (tmr.getPeriod());
    }

    bool click() 
    {
      bool pinVal = digitalRead(_pin);
      pinVal = !pinVal;
      if (pinVal && !_flag && tmr.ready()) 
      {
        _flag = true;
        return true;
      }
      if (!pinVal && _flag && tmr.ready()) 
      {
        _flag = false;
      }
      return false;
    }

    bool hold() 
    {
      bool pinVal = digitalRead(_pin);
      pinVal = !pinVal;
      if (pinVal && tmr.ready()) 
      {
        return true;
      }
      return false;
    }

  private:
    const byte _pin;
    Timer tmr;
    bool _flag;
};