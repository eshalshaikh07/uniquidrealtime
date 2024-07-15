#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FirebaseESP32.h>

// Replace with your network credentials
const char* ssid = "Your Wifi name";
const char* password = "Your Wifi psw";

// Firebase Project details
const char* FIREBASE_HOST = "Firebase project id";
const char* FIREBASE_AUTH = "Firebase API key";

FirebaseData firebaseData;
FirebaseConfig config;
FirebaseAuth auth;

// OLED display width and height, in pixels
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// I2C address of the OLED display
#define OLED_I2C_ADDRESS 0x3C

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  // Assign the Firebase credentials
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Initialize the OLED display with the I2C address
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  
  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Enter ID in the form 123B1EXXX:");
  display.display();

  Serial.println("Enter ID in the form 123B1EXXX:");
}

bool isValidIDFormat(const String& id) {
  if (id.length() != 9) return false;
  if (id.substring(0, 5) != "123B1") return false;
  if (id.charAt(5) != 'E') return false;
  for (int i = 6; i < 9; i++) {
    if (!isDigit(id.charAt(i))) return false;
  }
  return true;
}

String generateUniqueID() {
  return String(random(1000, 10000)); // Generate a random 4-digit number
}

void displayOnOLED(const String& message) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (isValidIDFormat(input)) {
      String uniqueID;

      if (Firebase.ready()) {
        if (Firebase.getString(firebaseData, "/UniqueIDs/" + input)) {
          uniqueID = firebaseData.stringData();
          Serial.print("Existing Unique 4-digit ID: ");
          Serial.println(uniqueID);
          displayOnOLED("Existing Unique 4-digit ID:\n" + uniqueID);
        } else {
          uniqueID = generateUniqueID();
          if (Firebase.setString(firebaseData, "/UniqueIDs/" + input, uniqueID)) {
            Serial.print("Generated New Unique 4-digit ID: ");
            Serial.println(uniqueID);
            displayOnOLED("Generated New Unique 4-digit ID:\n" + uniqueID);
          } else {
            Serial.println("Failed to send Unique 4-digit ID to Firebase");
            Serial.println(firebaseData.errorReason());
            displayOnOLED("Failed to send Unique 4-digit ID to Firebase");
            return;
          }
        }
      } else {
        Serial.println("Firebase not ready.");
        displayOnOLED("Firebase not ready.");
      }
    } else {
      Serial.println("Invalid ID format. Please enter an ID in the form 123B1EXXX where XXX are any integers:");
      displayOnOLED("Invalid ID format. Please enter an ID in the form 123B1EXXX where XXX are any integers:");
    }
  }
}
