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
void rfidCheck();
void autorisationNeeded();
void accesGranted();
void accesDenied();
void display();

void setup() 
{
  Serial.begin(INITIAL_DELAY);
  menu();

  tmrDisplay.setPeriod(DISPLAY_PERIOD);
  lcd.init();
  lcd.backlight();
  autorisationNeeded();




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
    if(tmrDisplay.ready())
    {
      autorisationNeeded();
    }
    setPeriod = 200;
    tmr.setPeriod(setPeriod);
    if(tmr.ready())
    {
      getFingerprintID();
      rfidCheck();
    }
  }

  if(tmr.ready())
  {
    if(lockButton.click())
    {
      fingerFlag = false;
      sistemOff();
      Serial.println("Acces denied");
      accesDenied();
    }
  }
  tmrDisplay.setPeriod(1000);
  if(tmrDisplay.ready())
  {
    display();
  }
  parkingFlag();
  parkingFront();
  parkingBack();
  iluminare();
}

bool getFingerprintID() 
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
  {
    return fingerFlag=false;
  }  

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
  {
    return fingerFlag=false;
  }  

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
    accesDenied();
    return fingerFlag=false;
  }  

  // found a match!
  accesGranted();
  return fingerFlag = true;
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
void rfidCheck()
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
  if (content.substring(1) == "A3 81 D5 20") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Authorized access");
    Serial.println();
    accesGranted();
    fingerFlag = true;
  }
 
 else   {
    Serial.println(" Access denied");
    accesDenied();
    fingerFlag = false;
  }
}
//FINAL

//Display

void autorisationNeeded()
{
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Authorisation needed");
  lcd.blink();
}

void accesGranted()
{
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Authorisation needed");
  lcd.print("Acces granted");
  lcd.blink();
}

void accesDenied()
{
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Authorisation needed");
  lcd.print("Acces denied");
  lcd.blink();
}

void display()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.noBlink();
  if (rightIndicator.getState() && leftIndicator.getState())
  {
    lcd.print("Avareica: ");
    lcd.write(127);
    lcd.print(" ");
    lcd.write(126);
  }
  else if (rightIndicator.getState())
  {
    lcd.print("Directie: ");
    lcd.write(126);
  }
  else if (leftIndicator.getState())
  {
    lcd.print("Directie: ");
    lcd.write(127);
  }
  else
  {
  lcd.print("Directie: ");
  lcd.print("0");
  }

  lcd.setCursor(0, 1);
  lcd.print("Lumina: ");
  if (!lowBeam.getState() && !highBeam.getState() && gabarit.getState())
  {
    lcd.print("Side");
  }
  else if (!lowBeam.getState() && highBeam.getState() && gabarit.getState())
  {
    lcd.print("High");
  }
  else if (lowBeam.getState() && !highBeam.getState() && gabarit.getState())
  {
    lcd.print("Low");
  }
  else
  {
    lcd.print("Off");
  }

  lcd.setCursor(13, 1);
  lcd.print("FOG:");
  if (fogLight.getState())
  {
    lcd.print("On");
  }
  else
  {
    lcd.print("Off");
  }

  lcd.setCursor(14, 0);
  lcd.print("P: ");
  if (parkingModeFlag)
  {
    lcd.print("1");
  }
  else
  {
    lcd.print("0");
  }
  
  if (parkingModeFlag)
  {
    lcd.setCursor(0, 2);
    lcd.print("F: ");
    lcd.print((parctronic.dist(0) + parctronic.dist(1))/2);
    lcd.print(" cm");
    lcd.setCursor(0, 3);
    lcd.print("B: ");
    lcd.print((parctronic.dist(2) + parctronic.dist(3))/2);
    lcd.print(" cm");
  }
  
}
//Final

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