#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>



const int lcd1Address = 0x27; // Define the I2C addresses for the two displays
char selectedDrinkType = ' '; // Variable to store selected drink type (A, B, or C)
int selectedSize = 0; // Variable to store selected size (1 or 2)


// Create instances of the LiquidCrystal_I2C library for each display
LiquidCrystal_I2C lcd1(lcd1Address, 20, 4);

byte rowPins[4] = {2, 3, 4, 5};
byte colPins[4] = {6, 7, 8, 9};

char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

Keypad test1 = Keypad(makeKeymap(keys), rowPins, colPins, 4, 4);

void menu1() {
  lcd1.clear();
  lcd1.setCursor(0,1);
  lcd1.print("-------MENU 1-------");
  delay(1500);
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.print("----Select Drink----");
  lcd1.setCursor(0,1);
  lcd1.print("A. Coffee");
  lcd1.setCursor(0,2);
  lcd1.print("B. Milk");
  lcd1.setCursor(0,3);
  lcd1.print("C. Plain Tea");
}

void menu2() {
  lcd1.clear();
  lcd1.setCursor(0,2);
  lcd1.print("-------MENU 2-------");
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.print("-Select Drink Size-");
  lcd1.setCursor(0,1);
  lcd1.print("1.Half Cup");
  lcd1.setCursor(0,2);
  lcd1.print("2.Full Cup");
}

enum State {
  DISPLAY_MENU1,
  WAIT_FOR_INPUT_MENU1,
  DISPLAY_MENU2,
  WAIT_FOR_INPUT_MENU2
};

State state = DISPLAY_MENU1;
State prevState = DISPLAY_MENU1;

String name ;
String size ;

void setup() {
  
  Wire.begin(); // Initialize the I2C communication
  Serial.begin(9600); // Initialize serial communication
 
  // Initialize the displays
  lcd1.begin(20, 4);

  randomSeed(analogRead(0));

  // Turn on the backlight for both displays
  lcd1.backlight();
  
}

void loop() {

  switch (state) {
    case DISPLAY_MENU1:
      menu1();
      prevState = DISPLAY_MENU1;
      state = WAIT_FOR_INPUT_MENU1;
      break;

    case WAIT_FOR_INPUT_MENU1:
      if (processInput('A', 'B', 'C')) {
        state = DISPLAY_MENU2;
      }
      break;

    case DISPLAY_MENU2:
      menu2();
      prevState = DISPLAY_MENU2;
      state = WAIT_FOR_INPUT_MENU2;
      break;

    case WAIT_FOR_INPUT_MENU2:
      if (processInput('1', '2','3')) {
        state = DISPLAY_MENU1;
      }
      break;
  }

}

bool processInput(char option1, char option2, char option3) {
  char key = test1.getKey();

  if (key != NO_KEY) {

    if (state == WAIT_FOR_INPUT_MENU1) {
      if (key == option1) {
        lcd1.clear();
        lcd1.setCursor(0,1);
        lcd1.print("You Choose Coffee");
        selectedDrinkType = 'A'; // Assume user selected drink type A
        delay(2000);
        name = "Coffee";
      } else if (key == option2) {
        lcd1.clear();
        lcd1.setCursor(0,1);
        lcd1.print("You Choose Milk");
        selectedDrinkType = 'B'; // Assume user selected drink type B
        delay(2000);
        name = "Milk";
      } else if (key == option3) {
        lcd1.clear();
        lcd1.setCursor(0,1);
        lcd1.print("You Choose Plain Tea ");
        selectedDrinkType = 'C'; // Assume user selected drink type C
        delay(2000);
        name = "Plain Tea";
      } else {
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("--Invalid Choice !--");
        lcd1.setCursor(0,2);
        lcd1.print("Select Option Again");
        delay(2000);
        state = prevState;
        return false;
      }
    } else if (state == WAIT_FOR_INPUT_MENU2) {
      if (key == '1') {
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("-----Drink Size-----");
        lcd1.setCursor(0,2);
        lcd1.print("      Half Cup      ");
        size = "Half Cup";
        selectedSize = 1; // Assume user selected size 1
        delay(2000);
      } else if (key == '2') {
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("-----Drink Size-----");
        lcd1.setCursor(0,2);
        lcd1.print("      Full Cup      ");
        size = "Full Cup";
        selectedSize = 2; // Assume user selected size 2
        delay(2000);
      } else {
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("--Invalid Choice !--");
        lcd1.setCursor(0,2);
        lcd1.print("Select Option Again");
        delay(2000);
        state = prevState;
        return false;
      }
    }

    
    if(state == WAIT_FOR_INPUT_MENU2){
      lcd1.clear();
      lcd1.setCursor(0,0);
      lcd1.print("----Order Placed----");
      lcd1.setCursor(0,2);
      lcd1.print(size);
      lcd1.print(" Of ");
      lcd1.setCursor(0,3);
      lcd1.print(name);
      delay(5000);
      
      // Generate a unique order number based on the selected drink type and size
      unsigned long timestamp = millis(); // Get current timestamp in milliseconds
      
      // Create a unique order number by combining timestamp, drink type, and size
      String uniqueOrderNum = (selectedDrinkType) + String(selectedSize) + String(timestamp);

      lcd1.clear();
      lcd1.setCursor(0,0);
      lcd1.print("Your Order Number");
      lcd1.setCursor(0,2);
      lcd1.print(uniqueOrderNum);
      Serial.print(uniqueOrderNum);

      delay(10000); // Delay for demonstration purposes

      lcd1.setCursor(0,3);
      lcd1.print("Next Customer Plz!");
      delay(5000);
    }
    state = (state == WAIT_FOR_INPUT_MENU2) ? DISPLAY_MENU2 : DISPLAY_MENU1;

    return true;
  }

  return false;
}