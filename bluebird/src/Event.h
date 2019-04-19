#define ARDUINOJSON_ENABLE_PROGMEM 0

#include <ArduinoJson.h>

const char* get_id(DynamicJsonDocument doc);

const char* get_title(DynamicJsonDocument doc);

const char* get_start_time(DynamicJsonDocument doc);

const char* get_end_time(DynamicJsonDocument doc);

const char* get_color(DynamicJsonDocument doc);

const char* get_is_start(DynamicJsonDocument doc);

const char* get_is_end(DynamicJsonDocument doc);

const char* get_parent_routine(DynamicJsonDocument doc);

const char* get_v(DynamicJsonDocument doc);