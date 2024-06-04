#include "define.h"

void setup() 
{
  Serial.begin(INITIAL_DELAY);
  // menu();

  buzzer.toggleon();

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
  buzzer.toggle();

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
  buzzer.toggle();
}

void loop() 
{
  blinkInit();
  while(!fingerFlag)
  {
    blinkInit();
    setPeriod = 220;
    tmr.setPeriod(setPeriod);
    if(tmr.ready())
    {
      getFingerprintID();
      rfidCheck();
      hazard();
    }
  }

  tmrDisplay.setPeriod(1000);
  if(tmrDisplay.ready())
  {
    display();
  }
  parkingFlag();
  parkingFront();
  // parkingBack();
  iluminare();

    if(tmr.ready())
  {
    if(lockButton.click())
    {
      fingerFlag = false;
      sistemOff();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Authorisation needed");
      accesDenied();
    }
  }
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

void hazard()
{
  if(hazardLightPossition.hold() && !hazardBeam.getState())
  {
    rightIndicator.toggleon();
    hazardBeam.toggleon();
    Serial.print(hazardBeam.getState());
    Serial.println(" | Hazard Indicator");
  }  
  else if (hazardBeam.getState() && !hazardLightPossition.hold())
  {
    hazardBeam.toggleoff();
    rightIndicator.toggleoff();
  }
}

void indicareDirectie()
{
  if(hazardLightPossition.hold() && !hazardBeam.getState())
  {
    leftIndicator.toggleoff();
    rightIndicator.toggleon();
    hazardBeam.toggleon();
    Serial.print(hazardBeam.getState());
    Serial.println(" | Hazard Indicator");
  }
  else if (leftPossition.hold() && !leftIndicator.getState() && !hazardBeam.getState())
  {
    rightIndicator.toggleoff();
    leftIndicator.toggleon();
    Serial.print(leftIndicator.getState());
    Serial.println(" | Left Indicator"); 
  }
  else if (rightPossition.hold() && !rightIndicator.getState() && !hazardBeam.getState())
  {
    leftIndicator.toggleoff();
    rightIndicator.toggleon();
    Serial.print(rightIndicator.getState());
    Serial.println(" | Right Indicator");
  }
  else if ((leftIndicator.getState() || rightIndicator.getState()) && rightPossition.maxPosition() && leftPossition.maxPosition() && !hazardLightPossition.hold())
  {
    rightIndicator.toggleoff();
    leftIndicator.toggleoff();
  }
  else if (hazardBeam.getState() && !hazardLightPossition.hold())
  {
    hazardBeam.toggleoff();
    rightIndicator.toggleoff();
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

void brakePosition()
{
  if(brakeButton.hold())
  {
    brakeLight.toggleon();
  }
  else if (!brakeButton.hold())
  {
    brakeLight.toggleoff();
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
  buzzer.toggleon(); //buzzer 0n positin is off LOL

}

void iluminare()
{
  brakePosition();
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
  if(parkinngModeF.click() && !parkingBModeFlag)
  {
    parkingFModeFlag = !parkingFModeFlag;
    if (parkingFModeFlag) buzzer.toggleoff(); else buzzer.toggleon();
  }
  else if (parkinngModeB.click() && !parkingFModeFlag)
  {
    parkingBModeFlag = !parkingBModeFlag;
    if (parkingBModeFlag) buzzer.toggleoff(); else buzzer.toggleon();
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
  if(parkingFModeFlag && tmrParkingMode.ready())
  {
        tmrBuzzer.setPeriod(round(parctronic.dist(0)*10));
        if(tmrBuzzer.ready())
        {
          buzzerBip();
        }
      }
}
void parkingBack()
{
  if(parkingBModeFlag && tmrParkingMode.ready())
  {
        tmrBuzzer.setPeriod(round(parctronic.dist(2)*10));
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
  lcd.setCursor(0, 2);
  lcd.print("Acces granted");
  lcd.blink();
  delay(1000);
}

void accesDenied()
{
  lcd.setCursor(0, 2);
  lcd.print("Acces denied");
  lcd.blink();
  delay(1000);
  autorisationNeeded();
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
  if (parkingFModeFlag || parkingBModeFlag)
  {
    lcd.print("1");
  }
  else
  {
    lcd.print("0");
  }
  
  if (parkingFModeFlag)
  {
    lcd.setCursor(0, 2);
    lcd.print("F1: ");
    lcd.print(parctronic.dist(0));
    lcd.print(" cm");
    lcd.setCursor(0, 3);
    lcd.print("F2: ");
    lcd.print(parctronic.dist(1));
    lcd.print(" cm");
  }
  if (parkingBModeFlag)
  {
    lcd.setCursor(0, 2);
    lcd.print("B1: ");
    lcd.print(parctronic.dist(2));
    lcd.print(" cm");
    lcd.setCursor(0, 3);
    lcd.print("B2: ");
    lcd.print(parctronic.dist(3));
    lcd.print(" cm");
  }
  
}
//Final

// void menu()
// {
//   Serial.print(rightIndicator.getState());
//   Serial.println(" | 1. Right Indicator : pin 31");

//   Serial.print(leftIndicator.getState());
//   Serial.println(" | 2. Left Indicator : pin 33");

//   Serial.println("0 | 3. Hazard ");

//   Serial.print(lowBeam.getState());
//   Serial.println(" | 4. Low Beam : pin 35");

//   Serial.print(highBeam.getState());
//   Serial.println(" | 5. High Beam : pin 37");

//   Serial.print(fogLight.getState());
//   Serial.println(" | 6. Fog Light : pin 39");

//   Serial.print(brakeLight.getState());
//   Serial.println(" | 7. Brake Light : pin 41");

//   Serial.print(gabarit.getState());
//   Serial.println(" | 8. Gabarit : pin 43");
// }
