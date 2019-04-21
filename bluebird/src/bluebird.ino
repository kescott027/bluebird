/*
 * Project bluebird
 * Description:
 * Author:
 * Date:
 */

 #include "application.h"
 #include "neopixel.h"
 #include "Event.cpp"
 #include "Routine.cpp"
 #include <ArduinoJson.h>
 #include <HttpClient.h>
 // IMPORTANT: Set pixel COUNT, PIN and TYPE
 #define PIXEL_COUNT 8
 #define PIXEL_PIN D0
 #define PIXEL_TYPE WS2812B
 #define ARDUINOJSON_ENABLE_PROGMEM 0
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

HttpClient http;

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    { "Content-Type", "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;

http_response_t response;


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
  delay(1);
  if (tickCount >= 1000) {
    pollEvents();
  }

  if (tickCount == 500) {
    setColorAll(PIXEL_COUNT, OFF);
    pixel.show();
  }
  tickCount++;
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
  Serial.println("polling Events...");
  request.hostname = "bluebird-bluebird-api.herokuapp.com";

  request.port = 80;

  request.path = "/api/events/5ca78aa0c2f8b2001714758e";
  //request.path = "/api/routines/5ca78a6fc2f8b2001714758c";
  http.get(request, response, headers);
  // json
  DynamicJsonDocument doc(2048);

  deserializeJson(doc, response.body.c_str());

  Event event(doc);

  Serial.println(event._id);
  Serial.println(event.title);
  Serial.println(event.color);

  if (String(event.color) == "GREEN") {
    setColorAll(PIXEL_COUNT, GREEN);
  } else if (String(event.color) == "BLUE") {
      setColorAll(PIXEL_COUNT, BLUE);
  } else if (String(event.color) == "RED") {
      setColorAll(PIXEL_COUNT, RED);
  } else if (String(event.color) == "YELLOW") {
      setColorAll(PIXEL_COUNT, YELLOW);
  } else if (String(event.color) == "MAGENTA") {
      setColorAll(PIXEL_COUNT, MAGENTA);
  } else if (String(event.color) == "CYAN") {
      setColorAll(PIXEL_COUNT, CYAN);
  }
  pixel.show();
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

void setColorAll(int num_pixel, int color) {
    switch (color) {
  case RED :
    for (int i = 0; i < num_pixel; i++) {
        pixel.setPixelColor(i, 255, 0, 0);
    }
    break;
  case GREEN :
    for (int i = 0; i < num_pixel; i++) {
    pixel.setPixelColor(i, 0, 255, 0);
    };
    break;
  case BLUE :
  for (int i = 0; i < num_pixel; i++) {
    pixel.setPixelColor(i, 0, 0, 255);
  }
    break;
  case CYAN :
  for (int i = 0; i < num_pixel; i++) {
  pixel.setPixelColor(i, 0, 255, 255);
  }
  break;
  case YELLOW :
  for (int i = 0; i < num_pixel; i++) {
  pixel.setPixelColor(i, 255, 255, 0);
  }
  break;
  case MAGENTA :
  for (int i = 0; i < num_pixel; i++) {
  pixel.setPixelColor(i, 255, 0, 255);
  }
  break;
  case WHITE :
  for (int i = 0; i < num_pixel; i++) {
  pixel.setPixelColor(i, 255, 255, 255);
  }
  break;
  case OFF :
  for (int i = 0; i < num_pixel; i++) {
  pixel.setPixelColor(i, 0, 0, 0);
  }
  break;
  default : // white
    for (int i = 0; i < num_pixel; i++) {
    pixel.setPixelColor(i, 255, 255, 255);
    }
  }
}
