# Smart Beverage Machine

A smart beverage dispensing system that allows customers to place orders via a keypad interface and receive their drinks through an automated dispensing mechanism. The system uses Arduino microcontrollers and ESP8266 WiFi modules for communication between the ordering station and the dispensing station.

## Features

- **Multiple Drink Options**: Choose from three different beverages:
  - Cream Soda (Option A)
  - Fanta (Option B)
  - Portello (Option C)

- **Size Selection**: Select between two cup sizes:
  - Half Cup (Option 1)
  - Full Cup (Option 2)

- **Unique Order Number System**: Each order generates a unique identifier combining drink type, size, and timestamp

- **Two Communication Methods**:
  - WiFi communication using ESP8266 modules
  - Serial communication using Arduino Uno boards

- **Cup Detection**: IR sensor detects cup presence before dispensing

- **LCD Display**: Real-time feedback for customers on both ordering and dispensing stations

## Hardware Requirements

### Sender (Ordering Station)
- 1x Arduino Uno OR 1x NodeMCU ESP8266
- 1x I2C LCD Display (20x4 for Uno, 16x2 for ESP)
- 1x 4x4 Matrix Keypad
- Jumper wires
- Power supply (5V for Arduino Uno, 3.3V/5V for NodeMCU)

### Receiver (Dispensing Station)
- 1x Arduino Uno OR 1x NodeMCU ESP8266
- 1x I2C LCD Display (16x2)
- 1x 4x4 Matrix Keypad
- 1x IR Sensor (for cup detection)
- 3x Relay Modules (for controlling beverage dispensers)
- Jumper wires
- Power supply

## Software Requirements

- [Arduino IDE](https://www.arduino.cc/en/software) (1.8.x or later)
- Required Libraries:
  - `Keypad` library
  - `Wire` library (built-in)
  - `LiquidCrystal_I2C` library
  - `ESP8266WiFi` library (for ESP8266 versions)

### Installing Required Libraries

1. Open Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries**
3. Search for and install:
   - "Keypad" by Mark Stanley and Alexander Brevig
   - "LiquidCrystal I2C" by Frank de Brabander

For ESP8266 support:
1. Go to **File → Preferences**
2. Add this URL to "Additional Board Manager URLs": 
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
3. Go to **Tools → Board → Boards Manager**
4. Search for "ESP8266" and install the package

## Pin Configuration

### Sender_Uno (Arduino Uno)
- **LCD (I2C)**: SDA (A4), SCL (A5)
- **Keypad**:
  - Row Pins: D2, D3, D4, D5
  - Column Pins: D6, D7, D8, D9

### Receiver_Uno (Arduino Uno)
- **LCD (I2C)**: SDA (A4), SCL (A5)
- **Keypad**:
  - Row Pins: D2, D3, D4, D5
  - Column Pins: D6, D7, D8, D9
- **IR Sensor**: D10
- **Relay Modules**:
  - Relay A (Cream Soda): D11
  - Relay B (Fanta): D12
  - Relay C (Portello): D13

### ESP8266 Versions
For ESP8266 modules, refer to the NodeMCU pinout diagram for GPIO mapping.

## Installation & Setup

### Option 1: Using Arduino Uno (Serial Communication)

#### Sender Setup
1. Open `Sender_Uno/Sender_Uno.ino` in Arduino IDE
2. Connect the Arduino Uno to your computer via USB
3. Select **Tools → Board → Arduino Uno**
4. Select the correct COM port under **Tools → Port**
5. Click **Upload**

#### Receiver Setup
1. Open `Receiver_Uno/Receiver_Uno.ino` in Arduino IDE
2. Connect the second Arduino Uno to your computer via USB
3. Select **Tools → Board → Arduino Uno**
4. Select the correct COM port under **Tools → Port**
5. Click **Upload**

#### Wiring
Connect the TX pin of Sender Uno to RX pin of Receiver Uno and connect GND of both boards together.

### Option 2: Using ESP8266 (WiFi Communication)

#### Sender Setup (Access Point)
1. Open `Sender_ESP/Sender_ESP.ino` in Arduino IDE
2. Configure WiFi credentials:
   ```cpp
   const char* ssid = "Flavor_Flow";
   const char* password = "flavorflow123";
   ```
3. Connect the NodeMCU ESP8266 to your computer via USB
4. Select **Tools → Board → NodeMCU 1.0 (ESP-12E Module)**
5. Select the correct COM port under **Tools → Port**
6. Click **Upload**

#### Receiver Setup (WiFi Client)
1. Open `Receiver_ESP/Receiver_ESP.ino` in Arduino IDE
2. Ensure WiFi credentials match the sender:
   ```cpp
   const char* ssid = "Flavor_Flow";
   const char* password = "flavorflow123";
   const char* serverAddress = "192.168.4.1";
   ```
3. Connect the second NodeMCU ESP8266 to your computer via USB
4. Select **Tools → Board → NodeMCU 1.0 (ESP-12E Module)**
5. Select the correct COM port under **Tools → Port**
6. Click **Upload**

## Usage

### Ordering Process (Sender Station)

1. **Select Drink Type**: The LCD displays the drink menu
   - Press 'A' for Cream Soda
   - Press 'B' for Fanta
   - Press 'C' for Portello

2. **Select Size**: After selecting a drink, choose the size
   - Press '1' for Half Cup
   - Press '2' for Full Cup

3. **Receive Order Number**: A unique order number will be displayed on the LCD
   - Format: `[DrinkType][Size][Timestamp]`
   - Example: `A1123456789` (Cream Soda, Half Cup, timestamp: 123456789)

4. **Proceed to Dispensing Station**: Remember or note down your order number

### Dispensing Process (Receiver Station)

1. **Enter Order Number**: Using the keypad, enter your order number
   - Use '*' key for backspace/delete
   - Use '#' key to submit/confirm

2. **Place Cup**: If prompted, place your cup under the dispenser
   - IR sensor will detect the cup presence

3. **Receive Beverage**: The system will automatically dispense your selected drink
   - Half Cup: 3 seconds dispensing time
   - Full Cup: 5 seconds dispensing time

4. **Order Complete**: Remove your cup and enjoy your beverage!

## System Architecture

```
[Sender Station]                    [Receiver Station]
Customer Input                      Order Validation
    ↓                                      ↓
LCD Display Menu                    LCD Display Status
    ↓                                      ↓
Keypad Selection                    Keypad Entry
    ↓                                      ↓
Order Number Generation             Order Number Verification
    ↓                                      ↓
WiFi/Serial Transmission    →       Cup Detection (IR)
                                           ↓
                                    Relay Control
                                           ↓
                                    Beverage Dispensing
```

## Troubleshooting

### LCD Not Displaying
- Check I2C address (default: 0x27). Use an I2C scanner sketch to find the correct address
- Verify I2C connections (SDA, SCL)
- Check if LCD backlight is enabled in code

### WiFi Connection Issues (ESP8266)
- Verify SSID and password are correct
- Ensure both ESP8266 modules are within range
- Check if Access Point is created (Sender ESP should create "Flavor_Flow" network)
- Verify IP address (default AP IP: 192.168.4.1)

### Keypad Not Responding
- Check all row and column pin connections
- Verify pin numbers in the code match your wiring
- Test keypad with serial output to debug

### Relays Not Activating
- Check relay pin connections
- Ensure relay module has adequate power supply
- Verify relay logic (some relays are active LOW, others active HIGH)

### Cup Not Detected
- Check IR sensor connections and power
- Adjust IR sensor sensitivity (if available)
- Test sensor with Serial.println() to verify readings

## Customization

### Changing Drink Names
Edit the drink names in `Sender_Uno.ino` or `Sender_ESP.ino`:
```cpp
lcd1.print("A. Cream Soda");
lcd1.print("B. Fanta");
lcd1.print("C. Portello");
```

### Adjusting Dispensing Time
Modify the delay values in `Receiver_Uno.ino`:
```cpp
// Half Cup
digitalWrite(relayPinA, HIGH);
delay(3000);  // Change this value (milliseconds)
digitalWrite(relayPinA, LOW);

// Full Cup
digitalWrite(relayPinA, HIGH);
delay(5000);  // Change this value (milliseconds)
digitalWrite(relayPinA, LOW);
```

### Changing WiFi Credentials
Update in both `Sender_ESP.ino` and `Receiver_ESP.ino`:
```cpp
const char* ssid = "YourNetworkName";
const char* password = "YourPassword";
```

## License

This project is open source and available for educational and personal use.

## Contributing

Contributions are welcome! Feel free to:
- Report bugs
- Suggest new features
- Submit pull requests

## Authors

- Janith Jay

## Acknowledgments

- Thanks to the Arduino community for the excellent libraries
- Inspired by automated vending machine systems
