#include "DHT.h"
#include <LiquidCrystal.h>

// DHT11 Sensor
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// L293D Motor Driver Pins
#define ENABLE 5
#define DIRA 3
#define DIRB 4

// LED Pins
#define LED_GREEN 6   // Fan OFF indicator
#define LED_RED   13  // Fan ON indicator

// LCD (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Temperature threshold to trigger the fan (Fahrenheit)
const float TEMP_THRESHOLD = 82.0;

void setup() {
  Serial.begin(9600);
  dht.begin();

  // Motor pins
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  digitalWrite(ENABLE, LOW);

  // LED pins
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Start with green LED on (fan off)
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED, LOW);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("  Temp Control  ");
  lcd.setCursor(0, 1);
  lcd.print("  Starting...   ");
  delay(2000);
  lcd.clear();
}

void loop() {
  delay(2000);

  float f = dht.readTemperature(true);

  // Handle sensor read error
  if (isnan(f)) {
    Serial.println("Failed to read from DHT sensor! Check your wires.");

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!   ");
    lcd.setCursor(0, 1);
    lcd.print("Check wires.    ");

    // Flash both LEDs to signal error
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_RED, HIGH);
    delay(500);
    digitalWrite(LED_RED, LOW);
    delay(500);

    return;
  }

  // Print to Serial Monitor
  Serial.print("Current Temp: ");
  Serial.print(f);
  Serial.print(" F -> ");

  // Update LCD top row with temperature
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(f, 1);
  lcd.print(" F      ");

  // Fan logic
  if (f >= TEMP_THRESHOLD) {
    Serial.println("FAN IS ON (Too hot!)");

    // Turn fan on full speed
    analogWrite(ENABLE, 255);
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);

    // Red LED on, green off
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);

    // Update LCD bottom row
    lcd.setCursor(0, 1);
    lcd.print("Fan: ON  (Hot!) ");

  } else {
    Serial.println("FAN IS OFF (Temp safe)");

    // Kill power to motor
    digitalWrite(ENABLE, LOW);

    // Green LED on, red off
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);

    // Update LCD bottom row
    lcd.setCursor(0, 1);
    lcd.print("Fan: OFF (Safe) ");
  }
}
