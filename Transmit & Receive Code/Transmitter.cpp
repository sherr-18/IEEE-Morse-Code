/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/
#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

const int dotButtonPin = 27;       // Connect the dot button to digital pin 2
const int dashButtonPin = 26;     // Connect the dash button to digital pin 3
const int spaceButtonPin = 25;
const int LEDPin = 12;          
const int LED2Pin = 14;          
const int buzzerPin = 33;

int lcdplacement = 0;
int Row0;

bool dotButtonPressed = false;
bool dashButtonPressed = false;
bool spaceButtonPressed = false;

String lastInputType = "";
String morseCode = "";         // Variable to store the Morse code sequence
String Space = "";


// Morse code to letter mapping (using uppercase for comparison)
const char* morseAlphabet[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----","..---","...--","....-",".....","-....","--...","---..","----."};
const char* spacerule[] = {"~", "~~"};
unsigned long prevtime = millis();
long timeout = 2000;


// REPLACE WITH YOUR RECEIVER MAC Address
// uint8_t broadcastAddress[] = {0x08, 0xD1, 0xF9, 0x27, 0xB9, 0xEC};
uint8_t broadcastAddress[] = {0x40, 0x22, 0xD8, 0x08, 0x3A, 0xC0};

// uint8_t broadcastAddress2[] = {0xC8, 0x2E, 0x18, 0xD2, 0xEA, 0x3C};
// uint8_t broadcastAddress3[] = {0xA8, 0x42, 0xE3, 0x48, 0x24, 0x6C};
// uint8_t broadcastAddress4[] = {0xE4, 0x65, 0xB8, 0x74, 0x6B, 0x48};

//[DEFAULT] ESP32 Board MAC Address: 08:d1:f9:27:b9:ec
//[DEFAULT] ESP32 Board MAC Address: a8:42:e3:48:24:6c
//[DEFAULT] ESP32 Board MAC Address: c8:2e:18:d2:ea:3c
//[DEFAULT] ESP32 Board MAC Address: e4:65:b8:74:6b:48
//[DEFAULT] ESP32 Board MAC Address: a8:42:e3:48:24:6c

//[DEFAULT] ESP32 Board MAC Address: 08:d1:f9:27:b9:ec
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int a;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void undefined() {
  // Handle incorrect or incomplete code
  Serial.println("Unknown Morse Code");
  lcdplacement += 1;
  Serial.println(lcdplacement);

      myData.a = 40;
      // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    if (result == ESP_OK) {
    Serial.println("Sent with success");
    }
    else {
    Serial.println("Error sending the data");
    }

    for (int j = 0; j < 3; j++) { // Repeat 10 times (adjust as needed)
      Serial.println("buzzer");
    tone(buzzerPin, 4000); // Turn on the tone at 1000 Hz
    delay(200); // Wait for 100 milliseconds
    noTone(buzzerPin); // Turn off the tone
    delay(200); // Wait for 100 milliseconds before the next blink
    }
}

void interpretSpace(String Space){
  Serial.println(Space);
  for (int i = 0; i < 2; i++) {
    if (Space == spacerule[i]) {
      Serial.println(i);
      if (i < 1) {
        myData.a = 37;
        lcdplacement += 1;
        Serial.println(lcdplacement);

      }

      else if (i > 0 && i < 2) {
        myData.a = 38;
        lcdplacement -= 1;
        Serial.println(lcdplacement);
      }
    
    Serial.println(myData.a);
    digitalWrite(LED2Pin, HIGH);
    tone(buzzerPin, 500, 100);
    delay(100);
      // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    Serial.println("Dot Button Pressed");

    if (result == ESP_OK) {
    Serial.println("Sent with success");
    }
    else {
    Serial.println("Error sending the data");
    }
    return;
   }
  }
  undefined();

}

void interpretMorseCode(String morseCode) {
  for (int i = 0; i < 36; i++) {
    if (morseCode == morseAlphabet[i]) {
      Serial.println(i);
      myData.a = i;
      if (i < 26) {
        char Alphabet = char('A' + i);
        // lcd.print(Alphabet);

      } else if (i < 36) {
        // lcd.print(char('0' + i - 26)); // Print numbers
      }
      // lcd.display();
      Serial.println("LCD Updated");
      lcdplacement += 1;
      Serial.println(lcdplacement);
      digitalWrite(LED2Pin, HIGH);
      tone(buzzerPin, 500, 100);
      delay(100);
      
      // Send message via ESP-NOW
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    Serial.println("Dot Button Pressed");

    if (result == ESP_OK) {
    Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    return;
    }
  }
  undefined();
  
}

void buttonwork();
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  pinMode(dotButtonPin, INPUT_PULLUP);
  pinMode(dashButtonPin, INPUT_PULLUP);
  pinMode(spaceButtonPin, INPUT_PULLUP);
  pinMode(LEDPin, OUTPUT);
  pinMode(LED2Pin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

}

bool reset = false;

void loop() {

  buttonwork();

  if (lcdplacement >= 16){
    if (reset == false){
      Serial.println("LCD");
      lcdplacement = 0;
      Row0 = 1;
      reset = true;
    } else if (reset == true && lcdplacement == 17){
      Serial.println("Rest");
      tone(buzzerPin, 4000, 300);
      Row0 = 0;
      reset = false;
      lcdplacement = 0;
    }
  }

  digitalWrite(LED2Pin, LOW);

  // Serial.println("Start");

}

void buttonwork() {
    unsigned long currenttime = millis();

    // Jika dot atau dash ditekan setelah space, tandai sebagai unknown
    if ((digitalRead(dotButtonPin) == LOW || digitalRead(dashButtonPin) == LOW) && lastInputType == "space") {
        undefined();
        morseCode = "";
        Space = "";
        lastInputType = "";
        return;
    }

    // Jika space ditekan setelah dot atau dash, tandai sebagai unknown
    if (digitalRead(spaceButtonPin) == LOW && lastInputType == "morse") {
        undefined();
        morseCode = "";
        Space = "";
        lastInputType = "";
        return;
    }

    // Jika tombol dot ditekan
    if (digitalRead(dotButtonPin) == LOW) {
        while (digitalRead(dotButtonPin) == LOW) {}
        digitalWrite(LEDPin, HIGH);
        tone(buzzerPin, 1000, 100);
        delay(100);
        morseCode += ".";
        Serial.println("Dot Button Pressed");
        prevtime = currenttime;
        lastInputType = "morse";  // Perbarui status input terakhir
    }

    // Jika tombol dash ditekan
    if (digitalRead(dashButtonPin) == LOW) {
        while (digitalRead(dashButtonPin) == LOW) {}
        digitalWrite(LEDPin, HIGH);
        tone(buzzerPin, 2000, 100);
        delay(100);
        morseCode += "-";
        Serial.println("Dash Button Pressed");
        prevtime = currenttime;
        lastInputType = "morse";  // Perbarui status input terakhir
    }

    // Jika tombol space ditekan
    if (digitalRead(spaceButtonPin) == LOW) {
        while (digitalRead(spaceButtonPin) == LOW) {}
        digitalWrite(LEDPin, HIGH);
        tone(buzzerPin, 3000, 100);
        delay(100);
        Space += "~";
        Serial.println("Space Button Pressed");
        prevtime = currenttime;
        lastInputType = "space";  // Perbarui status input terakhir
    }

    digitalWrite(LEDPin, LOW);

    // Jika timeout tercapai, interpretasikan input
    if (currenttime - prevtime > timeout && morseCode.length() > 0) {
        interpretMorseCode(morseCode);
        morseCode = "";
        lastInputType = "";  // Reset setelah interpretasi
    }

    if (currenttime - prevtime > timeout && Space.length() > 0) {
        interpretSpace(Space);
        Space = "";
        lastInputType = "";  // Reset setelah interpretasi
    }
}
