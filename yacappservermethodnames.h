#ifndef YACAPPSERVERMETHODNAMES_H
#define YACAPPSERVERMETHODNAMES_H

#include "jw78Macros.h"

class YACAPPServerMethodNames
{
public:
    MACRO_ConstString(updateDeviceToken);
    MACRO_ConstString(updateAppUserProfile);
    MACRO_ConstString(requestVerifyToken);
    MACRO_ConstString(registerAppUser);
    MACRO_ConstString(fetchProfile);
    MACRO_ConstString(fetchMyProfile);
    YACAPPServerMethodNames();
};

#endif // YACAPPSERVERMETHODNAMES_H
