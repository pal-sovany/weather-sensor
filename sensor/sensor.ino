/*
    Basic software for ESP8266 based weather sensor unit.
    Periodically reads sensor data and sends it to a remote server via HTTP.
    
    Created by Pál Sovány, 2018.
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include "DHTesp.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

#define TEMP_SENSOR_BUS 0
#define HUMIDITY_SENSOR_BUS 14

const char* ssid = "";
const char* password = "";

const char* host = "192.168.0.22";
const int port = 5000;

DHTesp dht;
OneWire oneWire(TEMP_SENSOR_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  dht.setup(HUMIDITY_SENSOR_BUS, DHTesp::DHT11);
  sensors.begin();

}

void loop() {
  //Read humidity
  float humidity = dht.getHumidity();

  //Read temperature
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
    
  //Send
  send(temperature, humidity);
  delay(10000);
}

void send(float temperature, float humidity, float airPressure) {
  WiFiClient client;
  HTTPClient http;

  http.begin("http://" + String(host) + ":" + String(port) + "/sensorData");
  http.addHeader("Content-Type", "application/json");
  http.POST("{\"temperature\": " + String(temperature) + " ,\"humidity\": " + String(humidity) + " }");
  http.end();
  
  Serial.println("request sent: {\"temperature\": " + String(temperature) + " ,\"humidity\": " + String(humidity) + " ,\"airPressure\": " + String(airPressure) + " }");
}
