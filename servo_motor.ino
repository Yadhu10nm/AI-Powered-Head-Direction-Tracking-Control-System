#include <Servo.h>

Servo myservo;
String pos = "center";

void setup() {
  Serial.begin(9600);
  myservo.attach(10);      // attach before writing
  myservo.write(90);       // start centered
}

void left() {
  for (int i = 90; i >= 0; i -= 5) {
    myservo.write(i);
    delay(30);             // smoother motion
  }
  pos = "left";
}

void right() {
  for (int i = 90; i <= 180; i += 5) {
    myservo.write(i);
    delay(30);
  }
  pos = "right";
}

void center() {
  if (pos == "left") {
    for (int i = 0; i <= 90; i += 5) {
      myservo.write(i);
      delay(30);
    }
  } else if (pos == "right") {
    for (int i = 180; i >= 90; i -= 5) {
      myservo.write(i);
      delay(30);
    }
  }
  pos = "center";
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'L') {
      left();
    } 
    else if (command == 'R') {
      right();
    } 
    else if (command == 'C') {
      center();
    }
  }
}
