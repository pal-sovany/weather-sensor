/*
    Basic software for ESP8266 based weather sensor unit.
    
    Created by Pál Sovány, 2018.
*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define TEMP_SENSOR_BUS 0

OneWire oneWire(TEMP_SENSOR_BUS);
DallasTemperature sensors(&oneWire);

const char* ssid = "...";
const char* password = "...";
const char* mqtt_server = "...";
const char* topic = "test";

WiFiClient espClient;
PubSubClient client(espClient);
#define MSG_BUFFER_SIZE 50
char msg[MSG_BUFFER_SIZE];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
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
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP8266Client-1234";
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(topic, "123.9");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  setup_wifi();
  client.setServer(mqtt_server, 1234);

  sensors.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  //Read temperature
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0);
    
  //Send
  snprintf(msg, MSG_BUFFER_SIZE, "%f", temperature);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish(topic, msg);
  
  delay(10000);
}
