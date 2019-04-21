#define ARDUINOJSON_ENABLE_PROGMEM 0

#include <ArduinoJson.h>

class Event {

public:

  const char* _id;

  const char* title;

  const char* start_time;

  const char* end_time;

  const char* color;

  const char* is_start;

  const char* is_end;

  const char* parent_routine;

  const char* _v;

  Event() {

  }

  Event(DynamicJsonDocument doc) {
    _id = doc["_id"];
    title = doc["title"];
    start_time = doc["start_time"];
    end_time = doc["end_time"];
    color = doc["color"];
    is_start = doc["is_start"];
    is_end = doc["is_end"];
    parent_routine = doc["parent_routine"];
    _v = doc["__v"];
  }

};
