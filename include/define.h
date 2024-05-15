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

#define mySerial Serial1
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

#define RIGHT_INDICATOR 31
#define LEFT_INDICATOR 33
#define LOW_BEAM 35
#define HIGH_BEAM 37
#define FOG_LIGHT 39
#define BRAKE_LIGHT 41
#define GABARIT 43
#define INITIAL_DELAY 9600
#define HIGH_LOW_BEAM_PIN A0
#define LEFT_RIGHT_PIN A1
#define LIGHT_POSSITION_PIN A2
#define GABARIT_POSSITION_PIN A3
#define FOG_LIGHT_PIN 8
#define HAZARD_LIGHT_PIN 7
#define LOCK_BUTTON 9
//Parctronic
#define BUTTON_PARCTRONIC 4
#define TRIGER_PARCTRONIC 3
#define PARCTRONIC_SENSORS 4
#define ECHO_PARCTRONIC1 10
#define ECHO_PARCTRONIC2 11
#define ECHO_PARCTRONIC3 12
#define ECHO_PARCTRONIC4 13
#define BUZZER 5

bool parkingModeFlag = false;
Timer tmrParkingMode, tmrBuzzer;
int parkingModePeriod = 100;

HCSR04 parctronic(TRIGER_PARCTRONIC, new int[PARCTRONIC_SENSORS]{ECHO_PARCTRONIC1, ECHO_PARCTRONIC2, ECHO_PARCTRONIC3, ECHO_PARCTRONIC4}, PARCTRONIC_SENSORS);
//Final
//RFID
#define RST_PIN 5
#define SS_PIN 53  

MFRC522 rfidscanner(SS_PIN, RST_PIN); 
//Final
bool fingerFlag = false;
bool hazardFlag = false;
bool sistemStartFlag = false;
int setPeriod = 0;
int directionPeriod = 750;

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

Timer tmr, tmrDirection;


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
