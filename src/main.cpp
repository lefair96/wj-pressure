#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// some unit properties

// Pressure
const float bar = 101325;
const float Pa = 1;
const float MPa = 1000000;
const float kPa = 1000;

// Voltage
const float mV = 1;
const float V = 1000;

// Time
const float s = 1000;
const float ms = 1;

// sensor properties
const float sensor_pressure_range [2] = {20*kPa, 400*kPa};
const float sensor_pressure_responseTime = 1*ms;
const float sensor_pressure_warmUpTime = 20*ms;
const float sensor_pressure_sensitivity = 12.1*mV/kPa; //V/Pa
const float sensor_pressure_errorMax = 5.5*kPa; // +/- 5.5

// display properties
const int display_nCols = 20;
const int display_nRows = 4;

LiquidCrystal_I2C lcd(0x27,display_nCols,display_nRows);  // set the LCD address to 0x27 for a 16 chars and 2 line display
float display_units_pressure = kPa; // so that you can express the readings in any other way
float display_values_pressure = 0;
float sensor_pressure_reading;
float sensor_pressure_voltage;
float sensor_supply_voltage = 4960;
float sensor_nominal_voltage = 5100;
float sensor_ampfactor;

float display_values_average_len = 5;
float display_values_average = 0;

// A5 for the analog reading of pressure
int sensor_pressure_pin = A5;



void setup() {


  // currently using pin 13 to supply voltage to LCD
  pinMode(13,OUTPUT);

  pinMode(sensor_pressure_pin, INPUT);

  digitalWrite(13,HIGH);
  // initialize the LCD
  lcd.init();

  // Turn on the blacklight and print a message.
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("WatAJet - WJprobe v1");

  lcd.setCursor(0, 1);
  lcd.print("====================");



}

void loop(){
  // Do nothing here...


  if(millis()%100 == 0) // every 500 ms, 2 times per second
  {
    sensor_pressure_voltage = map(analogRead(sensor_pressure_pin), 0, 1023, 0, 5000);

    sensor_ampfactor = sensor_nominal_voltage/sensor_supply_voltage;

    sensor_pressure_reading = sensor_pressure_voltage*sensor_ampfactor/sensor_pressure_sensitivity;
    display_values_pressure = sensor_pressure_reading/display_units_pressure;

    lcd.setCursor(0,2);
    lcd.print("mV:");
    lcd.print(sensor_pressure_voltage);
    lcd.print("    "); // to clear previous numbers

    lcd.setCursor(0,3);
    lcd.print("P(kPa):");
    lcd.print(display_values_pressure);
    lcd.print("    "); // to clear previous numbers

  }

}
