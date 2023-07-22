#include <Keyboard.h>

int buttonPin = 9;  // Set a button to any pin
int previousButtonState = HIGH;

void setup()
{
  pinMode(buttonPin, INPUT);  // Set the button as an input
  digitalWrite(buttonPin, HIGH);  // Pull the button high
}

void loop()
{
  int buttonState = digitalRead(buttonPin);
  if ((buttonState != previousButtonState)  
    && (buttonState == LOW)) {
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(' ');
      delay(1000); 
    }
  else if ((buttonState != previousButtonState)  
    && (buttonState == HIGH)) {
      Keyboard.releaseAll();
      delay(1000);
    }
  previousButtonState = buttonState;
}
