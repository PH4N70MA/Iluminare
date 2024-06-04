#pragma once

#include "manetaAnalogSignals.h"
#include "manetaDigitalSignals.h"
#include "relay.h"
#include "timer.h"
#include "Adafruit_Fingerprint.h"
#include "HCSR04.h"
#include "SPI.h"
#include "MFRC522.h"
#include "EEPROM.h"
#include "LiquidCrystal_I2C.h"

#define mySerial Serial1
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define RIGHT_INDICATOR 31
#define LEFT_INDICATOR 33
#define LOW_BEAM 35
#define HIGH_BEAM 37
#define FOG_LIGHT 39
#define BRAKE_LIGHT 41
#define GABARIT 43
#define HAZARD 45
#define INITIAL_DELAY 9600
#define HIGH_LOW_BEAM_PIN A0
#define LEFT_RIGHT_PIN A1
#define LIGHT_POSSITION_PIN A2
#define GABARIT_POSSITION_PIN A3
#define FOG_LIGHT_PIN 8
#define HAZARD_LIGHT_PIN 7
#define LOCK_BUTTON 9
#define BRAKE_PIN 2
//Parctronic
#define BUTTON_PARCTRONIC_F 4
#define BUTTON_PARCTRONIC_B 22

#define TRIGER_PARCTRONIC 3
#define PARCTRONIC_SENSORS 4
#define ECHO_PARCTRONIC1 10
#define ECHO_PARCTRONIC2 11
#define ECHO_PARCTRONIC3 12
#define ECHO_PARCTRONIC4 13
#define BUZZER 6                

bool parkingFModeFlag = false;
bool parkingBModeFlag = false;
Timer tmrParkingMode, tmrBuzzer;
int parkingModePeriod = 100;

HCSR04 parctronic(TRIGER_PARCTRONIC, new int[PARCTRONIC_SENSORS]{ECHO_PARCTRONIC1, ECHO_PARCTRONIC2, ECHO_PARCTRONIC3, ECHO_PARCTRONIC4}, PARCTRONIC_SENSORS);
//Final
//RFID
#define RST_PIN 5
#define SS_PIN 53  

MFRC522 rfidscanner(SS_PIN, RST_PIN); 
//Final

#define DISPLAY_ADDRESS 0x27
#define DISPLAY_COLUMNS 20
#define DISPLAY_ROWS 4
#define DISPLAY_PERIOD 2000

Timer tmrDisplay;

LiquidCrystal_I2C lcd(DISPLAY_ADDRESS, DISPLAY_COLUMNS, DISPLAY_ROWS);

bool fingerFlag = false;
bool hazardFlag = false;
bool sistemStartFlag = false;
int setPeriod = 0;

#define NOT_USED 0

#define HIGH_LOW_MIN_VAL 100
#define HIGH_LOW_MAX_VAL 600
#define LEFT_MIN_VAL 100
#define LEFT_MAX_VAL 600
#define RIGHT_MIN_VAL 0
#define RIGHT_MAX_VAL 100
#define LOW_GABARIT_BEAM_MIN_VAL 200
#define LOW_GABARIT_BEAM_MAX_VAL 300
#define LIGHT_AUTO_MIN_VAL 400
#define LIGHT_AUTO_MAX_VAL 700

Timer tmr;


Relay leftIndicator(LEFT_INDICATOR);
Relay rightIndicator(RIGHT_INDICATOR);
Relay lowBeam(LOW_BEAM);
Relay highBeam(HIGH_BEAM);
Relay fogLight(FOG_LIGHT);
Relay brakeLight(BRAKE_LIGHT);
Relay gabarit(GABARIT);
Relay buzzer(BUZZER);
Relay hazardBeam(HAZARD);

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
ManetaDigital parkinngModeF(BUTTON_PARCTRONIC_F);
ManetaDigital parkinngModeB(BUTTON_PARCTRONIC_B);
ManetaDigital brakeButton(BRAKE_PIN);

void display();
void rfidCheck();
void autorisationNeeded();
void accesGranted();
void accesDenied();
void menu();
void iluminare();
bool getFingerprintID();
void sistemOff();
void parkingFlag();
void parkingFront();
void hazard();
void blinkInit();

// void parkingBack();

//Negru - GND
//ROSU - A2
//Galben - 8
//Alb - A3
//MijlocPavarotnic - GND
//SusPavarotnic - A0
//JosPavarotnic - A1
//StopButton - 9
//HazardButton - 7

//ButtonParctronic - 4
//TrigerParctronic - 3
//EchoParctronic1 - 10
//EchoParctronic2 - 11
//EchoParctronic3 - 12
//EchoParctronic4 - 13