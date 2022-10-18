TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += RAPIDJSON_HAS_STDSTRING

LIBS += \
    -lpistache \
    -L /usr/local/lib \
    -lpqxx -lpq

INCLUDEPATH += ../utils
INCLUDEPATH += ../postgres

SOURCES += \
        handler/handlergetapp.cpp \
        handler/handlerlogin.cpp \
        handler/handlerregister.cpp \
        handler/handleruploadapp.cpp \
        interfaces/pistachehandlerinterface.cpp \
        interfaces/pistacheserverinterface.cpp \
        postgres/pgcommandtransactor.cpp \
        postgres/pgconnection.cpp \
        postgres/pgsqlstring.cpp \
        postgres/pgutils.cpp \
        sole/sole.cpp \
        utils/extrapidjson.cpp \
        utils/extstring.cpp \
        main.cpp \
        postgres/pgconnectionpool.cpp \
        yacappserver.cpp

HEADERS += \
  handler/handlergetapp.h \
  handler/handlerlogin.h \
  handler/handlerregister.h \
  handler/handleruploadapp.h \
  interfaces/pistachehandlerinterface.h \
  interfaces/pistacheserverinterface.h \
  postgres/pgcommandtransactor.h \
  postgres/pgconnection.h \
  postgres/pgconnectionpool.h \
  postgres/pgsqlstring.h \
  postgres/pgutils.h \
  sole/sole.hpp \
  utils/definitions.h \
  utils/extrapidjson.h \
  utils/extstring.h \
  yacappserver.h

DISTFILES += \
  YACAppServerConfig.json
