#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
// Define the Keymap
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[4] = {2, 3, 4, 5};
byte colPins[4] = {6, 7, 8, 9};

// Initialize an instance of class Keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int irSensorPin = 10; // IR sensor pin
int irSensorValue = 0;

// Define relay pins
const int relayPinA = 11; // Relay connected to pin 11
const int relayPinB = 12; // Relay connected to pin 12
const int relayPinC = 13; // Relay connected to pin 13

String receivedOrderNumber = ""; // Variable to store the received order number
String enteredOrderNumber = "";  // Variable to store the entered order number

void setup() {
  // Initialize the LCD
  lcd.init();
  // Turn on the backlight (optional)
  lcd.backlight();

  // Display initial message
  lcd.setCursor(0, 0);
  lcd.print("Enter Order No:");

  Serial.begin(9600);

  pinMode(irSensorPin, INPUT);

  // Set relay pins as outputs
  pinMode(relayPinA, OUTPUT);
  pinMode(relayPinB, OUTPUT);
  pinMode(relayPinC, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    receivedOrderNumber = Serial.readStringUntil('\n'); // Read the received string until newline character
    receivedOrderNumber.trim(); // Remove leading and trailing whitespaces
    Serial.println(receivedOrderNumber);
  }

  static bool orderEntered = false;

  if (!orderEntered) {
    char customKey = customKeypad.getKey();

    if (customKey) {
      if (customKey == '*') { // '*' key for backspace
        if (enteredOrderNumber.length() > 0) {
          enteredOrderNumber.remove(enteredOrderNumber.length() - 1);
          lcd.setCursor(0, 1);
          lcd.print("                "); // Clear the line
          lcd.setCursor(0, 1);
          lcd.print(enteredOrderNumber);
        }
      } else if (customKey != '#') { // '#' key as Enter
        enteredOrderNumber += customKey; // Concatenate keys to form entered order number
        lcd.setCursor(0, 1);
        lcd.print(enteredOrderNumber);
        delay(200); // Delay to debounce keypad
      } else {
        orderEntered = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Order No:");
        lcd.setCursor(0, 1);
        lcd.print(enteredOrderNumber);

        enteredOrderNumber.trim();

        // Compare entered order number with received order number
        if(enteredOrderNumber == receivedOrderNumber){

          irSensorValue = digitalRead(irSensorPin); // Read IR sensor status

          if (irSensorValue == LOW) {
          
            // Correct order number entered
            char startChar = enteredOrderNumber.charAt(0);

            if (startChar == 'A') {
              // Control relay connected to pin relayPinA
              digitalWrite(relayPinA, HIGH); // Turn on relay connected to relayPinA
            } else if (startChar == 'B') {
              // Control relay connected to pin relayPinB
              digitalWrite(relayPinB, HIGH); // Turn on relay connected to relayPinB
            } else if (startChar == 'C') {
              // Control relay connected to pin relayPinC
              digitalWrite(relayPinC, HIGH); // Turn on relay connected to relayPinC
            } else {
            lcd.clear();
            lcd.print("Wrong Order No!");
            delay(1500); // Display "Wrong Order!" for 2 seconds
            }

            delay(500); // Delay to hold the relay on for 0.5 seconds

            // Turn off all relays
            digitalWrite(relayPinA, LOW);
            digitalWrite(relayPinB, LOW);
            digitalWrite(relayPinC, LOW);

            receivedOrderNumber = "";

          } else {
          // Logic for cup detection and additional actions
          // Cup detection code
          lcd.clear();
          lcd.print("Please put a cup!");
          delay(2000); // Display "Please put a cup!" for 2 seconds

          // Wait for the cup to be placed (IR sensor detects obstacle)
          while (digitalRead(irSensorPin) == HIGH) {
            delay(100);
          }
          lcd.clear();
          lcd.print("Cup Added!");
          delay(1000); // Display "Cup Added!" for 1 second

          // Additional actions based on cup detection
          char startChar = enteredOrderNumber.charAt(0);

            if (startChar == 'A') {
              // Control relay connected to pin relayPinA
              digitalWrite(relayPinA, HIGH); // Turn on relay connected to relayPinA
            } else if (startChar == 'B') {
              // Control relay connected to pin relayPinB
              digitalWrite(relayPinB, HIGH); // Turn on relay connected to relayPinB
            } else if (startChar == 'C') {
              // Control relay connected to pin relayPinC
              digitalWrite(relayPinC, HIGH); // Turn on relay connected to relayPinC
            } else {
            lcd.clear();
            lcd.print("Wrong Order No!");
            delay(1500); // Display "Wrong Order!" for 2 seconds
            }

            delay(500); // Delay to hold the relay on for 0.5 seconds

            // Turn off all relays
            digitalWrite(relayPinA, LOW);
            digitalWrite(relayPinB, LOW);
            digitalWrite(relayPinC, LOW);

            receivedOrderNumber = "";
          }
          
        } else {
            lcd.clear();
            lcd.print("Wrong Order No!");
            delay(1500); // Display "Wrong Order!" for 2 seconds
        }
        
        // Reset variables for the next order entry
        enteredOrderNumber = "";
        orderEntered = false;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Enter Order No:");
      }
    }
  }
}
