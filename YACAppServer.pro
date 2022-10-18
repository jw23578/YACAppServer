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
        interfaces/pistachehandlerinterface.cpp \
        interfaces/pistacheserverinterface.cpp \
        main.cpp \
        yacappserver.cpp

HEADERS += \
  handler/handlergetapp.h \
  handler/handlerlogin.h \
  handler/handlerregister.h \
  handler/handleruploadapp.h \
  interfaces/pistachehandlerinterface.h \
  interfaces/pistacheserverinterface.h \
  yacappserver.h
