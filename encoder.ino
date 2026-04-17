// --- פינים ---
const int encoderPin = 2; // חייב להיות פין פסיקה (Interrupt)
const int joyYPin = A0;   // ג'ויסטיק
const int enA = 9;        // מהירות מנוע (PWM)
const int in1 = 8;        // כיוון מנוע 1
const int in2 = 10;       // כיוון מנוע 2

// --- משתני אינקודר למדידת מהירות ---
volatile unsigned long pulseCount = 0; 
unsigned long previousMillis = 0;
const int interval = 1000; // מדידה כל 1000 אלפיות שניה (שניה)
const float pulsesPerRevolution = 20.0; // שנה למספר הפולסים לסיבוב של האינקודר שלך

void setup() {
  Serial.begin(9600);
  
  // הגדרות אינקודר
  pinMode(encoderPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(encoderPin), countPulse, FALLING);
  
  // הגדרות מנוע
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  
  // מתחילים כשהמנוע כבוי
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);

  Serial.println("System Ready. Printing Encoder RPM only...");
}

void loop() {
  // ==========================================
  // חלק 1: שליטה על המנוע בעזרת הג'ויסטיק
  // ==========================================
  int joyVal = analogRead(joyYPin); 
  int motorSpeed = 0;

  // שטח מת - הג'ויסטיק באמצע
  if (joyVal >= 470 && joyVal <= 550) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    motorSpeed = 0;
  }
  // ג'ויסטיק קדימה
  else if (joyVal > 550) {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    motorSpeed = map(joyVal, 550, 1023, 0, 255);
  }
  // ג'ויסטיק אחורה
  else if (joyVal < 470) {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    motorSpeed = map(joyVal, 470, 0, 0, 255);
  }

  // הפעלת המנוע במהירות המחושבת (ללא הדפסה)
  analogWrite(enA, motorSpeed);


  // ==========================================
  // חלק 2: חישוב והדפסת מהירות האינקודר
  // ==========================================
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // עצירת פסיקות רגעית להעתקת הנתונים בבטחה
    noInterrupts();
    unsigned long pulses = pulseCount;
    pulseCount = 0; 
    interrupts(); 

    // חישוב RPM
    float rotationsPerSecond = pulses / pulsesPerRevolution;
    float rpm = rotationsPerSecond * 60.0;

    // הדפסה של נתוני האינקודר בלבד
    Serial.print("Encoder Pulses: ");
    Serial.print(pulses);
    Serial.print("  |  Actual Motor Speed: ");
    Serial.print(rpm);
    Serial.println(" RPM");

    previousMillis = currentMillis;
  }
}

// פונקציית הפסיקה - מעלה את המונה בכל קליק של האינקודר
void countPulse() {
  pulseCount++;
}
