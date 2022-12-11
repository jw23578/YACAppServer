#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <chrono>
#include "sole/sole.hpp"

#define MACRO_NameValue(variable) #variable, variable

const std::chrono::system_clock::time_point TimePointPostgreSqlNull = std::chrono::system_clock::time_point::min();
const std::chrono::system_clock::time_point TimePointPostgreSqlNow = std::chrono::system_clock::time_point::min() + std::chrono::microseconds(1);

const sole::uuid NullUuid = sole::rebuild("00000000-0000-0000-0000-000000000000");

#endif // DEFINITIONS_H
