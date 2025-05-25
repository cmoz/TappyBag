#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Adjust these if your OLED is a different size
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// If your OLED has no RESET pin, set to -1
#define OLED_RESET    -1

// Create display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define your I2C pins (adjust if needed for your board)
#define I2C_SDA 5 //5
#define I2C_SCL 4 //4

int myPin = 1; // GPIO2 (pin 8 on DevKitM-1)
bool touching = false;
unsigned long touchStart = 0;

String message = "Hello! ESP32-C3 OLED";
void displayMessage(String message);

void setup() {
  Serial.begin(115200);
  delay(1000);
  // Initialize I2C with custom pins for ESP32-C3
  Wire.begin(I2C_SDA, I2C_SCL);
  //Wire.begin(); // 

  // Initialize the display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // 0x3C check your I2C address
    Serial.println(F("SSD1306 allocation failed"));
    while (1); // Don't proceed, loop forever
  }
  Serial.println(F("SSD1306 allocation successful"));
  //display.display(); // Show initial splash screen
  delay(2000); // Pause for 2 seconds
  pinMode(myPin, INPUT_PULLDOWN); // Prevents random fluctuations
  // Set up the display
  Serial.println("OLED Init success");
  display.clearDisplay();
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Hello!");
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.println("ESP32-C3 OLED");
  display.display(); // Actually display all of the above
}

void loop() {
  // Read the analog value from the touch pin
  // The value will be between 0 and 4095  
  int sensorValue = analogRead(myPin);
  
  // Debug first!
  Serial.print(sensorValue);
  Serial.print(" - ");
  
  if (sensorValue > 1800) { // Adjust after seeing raw values 1500
    if (!touching) touchStart = millis();
    touching = true;
    
    if (millis() - touchStart < 500) {
      Serial.println("Tap");
      displayMessage("Tap\n...or keep holding");
    } else {
      Serial.println("Hold");
      displayMessage("Holding");
      // If holding for more than 2 seconds, turn on the light
      if (millis() - touchStart > 3000) {
        Serial.println("still holding!");
        displayMessage("...Still holding!");
        // Here you can add code to turn on an LED or perform another action
      } 
      if (millis() - touchStart > 8000) {
        displayMessage("Phew!\n\nThat was a long hold!");
      }
    }
  } else {
    touching = false;
    Serial.println("Tap for light");
    displayMessage("Tap for light");
  }
  delay(100);
}
  

  void displayMessage(String message) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("   *Tappy Rainbow*");

    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 18);
    display.println(message);
    display.display();
    delay(500);
  } 

