#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <chrono>

#define MACRO_NameValue(variable) #variable, variable

#define UNUSED(x) (void)(x)

typedef std::chrono::system_clock::time_point TimePoint;
const std::chrono::system_clock::time_point TimePointPostgreSqlNull = std::chrono::system_clock::time_point::min();
const std::chrono::system_clock::time_point TimePointPostgreSqlNow = std::chrono::system_clock::time_point::min() + std::chrono::microseconds(1);

#endif // DEFINITIONS_H
