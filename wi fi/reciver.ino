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

const int servoRightPin = 9; //stronger
const int servoLeftPin = 10; //weaker

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

int rightSpeed = minSpeed;
int leftSpeed = minSpeed;

int lastRightPWM = 1100;
int lastLeftPWM = 1100;
const int MAX_PWM_STEP = 30;

const float Kp = 1.6;
const float Kd = 0.1;


int motorControl(String motor, float KP, float KD, int TARGET_ANGLE, int basicSpeed) {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  int pwmOutput = basicSpeed; 
  float lastError = 0;
  float lastMillis = 0;

  if (Wire.available() == 6) {
    // float currentMillis = millis();
    int16_t AcX = Wire.read() << 8 | Wire.read();
    int16_t AcY = Wire.read() << 8 | Wire.read();
    int16_t AcZ = Wire.read() << 8 | Wire.read();

    float angle = atan2(AcY, AcZ) * 180 / PI;
    currentAngle = angle; 

    float error = TARGET_ANGLE - angle;
    float correction = (error * KP) + ((lastError - error) * KD);
    
    if (motor == "left") {
      pwmOutput = basicSpeed + correction;
      if (pwmOutput > maxSpeed)  pwmOutput = maxSpeed;
    }
    
    else if (motor == "right") {
      int rawRight = (basicSpeed - correction);
      if (rawRight < (minSpeed + 200)) {
        pwmOutput = rawRight;
      } else {
        pwmOutput = rawRight * multification;
      }
      if (pwmOutput > maxSpeed * multification)  pwmOutput = maxSpeed * multification;
    }

    if (pwmOutput < minSpeed)  pwmOutput = minSpeed;
    
    lastError = error;
    // lastMillis = currentMillis;
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

    Serial.println(pwmValue);
  }

  if (pwmValue == 0) {
    lastRightPWM = minSpeed;
    lastLeftPWM = minSpeed;
    rightServo.write(minSpeed);
    leftServo.write(minSpeed);
  } else {
    rightSpeed = motorControl("right", Kp, Kd, 0, pwmValue);
    leftSpeed = motorControl("left", Kp + 1, Kd, 0, pwmValue);

    rightServo.write(rightSpeed);
    leftServo.write(leftSpeed);
  }

  if (millis() - lastDisplayTime >= displayInterval) {
    lastDisplayTime = millis();
    
    lcd.setCursor(0, 0);
    lcd.print("Angle: ");
    lcd.print(currentAngle, 1); 
    lcd.print("   "); 
    
    lcd.setCursor(0, 1);
    lcd.print("L:");
    lcd.print(leftSpeed);
    lcd.print(" R:");
    lcd.print(rightSpeed);
    lcd.print("   "); 
  }
}
