#include <Servo.h>


const int trigEntry = 9;
const int echoEntry = 10;
const int trigExit = 7;
const int echoExit = 8;
const int servoPin = 6; 

Servo myServo;

int entryCount = 0;
const int maxCapacity = 2;

bool entryDetected = false;
bool exitDetected = false;

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  myServo.write(20); 

  pinMode(trigEntry, OUTPUT);
  pinMode(echoEntry, INPUT);
  pinMode(trigExit, OUTPUT);
  pinMode(echoExit, INPUT);
}

long readDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH); 
  if (duration == 0) return 999; 
  long distance = duration * 0.034 / 2;
  return distance;
}



void loop() {
  long entryDist = readDistance(trigEntry, echoEntry);
  long exitDist = readDistance(trigExit, echoExit);

  Serial.print("Entry Distance: ");
  Serial.print(entryDist);
  Serial.print(" cm, Exit Distance: ");
  Serial.print(exitDist);
  Serial.println(" cm, Current Count: ");
  Serial.print(entryCount);

  // -------- Entry Logic --------
  if (entryDist < 8 && !entryDetected && entryCount < maxCapacity) {
    entryDetected = true;
    myServo.write(120);  // Open gate
    delay(3000);
    myServo.write(20);   // Close gate
    entryCount++;
    Serial.println("Vehicle Entered.");
  }

  if (entryDist > 10) {
    entryDetected = false; // Reset for next vehicle
  }

  // -------- Exit Logic --------
  if (exitDist < 12 && !exitDetected && entryCount > 0) {
    exitDetected = true;
    myServo.write(120);  // Open gate
    delay(3000);
    myServo.write(20);   // Close gate
    entryCount--;
    Serial.println("Vehicle Exited.");
  }

  if (exitDist > 10) {
    exitDetected = false; // Reset for next vehicle
  }

  delay(300);
}