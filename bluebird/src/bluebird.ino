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

struct BluebirdEvent {
    unsigned long _id;
    int version;
    char title[48];
    uint32_t start_time;
    uint32_t end_time;
    char color[16];
};

BluebirdEvent new_event = {0, 0, char(0), 0, 0, char(0)};

BluebirdEvent next_event = {0, 0, char(0), 0, 0, char(0)};

HttpClient http;

// local timer
unsigned long sys_time;

// local event array
Event *event_list = new Event[3];

// Headers currently need to be set at init, useful for API keys etc.
http_header_t headers[] = {
    { "Content-Type", "application/json" },
    { "Accept" , "*/*"},
    { NULL, NULL } // NOTE: Always terminate headers will NULL
};

http_request_t request;

http_response_t response;

uint32_t local_device_time;

uint32_t next_event_time;

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
  // check on startup?
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  int mom_state = INACTIVE;
  mom_state = digitalRead(mom_pin);
  if (mom_state != last_button_state) {
    if (mom_state == ACTIVE) {
    // change_led_state();
    ackEvent();
    }
    last_button_state = mom_state;
  }
  // delay(1);
  if (tickCount >= 10000) {
    pollEvents();
    checkforevents();
    // sys_time = millis();
    // Serial.println(sys_time);
    for (int k = 0; k < 3; k++) {
      Serial.println(event_list[k]._id);
      Serial.println(event_list[k].title);
      Serial.println(event_list[k].color);
      Serial.println(event_list[k].start_time);
      Serial.println(event_list[k].end_time);
      Serial.println(event_list[k].is_start);
      Serial.println(event_list[k].is_end);
      Serial.println(event_list[k].parent_routine);
      Serial.println(event_list[k]._v);
    }
  }

  /* if (tickCount == 500) {
    setColorAll(PIXEL_COUNT, OFF);
    pixel.show();
  } */
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
  // Serial.println("polling Events...");
  request.hostname = "bluebird-bluebird-api.herokuapp.com";
  Serial.println(request.hostname);

  request.port = 80;

  request.path = "/api/events?routine=5cb690a05b4d700017bede09";
  //request.path = "/api/routines/5ca78a6fc2f8b2001714758c";
  http.get(request, response, headers);
  // json
  // DynamicJsonDocument doc(8192);

  // deserializeJson(doc, response.body.c_str());

  // Event event;

  // event_list[0] = event;

  // Serial.println(event._id);
  // Serial.println(event.title);
  // Serial.println(event.color);
  // Serial.println(response.body.c_str());

  Serial.println(response.body);
  // parsing events json
  String json_result = response.body.c_str();
  json_result = json_result.substring(1, json_result.length());
  char result[json_result.length() + 1];
  strcpy(result, json_result);
  char* temp = strtok(result, "}");
  int i = 0;
  while (temp) {
    Serial.println("Parsed event:");
    Serial.println(i);
    String str_temp(temp);
    if (str_temp != "]") {
      str_temp.concat("}");
      if (str_temp.charAt(0) == ',') {
        str_temp = str_temp.substring(1, str_temp.length());
      }
      Serial.println(str_temp);
      DynamicJsonDocument *doc = new DynamicJsonDocument(2048);
      deserializeJson(*doc, str_temp.c_str());
      Event *event = new Event(*doc);
      event_list[i] = *event;
      // Serial.println(event_list[i]._id);
      // Serial.println(event_list[i].title);
      // Serial.println(event_list[i].color);
      // Serial.println(event_list[i].start_time);
      // Serial.println(event_list[i].end_time);
      // Serial.println(event_list[i].is_start);
      // Serial.println(event_list[i].is_end);
      // Serial.println(event_list[i].parent_routine);
      // Serial.println(event_list[i]._v);
      i++;
      delete doc;
      delete event;
    }
    temp = strtok(NULL, "}");
  }

  // Event event = event_list[0];
  // Serial.println(event._id);
  // Serial.println(event.title);
  // Serial.println(event.color);
  //
  // if (String(event.color) == "GREEN") {
  //   setColorAll(PIXEL_COUNT, GREEN);
  // } else if (String(event.color) == "BLUE") {
  //     setColorAll(PIXEL_COUNT, BLUE);
  // } else if (String(event.color) == "RED") {
  //     setColorAll(PIXEL_COUNT, RED);
  // } else if (String(event.color) == "YELLOW") {
  //     setColorAll(PIXEL_COUNT, YELLOW);
  // } else if (String(event.color) == "MAGENTA") {
  //     setColorAll(PIXEL_COUNT, MAGENTA);
  // } else if (String(event.color) == "CYAN") {
  //     setColorAll(PIXEL_COUNT, CYAN);
  // }
  // pixel.show();

  // set next_event_time
  // set next_event_color

  tickCount = 0;

}

void checkforevents() {
  // loop through polled active events and determine if new events exist.
  // if so, put them into the event structure.
}


void store_local_event() {
/*    struct BluebirdEvent next_event = {
            long _id ;
            int version;
            char title[48];
            uint32_t start_time;
            uint32_t end_time;
            char color[16];
    }; */
    // create start time
    // create end time
    // set color
    // maybe something else
}

void ackEvent() {
  // make web request
  int i;
  for (i=0;i<=3;i++) {
  setColorAll(PIXEL_COUNT, CYAN);
  pixel.show();
  delay(220);
  setColorAll(PIXEL_COUNT, OFF);
  pixel.show();
  delay(80);
  }
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
