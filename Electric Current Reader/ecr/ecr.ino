#include <LiquidCrystal_PCF8574.h>
#include "ACS712.h"

LiquidCrystal_PCF8574 lcd(0x27); 
ACS712 sensor(ACS712_30A, A0);
ACS712 sensor1(ACS712_30A, A1);
ACS712 sensor2(ACS712_30A, A2);

float Arus1,Arus2,Arus3;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2); 
  Serial.println("Program Siap");
}


void loop() {
  Arus1 = sensor.getCurrentAC();
  Arus2 = sensor1.getCurrentAC();
  Arus3 = sensor2.getCurrentAC();
  lcd_view();
  String minta = "";
  delay(1000);
}

void lcd_view()
{
  lcd.setBacklight(255);
  lcd.setCursor(0, 0);
  lcd.print("Arus AC1=    ");
  lcd.setCursor(10, 0);
  lcd.print(Arus1);
  lcd.setCursor(15, 0);
  lcd.print("A");
  lcd.setCursor(0, 1);
  lcd.print("Arus AC2=    ");
  lcd.setCursor(10, 1);
  lcd.print(Arus2);
  lcd.setCursor(15, 1);
  lcd.print("A");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Arus AC3=    ");
  lcd.setCursor(10, 0);
  lcd.print(Arus3);
  lcd.setCursor(15, 0);
  lcd.print("A");
  delay(2000);
}
