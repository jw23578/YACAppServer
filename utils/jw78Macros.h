#ifndef JW78MACROS_H
#define JW78MACROS_H

#include <string>

#define MACRO_ConstString(nameValue) \
    const std::string nameValue{#nameValue}

#endif // JW78MACROS_H
