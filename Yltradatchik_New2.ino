# define TRIG_PIN A0 //ультрадатчик
# define ECHO_PIN A1
#define CLK 6  //вроде 7-сегментный дисплей(не используется)
#define DIO 7
#define Dvig_R 13  //моторы
#define Speed_R 11
#define Dvig_L 12
#define Speed_L 10
//#include "GyverTM1637.h"  // для дисплея(на используется)
#include <NewPing.h>  //для ультрадатчика
#include <Servo.h>    //для сервопривода
Servo myservo;
int arr[2][180];
int max_znach = 0;
int max_angle = 0;
int min_znach = 1000;
int min_angle = 0;
const int NUM = 180;
byte m1S = 255;
byte m2S = 0;
float k = 16.5; //подбирается индивидуально для каждой машины (надо изменить)*
int Fi = 0;
int tims = 0; // время движения
NewPing sonar(TRIG_PIN, ECHO_PIN, 700);  //ультрадатчик
//GyverTM1637 disp(CLK, DIO);// дисплей
void setup() {
  Serial.begin(9600);
  pinMode(Dvig_R, OUTPUT);
  pinMode(Speed_R, OUTPUT);
  pinMode(Dvig_L, OUTPUT);
  pinMode(Speed_L, OUTPUT);
  myservo.attach(9);
  myservo.write(0);
}

void loop() {
  Distant();
  Result();
}

void Distant() {
  for (int i = 0; i <= NUM; ++i) {
    myservo.write(i);
    arr[0][i] = i;
    //  Serial.print(myservo.read());
    delay(10);
    arr[1][i] = sonar.ping_cm();
    if (sonar.ping_cm() < 5) {
      arr[1][i] = 5;
    }
    //   Serial.print('\t');
    // Serial.println(arr[1][i]);
    if ( arr[1][i] > max_znach )  {  //поиск максимального значения
      max_znach = arr[1][i];
      max_angle = arr[0][i];
    }
    //    if ( arr[1][i] < min_znach ) {  //поиск минимального
    //      min_znach = arr[1][i];
    //      min_angle = arr[0][i];
    //    }
  }
}
void Result() {
  //Serial.print("min distance = ");
  //Serial.print(min_znach);
  //Serial.println(" cm");
  //Serial.print("min angle = ");
  //Serial.println(min_angle);
  //Serial.println("----------------------------------------------");
  Serial.print("max distance = ");
  Serial.println(max_znach);
  Serial.println(" cm");
  Serial.print("max angle = ");
  Serial.println(max_angle);
  Serial.println("------------------------------------------------");
  Fi = 90 - max_angle;
  tims = k * Fi;
  delay(1000);
  for (int i = 180; i >= max_angle; --i) {
    myservo.write(i);
    delay(15);
  }
  delay(2000);
  
  if (max_angle <= 85) {
    F_Right();
    delay(tims);
    Forward();
     if (sonar.ping_cm() < 25) {
      Stop();
    }
  }
  else {
    Forward();
    if (sonar.ping_cm() < 25) {
      Stop();
    }
  }
  if (max_angle >= 95) {
    F_Left();
    delay(tims);
    Forward();
     if (sonar.ping_cm() < 25) {
      Stop();
    }
  }
  else {
    Forward();
    if (sonar.ping_cm() < 25) {
      Stop();
    }
  }
  
  for (int i = max_angle; i >= 0; --i) {
    myservo.write(i);
    delay(20);
  }
  // for (int i = 180; i >= min_angle; --i) {
  //   myservo.write(i);
  //   delay(15);
  // }
  //  disp.displayInt(min_znach);
  //  delay(2000);
  // for (int i = min_angle; i > 0; --i) {
  //   myservo.write(i);
  //    delay(20);
  //}
  max_znach = 0;
  max_angle = 0;
  min_znach = 1000;
  min_angle = 0;
}
void Forward() {
  digitalWrite(Dvig_R, HIGH);
  analogWrite(Speed_R, m1S);
  digitalWrite(Dvig_L, HIGH);
  analogWrite(Speed_L, m1S);

}
void F_Right() {
  digitalWrite(Dvig_R, LOW);
  analogWrite(Speed_R, m1S);
  digitalWrite(Dvig_L, HIGH);
  analogWrite(Speed_L, m1S);
}
void F_Left() {
  digitalWrite(Dvig_R, HIGH);
  analogWrite(Speed_R, m1S);
  digitalWrite(Dvig_L, LOW);
  analogWrite(Speed_L, m1S);
}
void Stop() {
  digitalWrite(Dvig_R, LOW);
  analogWrite(Speed_R, m2S);
  digitalWrite(Dvig_L, LOW);
  analogWrite(Speed_L, m2S);
}
