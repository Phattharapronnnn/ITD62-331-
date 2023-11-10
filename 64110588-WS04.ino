#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"

const char* ssid = "Galaxy eiei";
const char* password = "1234567890";
unsigned long channelID = 2339907;
const char* writeKey = "CCXRIN509YA6XKLM";

unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

#define DHTPIN D4
#define DHTTYPE DHT11

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  connectToWiFi();
}

void loop() {
  if (shouldUpdateChannel()) {
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Attempting to connect");
      connectToWiFi();
    }

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    printSensorData(humidity, temperature);

    updateThingSpeak(channelID, temperature, humidity);

    lastTime = millis();
  }
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    delay(5000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi.");
}

bool shouldUpdateChannel() {
  return (millis() - lastTime) > timerDelay;
}

void printSensorData(float humidity, float temperature) {
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.println("°C ");
}

void updateThingSpeak(unsigned long channel, float temperature, float humidity) {
  ThingSpeak.setField(1, temperature); 
  ThingSpeak.setField(2, humidity);   
  
  int updateStatus = ThingSpeak.writeFields(channel, writeKey);
  if (updateStatus == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(updateStatus));
  }
}
