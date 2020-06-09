
#include <FastLED.h>
#define NUM_LEDS 44
#define DATA_PIN 5
#define COLOR_ORDER GRB
#define FRAMES_PER_SECOND  120
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


CRGB leds[NUM_LEDS];


CRGB::HTMLColorCode staticRainbowColours[6] = { CRGB::Violet, CRGB::DeepSkyBlue,  CRGB::Green, CRGB::Yellow, CRGB::Orange, CRGB::Red };

const int band = 5;
const int gap = 3;
int pixel;
int bcolour;
int brightness = 50;




const int buttonPin = 4;
int buttonState = 0;


// Rotary Encoder Inputs
const int inputCLK = 3;
const int inputDT = 2;
int scene;
int colour;
int count;
volatile boolean fired;
volatile boolean up;

String encdir = "";

void isr ()  {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (digitalRead (inputDT))
    up = digitalRead (inputCLK);
  else
    up = !digitalRead (inputCLK);
  fired = true;
}  // end of isr


void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setDither( 0 );

  // Set encoder pins as inputs

  pinMode (inputCLK, INPUT);
  pinMode (inputDT, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite (inputCLK, HIGH);     // enable pull-ups
  digitalWrite (inputDT, HIGH);


  // Setup Serial Monitor
  Serial.begin (9600);

  // Read the initial state of inputCLK
  // Assign to previousStateCLK variable
  attachInterrupt(digitalPinToInterrupt(inputDT), isr, CHANGE);
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { staticRainbow, bpm, juggle,rainbow, sinelon, white };
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop() {
  gPatterns[gCurrentPatternNumber]();

  int sensorVal = digitalRead(buttonPin);
  if (digitalRead(buttonPin) == LOW) {
    Serial.print("NEXT ");
    nextPattern();
    delay(500);

  }

  if (fired)
  {
    if (up)
      brightness-=10;
    else
      brightness+=10;
    fired = false;
  }
  brightness = min(255, max(0, brightness));
  Serial.print("Brightness: ");
  Serial.println(brightness);


  FastLED.setBrightness(brightness);
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;
  }

}


void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void white()
{
  fill_solid( leds, NUM_LEDS, CRGB::Violet);
}

void staticRainbow()
{
  pixel = 0;
  for ( bcolour = 0 ; bcolour <= 6 ; bcolour++) {
    for ( count = 0 ; count <= band  ; count ++) {

      leds[pixel] =  staticRainbowColours[bcolour];

      pixel += 1;
    }
  }
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
