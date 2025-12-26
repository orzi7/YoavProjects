#include <Wire.h>
#include <Servo.h>
#include <math.h> // נדרש לחישוב הזווית

Servo myServo;
const int MPU_ADDR = 0x68;

// *** הגדרות שליטה ***
const int TARGET_ANGLE = 0; // הזווית שאנחנו רוצים (0 = מאוזן)
const float Kp = 15.0;      // "כוח התיקון". אם המנוע מגיב חלש מדי, תגדיל. אם הוא משתולל, תקטין.

void setup() {
  Serial.begin(9600);
  Wire.begin();
  myServo.attach(9);

  // --- אתחול המנוע (ESC) ---
  // שליחת סיגנל "עצור" כדי שה-ESC יידרך
  myServo.writeMicroseconds(1500); 
  delay(1000); 

  // --- אתחול החיישן ---
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

void loop() {
  // 1. קריאת נתוני ה-Accelerometer (החל מרגיסטר 0x3B)
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true); // מבקשים 6 בייטים (X, Y, Z של המאוץ)

  if (Wire.available() == 6) {
    int16_t AcX = Wire.read() << 8 | Wire.read();
    int16_t AcY = Wire.read() << 8 | Wire.read();
    int16_t AcZ = Wire.read() << 8 | Wire.read();

    // 2. חישוב הזווית הנוכחית (במעלות)
    // אנו משתמשים ב-atan2 כדי לקבל זווית מדויקת ביחס לכוח המשיכה
    // ההנחה היא שהציר שמסתובב הוא Y. אם זה לא עובד, נסה להחליף בין AcX ל-AcY
    float angle = atan2(AcY, AcZ) * 180 / PI;

    // 3. חישוב השגיאה והתיקון
    float error = TARGET_ANGLE - angle;
    
    // חישוב עוצמת המנוע (1500 זה המרכז/עצירה)
    // אם הזווית חיובית מדי -> צריך להוריד כוח (או להפך, תכף נבדוק)
    int pwmOutput = 1500 + (error * Kp);

    // 4. הגבלת הערכים (Safety)
    // המנוע שלך עובד בין 1100 ל-1900. לא נחרוג מזה.
    pwmOutput = constrain(pwmOutput, 1100, 1900);

    // 5. שליחה למנוע
    myServo.writeMicroseconds(pwmOutput);

    // הדפסה לדיבאג (כדי שתוכל לראות מה קורה)
    Serial.print("Angle: "); Serial.print(angle);
    Serial.print(" | Error: "); Serial.print(error);
    Serial.print(" | Motor PWM: "); Serial.println(pwmOutput);
  }

  delay(50); // עידכון מהיר (20 פעמים בשנייה)
}
