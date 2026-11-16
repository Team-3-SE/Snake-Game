#pragma once
#include <cstdlib>
extern "C" int stub_system(const char*);
namespace std { using ::stub_system; }
#define system stub_system
