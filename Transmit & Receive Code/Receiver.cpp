/*
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/  
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns and 2 rows
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    int a;
    \
} struct_message;

// Create a struct_message called myData
struct_message myData;

int lcdplacement = 0;
int Row0;
const int buzzerPin = 17;
bool reset = false;

void undefined() {
  Serial.println(myData.a);

  tone(buzzerPin, 1000, 100);
  delay(100);

  if (myData.a > 39 && myData.a < 41){
    // Handle incorrect or incomplete code
  Serial.println("Unknown Morse Code");
  lcdplacement += 1;
  
  if (Row0 == 0){
    for (int j = 0; j < 3; j++) { // Repeat 10 times (adjust as needed)
      lcd.setCursor(lcdplacement - 1, Row0);
      lcd.print("X");
      Serial.println("X Printed");
      delay(100);
      lcd.setCursor(lcdplacement - 1, Row0);
      lcd.print(" ");
      Serial.println("Printed");
      delay(100);
    }

    Serial.println(lcdplacement);
    lcd.setCursor(lcdplacement - 1, Row0);
    lcd.print("?");
    Serial.println("? Printed");
    lcd.display();
  }

  if (Row0 == 1){
    for (int j= 0; j < 3; j++) { // Repeat 10 times (adjust as needed)
      lcd.setCursor(lcdplacement - 1, Row0);
      lcd.print("X");
      Serial.println("X Printed");
      delay(100);
      lcd.setCursor(lcdplacement - 1, Row0);
      lcd.print(" ");
      Serial.println("Printed");
      delay(100);
    }
    Serial.println(lcdplacement);
    lcd.setCursor(lcdplacement - 1, Row0);
    lcd.print("?");
    Serial.println("? Printed");
    lcd.display();
  }

  }
}


void interpretmorse(){
      Serial.println(myData.a);
      if (myData.a < 26) {
        lcd.print(char('A' + myData.a));
        Serial.println(char('A' + myData.a));

        lcd.display();
        lcdplacement += 1;
        Serial.println(lcdplacement);

      } else if (myData.a < 36) {
        lcd.print(char('0' + myData.a - 26)); // Print numbers
        lcd.display();
        lcdplacement += 1;
        Serial.println(lcdplacement);
      }

      tone(buzzerPin, 1000, 100);
      delay(100);
}

void interpretSpace (){
  
      Serial.println(myData.a);
      if (myData.a > 36 && myData.a < 38) {
        lcdplacement += 1;
        lcd.print(" ");
        lcd.display();
        //lcd.clear();
        Serial.println("Space");
        Serial.println(lcdplacement);
      }

      else if (myData.a > 37 && myData.a < 39) {
        if (Row0 == 0) {
        lcdplacement -= 1;
        lcd.setCursor(lcdplacement, Row0);
        lcd.print(" ");
        //lcd.clear();
        Serial.println("BackSpace");
        Serial.println(lcdplacement);
        lcd.setCursor(lcdplacement, Row0);
        }
         if (Row0 == 1) {
        lcdplacement -= 1;
        lcd.setCursor(lcdplacement, Row0);
        lcd.print(" ");
        lcd.display();
        //lcd.clear();
        Serial.println("BackSpace");
        Serial.println(lcdplacement);
        lcd.setCursor(lcdplacement, Row0);
          if (lcdplacement == -1) {
            Row0 = 0;
            Serial.println(Row0);
            lcd.setCursor(lcdplacement + 16, Row0);
            lcd.print(" ");
            lcdplacement = 15;
            Row0 = 0;
            lcd.display();
            reset = false;
            //lcd.clear();
            Serial.println("BackSpace");
            Serial.println(lcdplacement);
            lcd.setCursor(lcdplacement, Row0);

          }
        }
      }

}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));

  interpretSpace();
  interpretmorse();
  undefined();

  if (lcdplacement >= 16){
     Serial.println("LCD");
    if (reset == false){
      lcdplacement = 0;
      lcd.setCursor(0 ,1);
      Row0 = 1;
      reset = true;
    } else if (reset == true && lcdplacement == 17){

      Serial.println("Rest");
      lcd.clear();
      lcd.setCursor(0,0);
      Row0 = 0;
      reset = false;
      lcdplacement = 0;
    }
  }

}

 
void setup() {

  Wire.begin(16,4);
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  lcd.init();
  lcd.backlight();

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
}
 
void loop() {

}