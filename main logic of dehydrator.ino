#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 4          // Change this to the pin you're using for the DHT sensor
#define DHTTYPE DHT22     // Change to DHT11 if using a DHT11 sensor
DHT dht(DHTPIN, DHTTYPE);

#define HEATER_PIN 2      // Relay control pin

void setup() {
  Serial.begin(115200);

  // Initialize the OLED display
  if(!display.begin(SSD1306_I2C_ADDRESS, 0x3C)) { // Check the address (0x3C is common)
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);  // Pause for 2 seconds

  // Initialize DHT sensor
  dht.begin();
  
  // Initialize heater control pin
  pinMode(HEATER_PIN, OUTPUT);
  digitalWrite(HEATER_PIN, LOW); // Make sure heater is off initially
}

void loop() {
  // Read temperature and humidity
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Check if any reads failed and exit early
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Display temperature and humidity on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.print(F("Temp: "));
  display.print(temperature);
  display.print(F(" C"));
  display.setCursor(0,10);
  display.print(F("Humidity: "));
  display.print(humidity);
  display.print(F(" %"));
  display.display();

  // Control heater based on temperature
  if (temperature < 50) { // Set your desired temperature threshold
    digitalWrite(HEATER_PIN, HIGH); // Turn heater on
  } else {
    digitalWrite(HEATER_PIN, LOW);  // Turn heater off
  }

  delay(2000); // Delay between readings
}
