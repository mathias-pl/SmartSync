// ROBOHACKS 2024
// Authors: Celina Belleville, Mathias Pacheco Lemina, Zeeshan Rahman
// 17/02/2024 @ 18/02/2024
// We love robotics <3
#include <dht_nonblocking.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include "pitches.h"
#define DHT_SENSOR_TYPE DHT_TYPE_11

//Analog pins
int waterSensorPower = A0;
int waterSensorPin = A1;
int DHT_SENSOR_PIN = A2;
int displayPin = A3;
int powerPin = A4;
int airQualityWarningSensorPin = A5;

//Digital pins
int ledBluePin = 2;
int ledGreenPin = 3;
int ledRedPin = 4;
int ledYellowPin = 5;
int buzzerPin = 6;

//Display pins
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Special commands
DHT_nonblocking dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// Global variables
bool onOff = true;
bool airQualityWarning = false;
bool warningTemperature = false;
bool THwarning = false;
float temperature;
float humidity;
int waterSensor;
int displayState = 0;

//Custom char
byte celsius[8] = {
  B10000,
  B00011,
  B01100,
  B11000,
  B10000,
  B11000,
  B01100,
  B00011
};

void setup() {
    // Status pins
    pinMode(ledYellowPin, OUTPUT);
    pinMode(powerPin, INPUT_PULLUP);
    pinMode(displayPin, INPUT_PULLUP);

    // Alert pins
    pinMode(ledRedPin, OUTPUT);
    pinMode(ledGreenPin, OUTPUT);
    pinMode(ledBluePin, OUTPUT);

    // Sensor pins
    digitalWrite(waterSensorPower, LOW);
    pinMode(waterSensorPower, OUTPUT);
    pinMode(airQualityWarningSensorPin, INPUT);

    // Motor pin
    pinMode(buzzerPin, OUTPUT);

    // Display setup
    lcd.begin(16, 2);
    welcomeScreen();

    Serial.begin(9600);
}

/*
 * Poll for a measurement, keeping the state machine alive.  Returns
 * true if a measurement is available.
 */
static bool measure_environment(float *temperature, float *humidity) {
    static unsigned long measurement_timestamp = millis();

    if (dht_sensor.measure(temperature, humidity) == true) {
        measurement_timestamp = millis();
        return (true);
    }

    return (false);
}

int readWaterSensor() {
    int val = 0;
    digitalWrite(waterSensorPower, HIGH); // Turn the sensor ON
    delay(10);                       // wait 10 milliseconds
    val = analogRead(waterSensorPin);     // Read the analog value form sensor
    digitalWrite(waterSensorPower, LOW);  // Turn the sensor OFF
    return val;                      // send current reading
}

void loop() {
    // Button is not really a switch, we need to manually switch the value of a boolean
    if (digitalRead(powerPin) == LOW) {
        onOff = !onOff;
        delay(500); // If pressed more than 250ms, it's gonna turn back on - Hardware limitation
    }

    // Display button
    if (digitalRead(displayPin) == LOW) {
      displayState = (displayState + 1) % 4;
      delay(500);
    }

    // If the system is on, we want to retrieve data
    if (onOff) {
        // Set the yellow light on (active)
        digitalWrite(ledYellowPin, HIGH);

        //Retrieve data as soon as DHT11 have data to send to the system (approx. 4s)
        if (measure_environment(&temperature, &humidity) == true) {
            //---DATA---

            // Here so we don't update the variables unnecessarily
            waterSensor = readWaterSensor();
            airQualityWarning = digitalRead(airQualityWarningSensorPin);

            // Displaying the retrieved data from the house - for data analysis only
            Serial.print(temperature, 0);
            Serial.print("\t");
            Serial.print(humidity, 0);
            Serial.print("\t");
            Serial.print(waterSensor);
            Serial.print("\t");
            Serial.println(airQualityWarning);

            //TODO - CSV persistence of retrieved data (OPTIONAL)
            //https://stackoverflow.com/questions/25201131/writing-csv-files-from-c#:~:text=If%20you%20wirte%20to%20a,an%20empty%20row%20

            //---LED ALERTS---
            THwarning = (humidity < 15) || (humidity > 40) || (temperature < 10) || (temperature > 28);// TODO: Get more realistic values

            // Any problem with T&H
            if (THwarning) digitalWrite(ledRedPin, HIGH);
            else digitalWrite(ledRedPin, LOW);
            
            // System cooldown
            if (temperature > 23) warningTemperature = true;
            else warningTemperature = false;

            // Current airQualityWarning
            if (airQualityWarning) digitalWrite(ledGreenPin, HIGH);
            else digitalWrite(ledGreenPin, LOW);

            // Current water level threshold: 300 -> approx. half of the sensor
            if (waterSensor > 300) digitalWrite(ledBluePin, HIGH);
            else digitalWrite(ledBluePin, LOW);

            //---DISPLAY---
            lcd.clear(); //Default
        }

        //Display function
        if (displayState == 0) {
          lcd.setCursor(0, 0);
          lcd.print("Temperature:");
          lcd.print(temperature, 0);
          lcd.write(byte(0));
          lcd.print("       ");

          lcd.setCursor(0, 1);
          lcd.print("Humidity:");
          lcd.print(humidity, 0);
          lcd.print("%");
          lcd.print("       ");
        } else if (displayState == 1) {
          lcd.setCursor(0, 0);
          lcd.print("Humidity:");
          lcd.print(humidity, 0);
          lcd.print("%");
          lcd.print("       ");

          lcd.setCursor(0, 1);
          lcd.print("WaterTest:");
          lcd.print(waterSensor);
          lcd.print("PPM     ");
        } else if (displayState == 2) {
          lcd.setCursor(0, 0);
          lcd.print("WaterTest:");
          lcd.print(waterSensor);
          lcd.print("PPM      ");

          lcd.setCursor(0, 1);
          lcd.print("Air problem:");
          
          if (airQualityWarning) lcd.print("YES");
          else lcd.print("NO");
          lcd.print("       ");
        } else {
          lcd.setCursor(0, 0);
          lcd.print("Air problem:");
          
          if (airQualityWarning) lcd.print("YES");
          else lcd.print("NO");
          lcd.print("       ");

          lcd.setCursor(0, 1);
          lcd.print("Temperature:");
          lcd.print(temperature, 0);
          lcd.write(byte(0));
          lcd.print("       ");
        }

        //---BUZZER---
        if (airQualityWarning || THwarning || waterSensor > 300) digitalWrite(buzzerPin, HIGH);
        else digitalWrite(buzzerPin, LOW);
    }
    else
    {
        digitalWrite(ledYellowPin, LOW);
        digitalWrite(ledRedPin, LOW);
        digitalWrite(ledGreenPin, LOW);
        digitalWrite(ledBluePin, LOW);
        digitalWrite(buzzerPin, LOW);

        lcd.setCursor(0, 0);
        lcd.print("--- SmartSync --");
        lcd.setCursor(0, 1);
        lcd.print("--- Disabled ---");
    }
}

void welcomeScreen() {
    lcd.clear();
    lcd.print("-- Welcome to --");
    lcd.setCursor(0, 1);
    lcd.print("-- SmartSync ---");
    lcd.createChar(0, celsius);

    digitalWrite(buzzerPin, HIGH);
    delay(250);
    digitalWrite(buzzerPin, LOW);
    delay(750);

    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    delay(500);

    digitalWrite(buzzerPin, HIGH);
    delay(750);
    digitalWrite(buzzerPin, LOW);
    delay(250);

    lcd.clear();
    digitalWrite(buzzerPin, HIGH);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print(".               ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, LOW);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print("..              ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, HIGH);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print("...             ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, LOW);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print("....            ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, HIGH);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print(".....           ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, LOW);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print("......          ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, HIGH);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print(".......         ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, LOW);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print(".........       ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, HIGH);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print("..........      ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, LOW);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print("...........     ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, HIGH);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print("............    ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, LOW);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print(".............   ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, HIGH);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print("..............  ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, LOW);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print("............... ");
    delay(200);

    lcd.clear();
    digitalWrite(buzzerPin, HIGH);
    lcd.print("--- Starting ---");
    lcd.setCursor(0, 1);
    lcd.print("................");
    delay(200);
    digitalWrite(buzzerPin, LOW);
}