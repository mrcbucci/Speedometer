// Schermo
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Temperatura
#include <OneWire.h>
#include <DallasTemperature.h>
int tempPin = 5;
DeviceAddress sensore1 = {0x28, 0x27, 0x71, 0x96, 0xF0, 0x01, 0x3C, 0x31};
OneWire oneWire(tempPin);
DallasTemperature temp(&oneWire);
float tC;

// VARIABILI
// Menù
int menu = 0;

// Cronometro
int crono = 0;
float t = 0;
int m = 0;
float s = 0;
int Lm = 0;
float Ls = 0;
int Bm = 9;
float Bs = 59;

// Tasti
const int button1Pin = 2;
int button1State;
int lastButton1State = LOW;
const int button2Pin = 4;
int button2State;
int lastButton2State = LOW;

// Debounce
unsigned long debounceTime = 0;
unsigned long debounceDelay = 500;




void setup() {
  // Tasti
  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);
  // Temperatura
  temp.begin();
  temp.setResolution(sensore1, 10);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(5, 1);
  lcd.print("Benvenuto!");
  delay(2000);
  lcd.clear();
}


void loop() {

  // Menù + Debounce software
  if (digitalRead(button1Pin)) {
    if ((millis() - debounceTime) > debounceDelay) {
      // Comando
      lcd.clear();
      menu++;
      if (menu >= 2) {
        menu = 0;
      }
      debounceTime = millis();
    }
  }

  switch (menu) {

    case 0:
      //Temperatura
      temp.requestTemperatures();
      tC = temp.getTempCByIndex(0);
      lcd.setCursor(0, 1);
      lcd.print("Temp: ");
      lcd.print(tC);
      lcd.print((char)223);
      lcd.print("C");
      break;

    case 1:
      // Tasto avvio cronometro + Debounce software
      if (digitalRead(button2Pin)) {
        if ((millis() - debounceTime) > debounceDelay) {
          // Comando
          crono = 1;
          t = millis();
          lcd.clear();
          if (s != 0) {
            Lm = m;
            Ls = s;
            m = 0;
            // MIGLIOR TEMPO
            if (Lm <= Bm) {
              Bm = Lm;
              if (Ls <= Bs) {
                Bs = Ls;
              }
            }
            // FINE MIGLIOR TEMPO
          }
          debounceTime = millis();
        }
      }

      if (crono == 1) {
        s = (millis() - t) / 1000;
        if ( s > 60) {
          m++;
          t = millis();
        }
      }
      lcd.setCursor(0, 1);
      lcd.print("LAP      ");
      lcd.print(m);
      lcd.print(".");
      lcd.print(s);

      lcd.setCursor(0, 2);
      lcd.print("LAST     ");
      lcd.print(Lm);
      lcd.print(".");
      lcd.print(Ls);

      lcd.setCursor(0, 3);
      lcd.print("BEST     ");
      lcd.print(Bm);
      lcd.print(".");
      lcd.print(Bs);
      break;

    case 2:

      break;

  }
}
