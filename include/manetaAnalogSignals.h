#pragma once
#include <Arduino.h>
#include "timer.h"
#define _BTN_DEB_TIME 100

//ManetaAnalogical(int pin, int setpointP1, int setpointP2, int setpoint3)
//ManetaDigital(int pin)

//click() - setpointP1<click<setpointP2 setpointP3=0

class ManetaAnalogic
{
  public:
    ManetaAnalogic (byte pin, int setpointP1, int setpointP2, int setpoint3) : _pin(pin), _setpointP1(setpointP1), _setpointP2(setpointP2), _setpointP3(setpoint3)
    {
      pinMode(_pin, INPUT);
      tmr.setPeriod(_BTN_DEB_TIME);
    }

    void setPoint1(int setpointP1) 
    {
      _setpointP1 = setpointP1;
    }

    void setPoint2(int setpointP2) 
    {
      _setpointP2 = setpointP2;
    }

    void setPoint3(int setpointP3) 
    {
      _setpointP3 = setpointP3;
    }
    
    void setPeriod(uint16_t prd) 
    {
      tmr.setPeriod(prd);
    }

    uint16_t getPeriod() 
    {
      return (tmr.getPeriod());
    }

    int getPoint1() 
    {
      return _setpointP1;
    }

    int getPoint2() 
    {
      return _setpointP2;
    }

    int getPoint3() 
    {
      return _setpointP3;
    }

    bool click() 
    {
      int pinVal = analogRead(_pin);
      if (((_setpointP1 < pinVal) &&  (_setpointP2 > pinVal)) && !_flag && tmr.ready()) 
      {
        _flag = true;
        return true;
      }
      if (!((_setpointP1 < pinVal) &&  (_setpointP2 > pinVal)) && _flag && tmr.ready()) 
      {
        _flag = false;
      }
      return false;
    }

    bool hold() 
    {
      int pinVal = analogRead(_pin);
      if ((_setpointP1 <= pinVal) && (_setpointP2 >= pinVal) && tmr.ready()) 
      {
        return true;
      }
      return false;
    }

    bool maxPosition() 
    {
      int pinVal = analogRead(_pin);
      if ( pinVal > 1000 && tmr.ready()) 
      {
        return true;
      }
      return false;
    }

  private:
    const byte _pin;
    int _setpointP1, _setpointP2, _setpointP3;
    Timer tmr;
    bool _flag;
};

