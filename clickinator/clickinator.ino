
#include "Mouse.h"

int buttonPin = 9;
int leftPin = 8;
int rightPin = 7;

long randNumber;
int button = 1;

void setup()
{
  pinMode(buttonPin, INPUT);
  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);

  digitalWrite(buttonPin, HIGH);
  digitalWrite(leftPin, HIGH);
  digitalWrite(rightPin, HIGH);

  Mouse.begin();

  //Serial.begin(9600);
}

void loop()
{
  int buttonState = digitalRead(buttonPin);
  int leftState = digitalRead(leftPin);
  int rightState = digitalRead(rightPin);

  int speedValue = analogRead(A0);
  int speed = map(speedValue, 0, 1023, 1, 500);

  int randnessValue = analogRead(A1);
  int randnessMap = map(randnessValue, 0, 1023, 0, 100);
  int randness = random(1, randnessMap);

  if (leftState == LOW) {
    button = 1;
  } else if (rightState == LOW) {
    button = 2;
  } else {
    button = 4;
  }

  if (buttonState == LOW) {
    Mouse.click(button);
    //Serial.println(mouse);
    delay(speed + randness);
  }


  //DEBUG
  //Serial.println("speed: ");
  //Serial.println(speed);
  //Serial.println("randnessValue: ");
  //Serial.println(randnessValue);
  //Serial.print("Rand: ");
  //Serial.println(randness);
  //Serial.println(buttonState);
}
