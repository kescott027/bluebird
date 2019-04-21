#define ARDUINOJSON_ENABLE_PROGMEM 0

#include <ArduinoJson.h>

class Routine {

public:

  const char* _id;

  const char* title;

  const char* active;

  const char* _v;

  Routine() {

  }

  Routine(DynamicJsonDocument doc) {
    _id = doc["_id"];
    active = doc["active"];
    title = doc["title"];
    _v = doc["__v"];
  }

};
