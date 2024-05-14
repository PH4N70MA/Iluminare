#include "manetaAnalogSignals.h"
#include "manetaDigitalSignals.h"
#include "relay.h"
#include "timer.h"

#include "define.h"

Relay leftIndicator(LEFT_INDICATOR);
Relay rightIndicator(RIGHT_INDICATOR);
Relay lowBeam(LOW_BEAM);
Relay highBeam(HIGH_BEAM);
Relay fogLight(FOG_LIGHT);
Relay brakeLight(BRAKE_LIGHT);
Relay gabarit(GABARIT);

ManetaAnalogic highToLowBeam(HIGH_LOW_BEAM_PIN, HIGH_LOW_MIN_VAL, HIGH_LOW_MAX_VAL, NOT_USED);
ManetaAnalogic leftPossition(LEFT_RIGHT_PIN, LEFT_MIN_VAL, LEFT_MAX_VAL, NOT_USED);
ManetaAnalogic rightPossition(LEFT_RIGHT_PIN, RIGHT_MIN_VAL, RIGHT_MAX_VAL, NOT_USED);
ManetaAnalogic lowBeamPossition(LIGHT_POSSITION_PIN, LOW_GABARIT_BEAM_MIN_VAL, LOW_GABARIT_BEAM_MAX_VAL, NOT_USED);
ManetaAnalogic light(LIGHT_POSSITION_PIN, LIGHT_AUTO_MIN_VAL, LIGHT_AUTO_MAX_VAL, NOT_USED);
ManetaAnalogic gabaritPossition(GABARIT_POSSITION_PIN, LOW_GABARIT_BEAM_MIN_VAL, LOW_GABARIT_BEAM_MAX_VAL, NOT_USED);
ManetaAnalogic autoPossition(GABARIT_POSSITION_PIN, LIGHT_AUTO_MIN_VAL, LIGHT_AUTO_MAX_VAL, NOT_USED);
ManetaDigital fogLightPossition(FOG_LIGHT_PIN);


void menu();
void iluminare();

void setup() 
{
  Serial.begin(INITIAL_DELAY);
  menu();
}

void loop() 
{
  iluminare();
}

void blinkInit()
{
  rightIndicator.blink();
  leftIndicator.blink();
}

void hightoLowBeam()
{
  if (highToLowBeam.click() && (highBeam.getState() || lowBeam.getState()))
  {
    lowBeam.toggle();
    highBeam.toggle();
  }
}

void leftCotireChecker()
{
  if (leftPossition.hold() && !leftIndicator.getState())
  {
    leftIndicator.toggle();
    Serial.print(leftIndicator.getState());
    Serial.println(" | Left Indicator");
  }
  else if (leftIndicator.getState() && leftPossition.maxPosition())
  {
    leftIndicator.toggle();
    Serial.print(leftIndicator.getState());
    Serial.println(" | Left Indicator");
  }
}

void rightCotireChecker()
{
  if (rightPossition.hold() && !rightIndicator.getState())
  {
    rightIndicator.toggle();
    Serial.print(rightIndicator.getState());
    Serial.println(" | Right Indicator");
  }
  else if (rightIndicator.getState() && rightPossition.maxPosition())
  {
    rightIndicator.toggle();
    Serial.print(rightIndicator.getState());
    Serial.println(" | Right Indicator");
  }
}

void gabaritPossitionChecker()
{
  if (gabaritPossition.hold() && (!gabarit.getState() || lowBeam.getState() || highBeam.getState()) && lowBeamPossition.maxPosition())
  {
    gabarit.toggleon();  
    lowBeam.toggleoff();
    highBeam.toggleoff();                      
    Serial.print(gabarit.getState());
    Serial.println(" | Gabarit");
  }
}

void fogPossitionChecker()
{
  if (fogLightPossition.hold() && !fogLight.getState())
  {
    fogLight.toggle();
    Serial.print(fogLight.getState());
    Serial.println(" | Fog Light");
  }
  if (fogLight.getState() && !fogLightPossition.hold())
  {
    fogLight.toggleoff();
    Serial.print(fogLight.getState());
    Serial.println(" | Fog Light");
  }
}

void lightPossitionChecker()
{
  if (lowBeamPossition.hold() && !lowBeam.getState() && !highBeam.getState() && gabaritPossition.maxPosition())
  {
    lowBeam.toggle();
    Serial.print(lowBeam.getState());
    Serial.println(" | Light on");
  }
}

void offPositionChecker()
{
  if ((lowBeam.getState() || highBeam.getState() || gabarit.getState()) && light.hold() && gabaritPossition.maxPosition())
  {
    lowBeam.toggleoff();
    highBeam.toggleoff();
    gabarit.toggleoff();
    Serial.print(lowBeam.getState());
    Serial.println(" | Light off");
  }
}

void iluminare()
{
  blinkInit();
  hightoLowBeam();
  leftCotireChecker();
  rightCotireChecker();
  gabaritPossitionChecker();
  lightPossitionChecker();
  fogPossitionChecker();
  offPositionChecker();
}

void menu()
{
  Serial.print(rightIndicator.getState());
  Serial.println(" | 1. Right Indicator : pin 31");

  Serial.print(leftIndicator.getState());
  Serial.println(" | 2. Left Indicator : pin 33");

  Serial.println("0 | 3. Hazard ");

  Serial.print(lowBeam.getState());
  Serial.println(" | 4. Low Beam : pin 35");

  Serial.print(highBeam.getState());
  Serial.println(" | 5. High Beam : pin 37");

  Serial.print(fogLight.getState());
  Serial.println(" | 6. Fog Light : pin 39");

  Serial.print(brakeLight.getState());
  Serial.println(" | 7. Brake Light : pin 41");

  Serial.print(gabarit.getState());
  Serial.println(" | 8. Gabarit : pin 43");
}