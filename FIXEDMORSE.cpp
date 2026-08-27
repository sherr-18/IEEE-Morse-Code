#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 columns and 2 rows
const int dotButtonPin = 18;       // Connect the dot button to digital pin 2
const int dashButtonPin = 19;      // Connect the dash button to digital pin 3
const int spaceButtonPin = 16;
const int LEDPin = 5;          // Connect the LED to digital pin 5
const int buzzerPin = 4;

char Alphabet;
char Number;

int lcdplacement = 0;
int Row0;

bool dotButtonPressed = false;
bool dashButtonPressed = false;
bool spaceButtonPressed = false;
String morseCode = "";         // Variable to store the Morse code sequence
String Space = "";


// Morse code to letter mapping (using uppercase for comparison)
const char* morseAlphabet[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..", "-----", ".----","..---","...--","....-",".....","-....","--...","---..","----."};
const char* spacerule[] = {"~", "~~"};
unsigned long prevtime = millis();
long timeout = 1000;

void undefined() {
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

    for (int j = 0; j < 3; j++) { // Repeat 10 times (adjust as needed)
    tone(buzzerPin, 2000); // Turn on the tone at 1000 Hz
    delay(200); // Wait for 100 milliseconds
    noTone(buzzerPin); // Turn off the tone
    delay(200); // Wait for 100 milliseconds before the next blink
    }

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

    for (int j = 0; j < 3; j++) { // Repeat 10 times (adjust as needed)
    tone(buzzerPin, 2000); // Turn on the tone at 1000 Hz
    delay(200); // Wait for 100 milliseconds
    noTone(buzzerPin); // Turn off the tone
    delay(200); // Wait for 100 milliseconds before the next blink
    }

    lcd.display();
  }
  
}

void interpretSpace(String Space){
  Serial.println(Space);
  for (int i = 0; i < 2; i++) {
    if (Space == spacerule[i]) {
      Serial.println(i);
      if (i < 1) {
        lcd.print(" ");
        lcd.display();
        //lcd.clear();
        Serial.println("Space");
        lcdplacement += 1;
        Serial.println(lcdplacement);
      }

      else if (i > 0 && i < 2) {
        if (Row0 == 0) {
        lcdplacement -= 1;
        lcd.setCursor(lcdplacement, Row0);
        lcd.print(" ");
        lcd.display();
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
        }

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
      if (i < 26) {
        char Alphabet = char('A' + i);
        lcd.print(Alphabet);

      } else if (i < 36) {
        lcd.print(char('0' + i - 26)); // Print numbers
      }

      lcd.display();
      Serial.println("LCD Updated");
      lcdplacement += 1;
      Serial.println(lcdplacement);
      return;
    }
  }
  undefined();
}

void buttonwork() {

  unsigned long currenttime = millis();

  if (digitalRead(dotButtonPin) == LOW && !dotButtonPressed) {
    dotButtonPressed = true;
    Serial.println("ok");
  } 
  
  if (digitalRead(dotButtonPin) == HIGH && dotButtonPressed) {
    digitalWrite(LEDPin,HIGH);
    tone(buzzerPin, 1000, 100);
    morseCode += ".";
    delay(200); // debounce delay
    Serial.println("Dot Button Pressed");
    prevtime = currenttime;
  }

  if (digitalRead(dashButtonPin) == LOW && !dashButtonPressed) {
    dashButtonPressed = true;
    Serial.println("ok");
  } 
  
  if (digitalRead(dashButtonPin) == HIGH && dashButtonPressed) {
    digitalWrite(LEDPin,HIGH);
    tone(buzzerPin, 1000, 100);
    morseCode += "-";
    delay(200); // debounce delay
    Serial.println("Dash Button Pressed");
    prevtime = currenttime;
  }

  if (digitalRead(spaceButtonPin) == LOW && !spaceButtonPressed) {
    spaceButtonPressed = true;
    Serial.println("ok");
  } 
  
  if (digitalRead(spaceButtonPin) == HIGH && spaceButtonPressed) {
    digitalWrite(LEDPin,HIGH);
    tone(buzzerPin, 1000, 100);
    Space += "~";
    delay(200); // debounce delay
    Serial.println("Dash Button Pressed");
    prevtime = currenttime;
  }

    digitalWrite(LEDPin, LOW);
    dotButtonPressed = false;
    dashButtonPressed = false;
    spaceButtonPressed = false;

  if (currenttime - prevtime > timeout && morseCode.length() > 0) {
    interpretMorseCode(morseCode);
    morseCode = ""; // Reset Morse code sequence after confirmation
    Serial.println("Translated!!!");
  }

  if (currenttime - prevtime > timeout && Space.length() > 0) {
    interpretSpace(Space);
    Space = "";
    Serial.println("BackSpace!!!");
  }
}

void setup() {
  Serial.begin(115200);  // Initialize serial communication

  pinMode(dotButtonPin, INPUT_PULLUP);
  pinMode(dashButtonPin, INPUT_PULLUP);
  pinMode(spaceButtonPin, INPUT_PULLUP);
  pinMode(LEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();

}

bool reset = false;

void loop() {

  buttonwork();

  if (lcdplacement >= 16){
     Serial.println("LCD");
    if (reset == false){
      lcdplacement = 0;
      lcd.setCursor(0 ,1);
      Row0 = 1;
      reset = true;
    } else if (reset == true && lcdplacement >= 16){
      Serial.println("Rest");
      lcd.clear();
      lcd.setCursor(0,0);
      Row0 = 0;
      reset = false;
      lcdplacement = 0;
    }
  }
}



