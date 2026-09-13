#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ---------------- DS18B20 ----------------
#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// LCD: RS, EN, D4, D5, D6, D7
LiquidCrystal lcd(23, 22, 21, 19, 18, 17);

const int heartPin = 35;

int beats = 0;
int bpm = 0;

unsigned long startTime = 0;

bool lastState = LOW;

void setup() {

  Serial.begin(115200);

  pinMode(heartPin, INPUT);

  sensors.begin();

  lcd.begin(16, 2);

  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  // Connect WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  lcd.clear();
  lcd.print("Health Monitor");

  startTime = millis();
}

void loop() {

  bool currentState = digitalRead(heartPin);

  // Count only rising edge
  if (currentState == HIGH && lastState == LOW) {
    beats++;
  }

  lastState = currentState;

  // Every 5 seconds
  if (millis() - startTime >= 5000) {

    bpm = beats * 12;

    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);

    Serial.print("Temperature: ");
    Serial.print(tempC, 1);
    Serial.print(" C   ");

    Serial.print("Heart Rate: ");
    Serial.print(bpm);
    Serial.println(" BPM");

    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(tempC, 1);
    lcd.print((char)223);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("HR:");
    lcd.print(bpm);
    lcd.print(" BPM");

    if (WiFi.status() == WL_CONNECTED) {

      WiFiClientSecure client;
      client.setInsecure();

      HTTPClient http;

      String url =
      "https://iot-projects.eweb.org.in/stella/temp/update.php?temp=" +
      String(tempC, 1) +
      "&bpm=" +
      String(bpm);

      Serial.println(url);

      http.begin(client, url);

      int httpCode = http.GET();

      Serial.print("HTTP Response Code: ");
      Serial.println(httpCode);

      if (httpCode > 0) {
        Serial.println(http.getString());
      }

      http.end();
    }

    beats = 0;
    startTime = millis();
  }

  delay(10);
}