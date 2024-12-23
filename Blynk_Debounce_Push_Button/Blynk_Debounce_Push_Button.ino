//Arduino - Debounce a Push Button

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <SimpleTimer.h> // ไลบรารีสำหรับ BlynkTimer (ใน Blynk2 ใช้ BlynkTimer แทน SimpleTimer)

// กำหนดค่า Pin และตัวแปร
int fogPin = 13;         // ขา Fog
int springerPin = 12;    // ขา Springer
int buttonPin1 = 2;      // ปุ่ม 1
int buttonPin2 = 3;      // ปุ่ม 2
int buttonState1 = 0;
int buttonState2 = 0;
int lastButtonState1 = LOW;
int lastButtonState2 = LOW;

unsigned long lastDebounceTime1 = 0; 
unsigned long lastDebounceTime2 = 0; 
unsigned long debounceDelay = 50;   

// Blynk และ WiFi
char auth[] = "YourAuthToken"; 
char ssid[] = "YourSSID";
char pass[] = "YourPassword";

// Blynk Timer
BlynkTimer timer;

// ฟังก์ชันควบคุม Fog และ Springer
void turnOnFog() {
  digitalWrite(fogPin, HIGH); // เปิด Fog
  Serial.println("Fog is ON");
  Blynk.virtualWrite(V1, 1);  // ส่งสถานะไปยัง Blynk
}

void turnOffFog() {
  digitalWrite(fogPin, LOW); // ปิด Fog
  Serial.println("Fog is OFF");
  Blynk.virtualWrite(V1, 0); // ส่งสถานะไปยัง Blynk
}

void turnOnSpringer() {
  digitalWrite(springerPin, HIGH); // เปิด Springer
  Serial.println("Springer is ON");
  Blynk.virtualWrite(V2, 1);       // ส่งสถานะไปยัง Blynk
}

void turnOffSpringer() {
  digitalWrite(springerPin, LOW); // ปิด Springer
  Serial.println("Springer is OFF");
  Blynk.virtualWrite(V2, 0);     // ส่งสถานะไปยัง Blynk
}

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  // ตั้งค่าขา Pin
  pinMode(fogPin, OUTPUT);
  pinMode(springerPin, OUTPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);

  // ปิด Fog และ Springer เริ่มต้น
  digitalWrite(fogPin, LOW);
  digitalWrite(springerPin, LOW);

  // ตั้งเวลา (ตัวอย่าง: ปิด Fog และ Springer หลัง 5 วินาที)
  timer.setTimeout(5000L, turnOffFog);      // ปิด Fog หลัง 5 วินาที
  timer.setTimeout(5000L, turnOffSpringer); // ปิด Springer หลัง 5 วินาที
}

void loop()
{
  Blynk.run();
  timer.run(); // เรียกใช้ BlynkTimer

  // อ่านค่าปุ่มพร้อม debounce
  int reading1 = digitalRead(buttonPin1);
  int reading2 = digitalRead(buttonPin2);

  // ตรวจสอบการเปลี่ยนสถานะของปุ่ม 1
  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = millis();
  }

  if ((millis() - lastDebounceTime1) > debounceDelay) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;

      if (buttonState1 == HIGH) {
        turnOnFog(); // เปิด Fog
      }
    }
  }

  // ตรวจสอบการเปลี่ยนสถานะของปุ่ม 2
  if (reading2 != lastButtonState2) {
    lastDebounceTime2 = millis();
  }

  if ((millis() - lastDebounceTime2) > debounceDelay) {
    if (reading2 != buttonState2) {
      buttonState2 = reading2;

      if (buttonState2 == HIGH) {
        turnOnSpringer(); // เปิด Springer
      }
    }
  }

  // บันทึกสถานะล่าสุดของปุ่ม
  lastButtonState1 = reading1;
  lastButtonState2 = reading2;
}
