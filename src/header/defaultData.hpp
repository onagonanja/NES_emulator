#ifndef INCLUDED_DEFAULTDATA
#define INCLUDED_DEFAULTDATA

#include <map>
#include <string>
using namespace std;

map<string, int> defaultRegisters = {
    {"A", 0x00},        {"X", 0x00},     {"Y", 0x00},        {"SP", 0xff},        {"PC", 0x0000},  {"negative", false}, {"overflow", false},
    {"reserved", true}, {"break", true}, {"decimal", false}, {"interrupt", true}, {"zero", false}, {"carry", false},
};

#endif