TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += \
    -lpistache \
    -L /usr/local/lib \
    -lpqxx -lpq

SOURCES += \
        handler/handlergetapp.cpp \
        handler/handlerlogin.cpp \
        handler/handlerregister.cpp \
        handler/handleruploadapp.cpp \
        main.cpp \
        pistachehandlerinterface.cpp \
        yacappserver.cpp

HEADERS += \
  handler/handlergetapp.h \
  handler/handlerlogin.h \
  handler/handlerregister.h \
  handler/handleruploadapp.h \
  pistachehandlerinterface.h \
  yacappserver.h
