#include <Wire.h>
#include <LiquidCrystal.h>   // Library for 16x2 LCD display
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define LCD_RS 12
#define LCD_E 11
#define LCD_D4 10
#define LCD_D5 9
#define LCD_D6 8
#define LCD_D7 7

#define NUM_DHT_SENSORS 5
#define DHT_PIN_1 4
#define DHT_PIN_2 5
#define DHT_PIN_3 18
#define DHT_PIN_4 19
#define DHT_PIN_5 21

const int dhtPins[NUM_DHT_SENSORS] = {DHT_PIN_1, DHT_PIN_2, DHT_PIN_3, DHT_PIN_4, DHT_PIN_5};

#define DHT_TYPE DHT11

#define PELTIER_PIN_1 22
#define PELTIER_PIN_2 23
#define RELAY_HEATER_PIN 25

DHT dhtSensors[NUM_DHT_SENSORS] = {
  DHT(dhtPins[0], DHT_TYPE),
  DHT(dhtPins[1], DHT_TYPE),
  DHT(dhtPins[2], DHT_TYPE),
  DHT(dhtPins[3], DHT_TYPE),
  DHT(dhtPins[4], DHT_TYPE)
};

LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Wi-Fi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Server URL for monitoring
const char* serverUrl = "http://your-server-address/endpoint";

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Initialize the DHT sensors
  for (int i = 0; i < NUM_DHT_SENSORS; i++) {
    dhtSensors[i] = DHT(dhtPins[i], DHT_TYPE);
    dhtSensors[i].begin();
  }

  // Initialize control pins
  pinMode(PELTIER_PIN_1, OUTPUT);
  pinMode(PELTIER_PIN_2, OUTPUT);
  pinMode(RELAY_HEATER_PIN, OUTPUT);

  // Initialize LCD display
  lcd.begin(16, 2);
  lcd.print("Initializing...");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  // Read sensor values
  float temps[NUM_DHT_SENSORS];
  float humidities[NUM_DHT_SENSORS];

  for (int i = 0; i < NUM_DHT_SENSORS; i++) {
    temps[i] = dhtSensors[i].readTemperature();
    humidities[i] = dhtSensors[i].readHumidity();
  }

  // Display values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T1: ");
  lcd.print(temps[0]);
  lcd.print("C H1: ");
  lcd.print(humidities[0]);

  lcd.setCursor(0, 1);
  lcd.print("T2: ");
  lcd.print(temps[1]);
  lcd.print("C H2: ");
  lcd.print(humidities[1]);

  lcd.setCursor(0, 2);
  lcd.print("T3: ");
  lcd.print(temps[2]);
  lcd.print("C H3: ");
  lcd.print(humidities[2]);

  lcd.setCursor(0, 3);
  lcd.print("T4: ");
  lcd.print(temps[3]);
  lcd.print("C H4: ");
  lcd.print(humidities[3]);

  lcd.setCursor(0, 4);
  lcd.print("T5: ");
  lcd.print(temps[4]);
  lcd.print("C H5: ");
  lcd.print(humidities[4]);

  delay(5000); // Display data for 5 seconds

  // Control Peltier elements
  if (temps[0] > 30.0) {
    digitalWrite(PELTIER_PIN_1, HIGH);
  } else {
    digitalWrite(PELTIER_PIN_1, LOW);
  }

  if (temps[1] > 30.0) {
    digitalWrite(PELTIER_PIN_2, HIGH);
  } else {
    digitalWrite(PELTIER_PIN_2, LOW);
  }

  // Control heater relay
  if (humidities[0] < 40.0) {
    digitalWrite(RELAY_HEATER_PIN, HIGH);
  } else {
    digitalWrite(RELAY_HEATER_PIN, LOW);
  }

  // Send data to server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(serverUrl) + "?";
    for (int i = 0; i < NUM_DHT_SENSORS; i++) {
      url += "temp" + String(i + 1) + "=" + String(temps[i]) + "&humidity" + String(i + 1) + "=" + String(humidities[i]) + "&";
    }
    url.remove(url.length() - 1); // Remove trailing '&' character

    http.begin(client, url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }

  delay(10000); // Wait for 10 seconds before the next loop iteration
}
