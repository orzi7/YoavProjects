#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

int minutes = 0;
int seconds = 0;

void setTime(int startMinutes, int startSeconds) {
  minutes = startMinutes;
  seconds = startSeconds;
}

void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.clear();         
  lcd.backlight();
  setTime(3, 30);
}

void loop() {
  delay(1000);
  lcd.clear();
  if (minutes >= 0 && seconds >= 0) {
    seconds -= 1;
    if (seconds <= 0) {
      minutes -= 1;
      seconds = 59;
    }
    if (minutes >= 10) {
      lcd.setCursor(6,0);
      lcd.print(minutes);
    } else {
      lcd.setCursor(6,0);
      lcd.print("0");
      lcd.setCursor(7,0);
      lcd.print(minutes);
    }

    lcd.setCursor(8,0);
    lcd.print(":");

    if (seconds >= 10) {
      lcd.setCursor(9,0);
      lcd.print(seconds);
    } else {
      lcd.setCursor(9,0);
      lcd.print("0");
      lcd.setCursor(10,0);
      lcd.print(seconds);
    }
  } else {
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Time's up!");
  }

}
