#include "define.h"

Relay leftIndicator(LEFT_INDICATOR);
Relay rightIndicator(RIGHT_INDICATOR);
Relay lowBeam(LOW_BEAM);
Relay highBeam(HIGH_BEAM);
Relay fogLight(FOG_LIGHT);
Relay brakeLight(BRAKE_LIGHT);
Relay gabarit(GABARIT);
Relay buzzer(BUZZER);

ManetaAnalogic highToLowBeam(HIGH_LOW_BEAM_PIN, HIGH_LOW_MIN_VAL, HIGH_LOW_MAX_VAL, NOT_USED);
ManetaAnalogic leftPossition(LEFT_RIGHT_PIN, LEFT_MIN_VAL, LEFT_MAX_VAL, NOT_USED);
ManetaAnalogic rightPossition(LEFT_RIGHT_PIN, RIGHT_MIN_VAL, RIGHT_MAX_VAL, NOT_USED);
ManetaAnalogic lowBeamPossition(LIGHT_POSSITION_PIN, LOW_GABARIT_BEAM_MIN_VAL, LOW_GABARIT_BEAM_MAX_VAL, NOT_USED);
ManetaAnalogic light(LIGHT_POSSITION_PIN, LIGHT_AUTO_MIN_VAL, LIGHT_AUTO_MAX_VAL, NOT_USED);
ManetaAnalogic gabaritPossition(GABARIT_POSSITION_PIN, LOW_GABARIT_BEAM_MIN_VAL, LOW_GABARIT_BEAM_MAX_VAL, NOT_USED);
ManetaAnalogic autoPossition(GABARIT_POSSITION_PIN, LIGHT_AUTO_MIN_VAL, LIGHT_AUTO_MAX_VAL, NOT_USED);

ManetaDigital fogLightPossition(FOG_LIGHT_PIN);
ManetaDigital hazardLightPossition(HAZARD_LIGHT_PIN);
ManetaDigital lockButton(LOCK_BUTTON);
ManetaDigital parkinngMode(BUTTON_PARCTRONIC);

void menu();
void iluminare();
bool getFingerprintID();
void sistemOff();
void parkingFlag();
void parkingFront();
void parkingBack();
bool rfidCheck();

void setup() 
{
  Serial.begin(INITIAL_DELAY);
  menu();

  //RFID
  SPI.begin(); 
  rfidscanner.PCD_Init();
  Serial.println("Approximate your card to the reader or finger to the sensor");
  //Final 
  tmrParkingMode.setPeriod(parkingModePeriod);
  buzzer.toggleon();

  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
  }
  finger.getTemplateCount();
  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
  tmrDirection.setPeriod(directionPeriod);
}

void loop() 
{
  while(!fingerFlag)
  {
    //FingerPrint
    setPeriod = 200;
    tmr.setPeriod(setPeriod);
    if(tmr.ready())
      getFingerprintID();
    if(tmr.ready())
      rfidCheck();
    //Final
  }

  if(tmr.ready())
  {
    if(lockButton.click())
    {
      fingerFlag = false;
      sistemOff();
      Serial.println("Acces denied");
    }
  }
  parkingFlag();
  parkingFront();
  parkingBack();
  iluminare();
}

bool getFingerprintID() 
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return fingerFlag=false;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return fingerFlag=false;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return fingerFlag=false;

  // found a match!
  return fingerFlag = true;
  Serial.println("Acces granted");
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

void indicareDirectie()
{
  if (rightPossition.maxPosition() && leftPossition.maxPosition() && !hazardFlag && (rightIndicator.getState() || leftIndicator.getState()))
  {
    if(tmrDirection.ready())
    {
      rightIndicator.toggleoff();
      leftIndicator.toggleoff();
    }
  }
  else if (leftPossition.hold() && !leftIndicator.getState())
  {
    if(tmrDirection.ready())
    {
      leftIndicator.toggleon();
      Serial.print(leftIndicator.getState());
      Serial.println(" | Left Indicator"); 
    }
    
  }
  else if (rightPossition.hold() && !rightIndicator.getState())
  {
    if(tmrDirection.ready())
    {
      rightIndicator.toggleon();
      Serial.print(rightIndicator.getState());
      Serial.println(" | Right Indicator");
    }
  }
  else if (hazardLightPossition.hold() && rightPossition.maxPosition() && leftPossition.maxPosition() && hazardFlag)
  {
    if(tmrDirection.ready())
    {
      hazardFlag = false;
    }
    
  }
  else if( !hazardLightPossition.hold() && !rightIndicator.getState() && !leftIndicator.getState() && rightPossition.maxPosition() && leftPossition.maxPosition())
  {
    if(tmrDirection.ready())
    {
      hazardFlag = true;
      rightIndicator.toggleon();
      leftIndicator.toggleon();
    }
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
  if ((lowBeamPossition.hold() || autoPossition.hold()) && !lowBeam.getState() && !highBeam.getState())
  {
    lowBeam.toggle();
    gabarit.toggleon();
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

void sistemOff()
{
  rightIndicator.toggleoff();
  leftIndicator.toggleoff();
  lowBeam.toggleoff();
  highBeam.toggleoff();
  fogLight.toggleoff();
  brakeLight.toggleoff();
  gabarit.toggleoff();

}

void iluminare()
{
  blinkInit();
  hightoLowBeam();
  indicareDirectie();
  gabaritPossitionChecker();
  lightPossitionChecker();
  fogPossitionChecker();
  offPositionChecker();
}

//Parctronic
//Sunetul distanta*10 si asta o sa fie timerul pentru sunet
void parkingFlag()
{
  if(parkinngMode.click())
  {
    parkingModeFlag = !parkingModeFlag;
    buzzer.toggleon();
  }
}
void buzzerBip()
{
  buzzer.toggle(); 
  setPeriod = 100;
  if(tmr.ready())
  {
    buzzer.toggle();  
  }
}
void parkingFront()
{
  if(parkingModeFlag && tmrParkingMode.ready())
  {
        tmrBuzzer.setPeriod(round(((parctronic.dist(0)+parctronic.dist(1))/2)*10));
        if(tmrBuzzer.ready())
        {
          buzzerBip();
        }
      }
}
void parkingBack()
{
  if(parkingModeFlag && tmrParkingMode.ready())
  {
        tmrBuzzer.setPeriod(round(((parctronic.dist(2)+parctronic.dist(3))/2)*10));
        if(tmrBuzzer.ready())
        {
          buzzerBip();
        }
      }
}
//Final

//RFID
bool rfidCheck()
{
    // Look for new cards
  if ( ! rfidscanner.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! rfidscanner.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < rfidscanner.uid.size; i++) 
  {
     Serial.print(rfidscanner.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(rfidscanner.uid.uidByte[i], HEX);
     content.concat(String(rfidscanner.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(rfidscanner.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "FA BD 7E 82") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    return fingerFlag = true;
  }
 
 else   {
    Serial.println(" Access denied");
    return fingerFlag = false;
  }
}
//FINAL

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