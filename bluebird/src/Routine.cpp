#include "Routine.h"

const char* get_r_active(DynamicJsonDocument doc) {
    return doc["active"];
}

const char* get_r_id(DynamicJsonDocument doc) {
    return doc["_id"];
}

const char* get_r_title(DynamicJsonDocument doc) {
    return doc["title"];
}

const char* get_r_v(DynamicJsonDocument doc) {
    return doc["__v"];
}