#define CLK 5
#define DIO 4
#define BUT_RESET 10
#define BUT_MODE 12
#define SENSOR_1 2
#define SENSOR_2 4
#define LED 9
#include "GyverTM1637.h"
bool startFlag = false;
bool qc = false;
bool repetitionDone = false;
bool timerStarted = false;
unsigned long t = 0;
GyverTM1637 disp(CLK, DIO);


void setup() {
  Serial.begin(9600);
  pinMode(BUT_RESET, INPUT);
  pinMode(BUT_MODE, INPUT);
  pinMode(BUT_RESET, INPUT);
  pinMode(LED, OUTPUT);
  disp.clear();
  disp.brightness(7);
}

void loop() {
  Serial.print(analogRead(SENSOR_1));
  Serial.print(" ");
  Serial.print(analogRead(SENSOR_2));
  Serial.print(" ");
  long s1 = analogRead(SENSOR_1);
  long s2 = analogRead(SENSOR_2);
  if (digitalRead(BUT_MODE) == HIGH) {
    digitalWrite(LED, HIGH);
    if(s1 > 100 && timerStarted == true){
      digitalWrite(LED, LOW);
    }else if(timerStarted == true){
      digitalWrite(LED, HIGH);
    }
    if (digitalRead(BUT_RESET) == HIGH) {
      startFlag = true;
      timerStarted = false;
      t = 0;
      disp.display(0, 0, 0, 0);
      delay(100);
    }
    if(s1 > 100 && startFlag == false){
      if(round(millis() / 100) % 2 == 0){
        digitalWrite(LED, HIGH);
      }else{
        digitalWrite(LED, LOW);
      }
    }else if(timerStarted == true){
      digitalWrite(LED, LOW);
    }
    if (startFlag == true) {
      if (s1 > 800 && timerStarted == false) {
        t = millis();
        timerStarted = true;
        repetitionDone = false;
        Serial.print(t);
        Serial.print(" ");
      }
      if (timerStarted == true) {
        displayTime(millis() - t);
        Serial.print(millis() - t);
        Serial.print(" ");
        if(s1 < 200 && repetitionDone == false){
          repetitionDone = true;
        }
      }

      if ((s1 > 800  || digitalRead(BUT_RESET) == HIGH) && repetitionDone == true) {
        startFlag = false;
        timerStarted = false;
        t = millis() - t;
        displayTime(t);
      }
      Serial.print(startFlag);
        Serial.println(" ");
    }
  } else {
    digitalWrite(LED, LOW);
    if ((s1 > 100 || s2 > 100) && timerStarted == false) {
      qc = false;
      if(round(millis() / 100) % 2 == 0){
        digitalWrite(LED, HIGH);
      }else{
        digitalWrite(LED, LOW);
      }
    } else if (timerStarted == false) {
      qc = true;
    }
    if((s1 > 100 || s2 > 100) && timerStarted == true){
      digitalWrite(LED, HIGH);
    }else if(timerStarted == true){
      digitalWrite(LED, LOW);
    }
    if (digitalRead(BUT_RESET) == HIGH && qc == true) {
      startFlag = true;
      timerStarted = false;
      t = 0;
      delay(100);
      disp.display(0, 0, 0, 0);
    }
    if (startFlag == true) {
      if (s1 > 800 && timerStarted == false) {
        t = millis();
        timerStarted = true;
        Serial.println(t);
      }
      if (timerStarted == true) {
        displayTime(millis() - t);
        Serial.println(millis() - t);
      }
      if ((s2 > 800 || digitalRead(BUT_RESET) == HIGH) && timerStarted == true) {
        startFlag = false;
        timerStarted = false;
        t = millis() - t;
        Serial.println(t);
        displayTime(t);
      }
    }
  }
}

void displayTime(long mil) {
  int d1, d2, d3, d4;
  if (mil < 10000) {
    mil /= 10;
    d1 = 0;
    d2 = mil / 100;
    //Serial.println(d2);
    d3 = (mil / 10) % 10;
    // Serial.println(d3);
    d4 = mil % 10;
    //Serial.println(d4);
    disp.point(true);
  } else if (mil < 60000) {
    mil /= 10;
    d1 = mil / 1000;
    d2 = (mil / 100) % 10;
    d3 = (mil / 10) % 10;
    d4 = mil % 10;
    disp.point(true);
  } else if (mil < 6000000) {
    //mil /= 10;
    d1 = ((mil / 1000)  / 60) / 10;
    d2 = ((mil / 1000) / 60) % 10;
    d3 = ((mil / 1000) % 60) / 10;
    d4 = ((mil / 1000) % 60) % 10;
    disp.point(true);
  }
  disp.display(d1, d2, d3, d4);

}
