# Firebase Unique ID Generator with OLED Display

This Arduino sketch generates and stores unique 4-digit IDs in Firebase based on user input through the Serial Monitor. The generated or retrieved IDs are displayed on an SSD1306 OLED display connected to the Arduino.

## Components Used

- **Arduino Board (e.g., Arduino Uno, ESP32)**
- **SSD1306 OLED Display**
- **FirebaseESP32 Library**
- **WiFi Module**

## Circuit Diagram

### Pin Connections

Connect your components as follows:

- **SSD1306 OLED Display**:
  - **VCC** to Arduino 5V
  - **GND** to Arduino GND
  - **SCL** to Arduino SCL (A5 on Arduino Uno, GPIO21 on ESP32)
  - **SDA** to Arduino SDA (A4 on Arduino Uno, GPIO22 on ESP32)

## Installation

1. **Install Required Libraries**:
   - Install the `Adafruit_GFX`, `Adafruit_SSD1306`, `Wire`, `FirebaseESP32`, and `WiFi` libraries through the Arduino Library Manager.

2. **Configure Firebase Credentials**:
   - Replace placeholders in the sketch (`ssid`, `password`, `FIREBASE_HOST`, `FIREBASE_AUTH`) with your WiFi network credentials and Firebase project details.

3. **Upload Code**:
   - Open the provided Arduino sketch (`unique_id_generator.ino`) in your Arduino IDE.
   - Upload the sketch to your Arduino board.

## Usage

1. **Setup**:
   - Ensure your SSD1306 OLED display is correctly connected to your Arduino board as per the circuit diagram.
   - Adjust any pin configurations or settings in the sketch if necessary.

2. **Operation**:
   - Open the Serial Monitor (set baud rate to 115200).
   - Enter an ID in the format `123B1EXXX` where `XXX` are any integers and press Enter.
   - The program validates the ID format, generates a new unique 4-digit ID if necessary, stores it in Firebase, and displays the ID on the OLED display.

## Example Code

```cpp
// Your Arduino code goes here
