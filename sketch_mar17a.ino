#include <SoftwareSerial.h>

SoftwareSerial BT(2, 3); // RX, TX

// Ultrasonic
const int trigPin = 9;
const int echoPin = 10;

// Buzzer & Button
const int buzzer = 5;
const int buttonPin = 4;

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);
  int distance = duration * 0.034 / 2;

  return distance;
}

void loop() {

  // 🔘 SOS Button
  if (digitalRead(buttonPin) == LOW) {
    BT.println("SOS");   // send to phone

    // SOS buzzer pattern
    for (int i = 0; i < 3; i++) {
      tone(buzzer, 1000);
      delay(200);
      noTone(buzzer);
      delay(200);
    }

    delay(1000); // prevent spam
    return;
  }

  // 📏 Distance detection
  distance = getDistance();

  Serial.print("Distance: ");
  Serial.println(distance);

  // 👓 Obstacle alert (<70 cm)
  if (distance > 0 && distance < 70) {
    int beepDelay = map(distance, 5, 70, 50, 300);

    tone(buzzer, 1000);
    delay(50);
    noTone(buzzer);
    delay(beepDelay);
  } else {
    noTone(buzzer);
  }

  delay(50);
}