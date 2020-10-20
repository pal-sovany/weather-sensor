/*
    Basic software for ESP8266 based weather sensor unit.
    
    Created by Pál Sovány, 2018.
*/

#include <OneWire.h>
#include <DallasTemperature.h>

#define TEMP_SENSOR_BUS 0

OneWire oneWire(TEMP_SENSOR_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  Serial.println();

  sensors.begin();

}

void loop() {
  //Read temperature
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
    
  //Send
  Serial.println(temperature);
  
  delay(10000);
}
