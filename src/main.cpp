#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <I2CKeyPad.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// some unit properties.

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
int sensor_pressure_pin = A0;

int button_push_pin = 5;
const int debounce_timer = 100; // 1/10 of second
int debounce_time = 0; // 1/10 of second
bool debounce_set = 0;
bool button_push_toggle = 0;
bool change_state = 0;
int state = 0;

int button_switch_pin = 4;

// the keypad
I2CKeyPad keyPad(0x20);

uint32_t start, stop;
uint32_t lastKeyPressed = 0;

// the temperature sensor
#define SENSOR_BUS_PIN 12

OneWire oneWire(SENSOR_BUS_PIN);
DallasTemperature sensors(&oneWire);


//uint32_t start, stop;
//uint32_t lastKeyPressed = 0;



void setup() {


  pinMode(sensor_pressure_pin, INPUT);
  pinMode(button_push_pin, INPUT);
  pinMode(button_switch_pin, INPUT);

  // initialize the LCD
  lcd.init();



  // Turn on the blacklight and print a message..
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("WatAJet:WJprobe -- 0");

  lcd.setCursor(0, 1);
  lcd.print("====================");

  Wire.begin();
    Wire.setClock(400000);
  keyPad.begin();

  Serial.begin(9600);
   Serial.println("DS18B20 test");
   sensors.begin();


}

void loop(){
  // Do nothing here...

  uint32_t now = millis();
    //char keys[] = "123A456B789C*0#DNF";  // N = Nokey, F = Fail
    char keys[] = "DCBA#9630852*741NF";  // N = Nokey, F = Fail

    if (now - lastKeyPressed >= 100)
    {
      lastKeyPressed = now;

      start = micros();
      uint8_t idx = keyPad.getKey();
      stop = micros();

      if(idx<16)
      {
        lcd.setCursor(19, 0);
        lcd.print(keys[idx]);

      }


    }


  if(digitalRead(button_push_pin))
  {
    if(!debounce_set)
    {
      debounce_set = 1;
      debounce_time = millis();

    }else
    {
      if(millis()-debounce_time > debounce_timer)
      {
        change_state = 1;


      }
    }
  }else
  {
    if(debounce_set)
    {
      if(millis()-debounce_time > debounce_timer)
      {
        change_state = 0;
        debounce_set = 0;
        button_push_toggle = 0;
      }
    }
  }

  if(change_state && !button_push_toggle)
  {
    button_push_toggle = 1;
    state = state + 1;
    if(state > 9)
    {
      state = 0;
    }
    lcd.setCursor(19, 0);
    lcd.print(state);

    change_state = 0;
  }

  if(millis()%100 == 0) // every 500 ms, 2 times per second
  {

    Serial.print("Richiesta temperatura... ");
  //L'esecuzione si blocca sul comando per il tempo richiesto (dipende dalla risoluzione impostata)
  sensors.requestTemperatures(); //Invio comando per leggere temperatura
  //Stampo temperatura del (primo) sensore del bus
  Serial.print(sensors.getTempCByIndex(0));
  Serial.println("°C");


    sensor_pressure_voltage = map(analogRead(sensor_pressure_pin), 0, 1023, 0, 5000);


    sensor_ampfactor = sensor_nominal_voltage/sensor_supply_voltage;

    sensor_pressure_reading = sensor_pressure_voltage*sensor_ampfactor/sensor_pressure_sensitivity;
    display_values_pressure = sensor_pressure_reading/display_units_pressure;

    lcd.setCursor(0,2);
    lcd.print("mV:");
    lcd.print(sensor_pressure_voltage);
    lcd.print("    "); // to clear previous numbers

    lcd.setCursor(0,3);

    if(digitalRead(button_switch_pin))
    {
        lcd.print("P(bar):");
        lcd.print(display_values_pressure*kPa/bar);
    }else
    {
        lcd.print("P(kPa):");
        lcd.print(display_values_pressure);
    }

    lcd.print("    "); // to clear previous numbers

  }

}
