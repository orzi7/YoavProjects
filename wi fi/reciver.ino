#include <Wire.h>
#include <ServoTimer2.h>
#include <math.h>
#include <RH_ASK.h>
#include <LiquidCrystal_I2C.h> 

#ifdef RH_HAVE_HARDWARE_SPI
#include <SPI.h> 
#endif

RH_ASK driver(2000, 11, 4, 5);

ServoTimer2 rightServo;
ServoTimer2 leftServo;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

const int MPU_ADDR = 0x68;

const int servoRightPin = 9;
const int servoLeftPin = 10;

const int minSpeed = 1100;
const int maxSpeed = 1800;
const int minFlightSpeed = 1200;

const int deadzone = 20;
const int middleJoystick = 430;

const float multification = 2200.0 / 2450.0;

int pwmValue = 1100;

unsigned long lastDisplayTime = 0;
const unsigned long displayInterval = 250; 
float currentAngle = 0.0;

int lastRightPWM = 1100;
int lastLeftPWM = 1100;
const int MAX_PWM_STEP = 30;

int motorControl(String motor, float KP, int TARGET_ANGLE, int basicSpeed) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  int pwmOutput = basicSpeed; 

  if (Wire.available() == 6) {
    int16_t AcX = Wire.read() << 8 | Wire.read();
    int16_t AcY = Wire.read() << 8 | Wire.read();
    int16_t AcZ = Wire.read() << 8 | Wire.read();

    float angle = atan2(AcY, AcZ) * 180 / PI;
    currentAngle = angle; 

    float error = TARGET_ANGLE - angle;
    
    if (motor == "left") {
      int netSpeed = basicSpeed - minSpeed;
      pwmOutput = minSpeed + (netSpeed * multification) + (error * KP);
    }
    else if (motor == "right") {
      pwmOutput = basicSpeed - (error * KP);
    }

    if (pwmOutput < minSpeed)  pwmOutput = minSpeed;
    if (pwmOutput > maxSpeed)  pwmOutput = maxSpeed;

    return pwmOutput;
  }
  
  return pwmOutput; 
}

void setup() {
  Serial.begin(9600);
  if (!driver.init()) Serial.println("init failed");
  
  Wire.begin();
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);    
  Wire.endTransmission(true);

  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Drone Init...");

  rightServo.attach(servoRightPin);
  leftServo.attach(servoLeftPin);
  
  rightServo.write(1100); 
  leftServo.write(1100); 
  delay(3000);
  
  lcd.clear();
}

void loop() {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);

  if (driver.recv(buf, &buflen)) {
    buf[buflen] = '\0'; 
    pwmValue = atoi((char*)buf); 
  }

  if (pwmValue == 0) {
    lastRightPWM = minSpeed;
    lastLeftPWM = minSpeed;
    rightServo.write(minSpeed);
    leftServo.write(minSpeed);
  } else {
    int targetRight = motorControl("right", 1.2, 0, pwmValue);
    int targetLeft = motorControl("left", 1.2, 0, pwmValue);

    int diffRight = targetRight - lastRightPWM;
    diffRight = constrain(diffRight, -MAX_PWM_STEP, MAX_PWM_STEP);
    lastRightPWM += diffRight;

    int diffLeft = targetLeft - lastLeftPWM;
    diffLeft = constrain(diffLeft, -MAX_PWM_STEP, MAX_PWM_STEP);
    lastLeftPWM += diffLeft;

    rightServo.write(lastRightPWM);
    leftServo.write(lastLeftPWM);
  }

  if (millis() - lastDisplayTime >= displayInterval) {
    lastDisplayTime = millis();
    
    lcd.setCursor(0, 0);
    lcd.print("Angle: ");
    lcd.print(currentAngle, 1); 
    lcd.print("   "); 
    
    lcd.setCursor(0, 1);
    lcd.print("PWM: ");
    lcd.print(pwmValue);
    lcd.print("   ");
  }
}
