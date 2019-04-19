#include "Event.h"

const char* get_id(DynamicJsonDocument doc) {
    return doc["_id"];
}

const char* get_title(DynamicJsonDocument doc) {
    return doc["title"];
}

const char* get_start_time(DynamicJsonDocument doc) {
    return doc["start_time"];
}

const char* get_end_time(DynamicJsonDocument doc) {
    return doc["end_time"];
}

const char* get_color(DynamicJsonDocument doc) {
    return doc["color"];
}

const char* get_is_start(DynamicJsonDocument doc) {
    return doc["is_start"];
}

const char* get_is_end(DynamicJsonDocument doc) {
    return doc["is_end"];
}

const char* get_parent_routine(DynamicJsonDocument doc) {
    return doc["parent_routine"];
}

const char* get_v(DynamicJsonDocument doc) {
    return doc["__v"];
}