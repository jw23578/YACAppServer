TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += \
    -lpistache \
    -L /usr/local/lib \
    -lpqxx -lpq

SOURCES += \
        handler/handlerregister.cpp \
        main.cpp \
        pistachehandlerinterface.cpp \
        yacappserver.cpp

HEADERS += \
  handler/handlerregister.h \
  pistachehandlerinterface.h \
  yacappserver.h
