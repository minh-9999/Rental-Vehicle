#pragma once

#include "Vehicles.h"
#include "../third_party/json-src/single_include/nlohmann/json.hpp"

extern string jsondata;

void saveToJson(const unordered_map<string, Vehicles> &, const string &);

void loadFromJson(unordered_map<string, Vehicles> &, const string &);