#include <Servo.h>

#define TRIG_PIN A4
#define ECHO_PIN A5

Servo myServo;
int servoPos = 90; // start at mid

int THRESH = 20; // distance threshold in cm

// Function to get distance from HC-SR04
long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
   // assume far if no echo
  return duration / 29 / 2;
}

// Move servo to right
void servoRight() { 
  myServo.write(0); 
  delay(500); 
}

// Move servo to left
void servoLeft() { 
  myServo.write(180); 
  delay(500); 
}

// Move servo to mid
void servoMid() { 
  myServo.write(90); 
  delay(500); 
}

void setup() {
  Serial.begin(9600);
  myServo.attach(9); // attach servo to pin 9
  myServo.write(servoPos);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  // Check front
  servoMid();
  long frontDist = getDistance();
  Serial.print("Front: "); Serial.println(frontDist);

  if(frontDist < THRESH && frontDist > 0) {
    // Front blocked → turn right
    servoRight();
    long rightDist = getDistance();
    Serial.print("Right: "); Serial.println(rightDist);

    if(rightDist < THRESH) {
      // Right blocked → check left
      servoMid();
      delay(300);
      servoLeft();
      long leftDist = getDistance();
      Serial.print("Left: "); Serial.println(leftDist);

      if(leftDist < THRESH) {
        // Left also blocked → back to mid
        servoMid();
        Serial.println("All sides blocked!");
      }
    }

    // Return to mid after scan
    servoMid();
  }

  delay(200);
}
