#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust the I2C address if necessary

const int buttonPin = 7;     // Pin for the start button
const int waterSensorPin = A0; // Pin for the water sensor (analog pin)
const int relayPin = 8;      // Pin for the relay
int waterSensorValue = 0;
const int waterLimit = 600;  // Set the threshold for water detection

int seconds = 15;
bool timerStarted = false;

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  
  // Set up the button pin as input
  pinMode(buttonPin, INPUT_PULLUP);  // Use the internal pull-up resistor
  
  // Set up the relay pin as output
  pinMode(relayPin, OUTPUT);
  
  // Print the initial message to the LCD
  lcd.setCursor(0, 0);
  lcd.print("Press to Start");
}

void loop() {
  // Check if the button is pressed to start the timer
  if (digitalRead(buttonPin) == LOW && !timerStarted) {
    timerStarted = true;
    seconds = 15;  // Reset to 15 seconds
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Drink Now!");
    delay(500);  // Debounce delay
  }

  // Start the countdown and monitor drinking
  if (timerStarted) {
    // Check the water sensor value
    waterSensorValue = analogRead(waterSensorPin);

    // Stop the timer if the sensor no longer detects liquid
    if (waterSensorValue < waterLimit) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Congratulations!");
      timerStarted = false;
      lcd.setCursor(0, 1);
      lcd.print("Press to Restart");
      digitalWrite(relayPin, LOW); // Ensure relay is off
      return;  // Exit the loop to stop further counting
    }

    // Create a string for the current time
    String currentTime = "Time: ";
    if (seconds < 10) {
      currentTime += "0";
    }
    currentTime += seconds;

    // Print the time to the LCD
    lcd.setCursor(0, 1);
    lcd.print(currentTime);

    // Wait for one second
    delay(1000);

    // Decrement the seconds
    seconds--;

    // Check if time is up
    if (seconds < 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("It's over!");
      timerStarted = false;  // Reset the timer
      lcd.setCursor(0, 1);
      lcd.print("Press to Restart");

      // Turn on the relay for 3 seconds
      digitalWrite(relayPin, HIGH);
      delay(3000);  // Wait for 3 seconds
      digitalWrite(relayPin, LOW);  // Turn off the relay
    }
  }
}

