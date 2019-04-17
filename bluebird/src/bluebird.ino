/*
 * Project bluebird
 * Description:
 * Author:
 * Date:
 */

 #include "application.h"
 #include "neopixel.h"

 // IMPORTANT: Set pixel COUNT, PIN and TYPE
 #define PIXEL_COUNT 1
 #define PIXEL_PIN D0
 #define PIXEL_TYPE WS2812B
// setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w)

#define RED 0
#define GREEN 1
#define BLUE 2
#define YELLOW 3
#define MAGENTA 4
#define CYAN 5
#define WHITE 6
#define OFF 9
#define SMARTPIXEL 0
#define INACTIVE 0
#define ACTIVE 1
#define RESTING 0
#define ALERTING 1
#define ACKD 2


 Adafruit_NeoPixel pixel = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
volatile uint64_t tickCount = 0;
 // volatile int colorValue = 0;
 int mom_pin = D4;
 int last_button_state = INACTIVE;
 int alertStatus = RESTING;
 int colorValue = RED;
 int alertColor = RED;
 int ackColor = CYAN;
 int ledState = INACTIVE;

// setup() runs once, when the device is first turned on.
void setup() {
  pinMode(mom_pin, INPUT_PULLDOWN);
  pixel.begin();
  pixel.show();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  int mom_state = INACTIVE;
  mom_state = digitalRead(mom_pin);
  if (mom_state != last_button_state) {
    if (mom_state == ACTIVE) {
    change_led_state();
    }
    last_button_state = mom_state;
  }
  delay(100);
  if (tickCount >= 1000) {
    pollEvents();
  }
}

void change_led_state() {

  if (ledState == INACTIVE) {
    ledState = ACTIVE;
    setColor(SMARTPIXEL, colorValue);
    pixel.show();
  if (colorValue >= 6 ) {
    colorValue = 0;
    } else {
    colorValue ++;
    }
  } else {
    setColor(SMARTPIXEL, OFF);
    pixel.show();
    ledState = 0;
  }
}

void pollEvents() {
  //make web request
  // if a new event exists, go to ALERTING
  tickCount = 0;
}

void ackEvent() {
  // make web request
  alertStatus = RESTING;
}

void setColor(int target_pixel, int color) {
switch (color) {
  case RED :
    pixel.setPixelColor(target_pixel, 255, 0, 0);
    break;
  case GREEN :
    pixel.setPixelColor(target_pixel, 0, 255, 0);
    break;
  case BLUE :
    pixel.setPixelColor(target_pixel, 0, 0, 255);
    break;
  case CYAN :
  pixel.setPixelColor(target_pixel, 0, 255, 255);
  break;
  case YELLOW :
  pixel.setPixelColor(target_pixel, 255, 255, 0);
  break;
  case MAGENTA :
  pixel.setPixelColor(target_pixel, 255, 0, 255);
  break;
  case WHITE :
  pixel.setPixelColor(target_pixel, 255, 255, 255);
  break;
  case OFF :
  pixel.setPixelColor(target_pixel, 0, 0, 0);
  break;
  default : // white
    pixel.setPixelColor(target_pixel, 255, 255, 255);
  }
}
