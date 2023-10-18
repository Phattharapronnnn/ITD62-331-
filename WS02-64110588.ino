#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h> 

float temperature = 0;
float humidity = 0;

DHT dht14(D4, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  dht14.begin();
  lcd.begin();
  lcd.clear(); 
}

void loop() {
  humidity = dht14.readHumidity();
  temperature = dht14.readTemperature();

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C  Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  String tempText = "Temp: " + String(temperature) + " C";
  String humidityText = "Hum: " + String(humidity) + " %";

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(tempText);

  lcd.setCursor(0, 1);
  lcd.print(humidityText);

  delay(500);
}
