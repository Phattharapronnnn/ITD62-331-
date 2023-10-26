#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

ESP8266WebServer server(80);

const int ledPin = D6;
DHT dht11(D4, DHT11);

void init_wifi(String ssid, String password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup(void) {
  Serial.begin(115200);
  init_wifi("Galaxy eiei", "1234567890");

  pinMode(ledPin, OUTPUT);

  dht11.begin();

  server.on("/", HTTP_GET, []() {
    float temperature = dht11.readTemperature();
    float humidity = dht11.readHumidity();

    String html = "<html><body><div style='text-align: center;'>";
    html += "<h2 style='color:#778899;'>ESP8266</h2>";
    html += "<p><h2>Web Server</h2>";
    html += "<p>Humidity : <span id='humidity'>" + String(humidity) + " %</span></p>";
    html += "<p>Temperature : <span id='temperature'>" + String(temperature) + " C</span></p>";
    html += "<p>LED Status: ";
    if (digitalRead(ledPin) == HIGH) {
      html += "<span style='color: #00FF00;'>On</span>";
    } else {
      html += "<span style='color: #FF0000;'>Off</span>";
    }
    html += "</p>";
    html += "<form action='/TurnOn' method='get' style='display: inline; margin-right:25px;'><input type='submit' value='Turn LED On' onclick='this.style.backgroundColor=\"#00FF00\"'></form>&nbsp;";
    html += "<form action='/TurnOff' method='get' style='display: inline;'><input type='submit' value='Turn LED Off' onclick='this.style.backgroundColor=\"#FF0000\"'></form>";
    html += "</div></body></html>";

    html += "<script>";
    html += "function updateData() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open('GET', '/data', true);";
    html += "  xhr.onreadystatechange = function() {";
    html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
    html += "      var data = JSON.parse(xhr.responseText);";
    html += "      document.getElementById('temperature').textContent = data.temperature +'°C ';";  ///ลองเพิ่มองศาแต่ยังไม่ได้รัน
    html += "      document.getElementById('humidity').textContent = data.humidity + %' %';";
    html += "    }";
    html += "  };";
    html += "  xhr.send();";
    html += "}";
    html += "setInterval(updateData,1000);";
    html += "updateData();";
    html += "</script>";

    server.send(200, "text/html", html);
  });

  server.on("/TurnOn", HTTP_GET, []() {
    digitalWrite(ledPin, HIGH);
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/TurnOff", HTTP_GET, []() {
    digitalWrite(ledPin, LOW);
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/data", HTTP_GET, []() {
    float temperature = dht11.readTemperature();
    float humidity = dht11.readHumidity();

    String data = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";
    server.send(200, "application/json", data);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}


