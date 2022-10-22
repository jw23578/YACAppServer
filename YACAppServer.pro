TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += RAPIDJSON_HAS_STDSTRING

LIBS += \
    -lpistache \
    -L /usr/local/lib \
    -lpqxx -lpq \
    -lcurl -lpthread

INCLUDEPATH += utils
INCLUDEPATH += postgres
INCLUDEPATH += curlWrapper
INCLUDEPATH += interfaces

SOURCES += \
        curlWrapper/jw78curlwrapper.cpp \
        curlWrapper/jw78emailwrapper.cpp \
        curlWrapper/jw78imapwrapper.cpp \
        curlWrapper/jw78pop3wrapper.cpp \
        curlWrapper/jw78smtpwrapper.cpp \
        databaselogic.cpp \
        emaillogic.cpp \
        handler/handlergetapp.cpp \
        handler/handlerloginuser.cpp \
        handler/handlerregisteruser.cpp \
        handler/handleruploadapp.cpp \
        handler/handlerverifyuser.cpp \
        interfaces/extpistache.cpp \
        interfaces/pistachehandlerinterface.cpp \
        interfaces/pistacheserverinterface.cpp \
        loggedinuserscontainer.cpp \
        postgres/pgcommandtransactor.cpp \
        postgres/pgconnection.cpp \
        postgres/pgexecutor.cpp \
        postgres/pgsqlstring.cpp \
        postgres/pgutils.cpp \
        sole/sole.cpp \
        utils/base64.cpp \
        utils/extmap.cpp \
        utils/extrapidjson.cpp \
        utils/extstring.cpp \
        main.cpp \
        postgres/pgconnectionpool.cpp \
        utils/extvector.cpp \
        yacappserver.cpp

HEADERS += \
  curlWrapper/jw78curlwrapper.h \
  curlWrapper/jw78emailwrapper.h \
  curlWrapper/jw78imapwrapper.h \
  curlWrapper/jw78pop3wrapper.h \
  curlWrapper/jw78smtpwrapper.h \
  databaselogic.h \
  emaillogic.h \
  handler/handlergetapp.h \
  handler/handlerloginuser.h \
  handler/handlerregisteruser.h \
  handler/handleruploadapp.h \
  handler/handlerverifyuser.h \
  interfaces/extpistache.h \
  interfaces/pistachehandlerinterface.h \
  interfaces/pistacheserverinterface.h \
  loggedinuserscontainer.h \
  postgres/pgcommandtransactor.h \
  postgres/pgconnection.h \
  postgres/pgconnectionpool.h \
  postgres/pgexecutor.h \
  postgres/pgsqlstring.h \
  postgres/pgutils.h \
  sole/sole.hpp \
  utils/base64.h \
  utils/definitions.h \
  utils/extmap.h \
  utils/extrapidjson.h \
  utils/extstring.h \
  utils/extvector.h \
  utils/threadsafequeue.h \
  yacappserver.h

DISTFILES += \
  YACAppServerConfig.json \
  curl-format.txt \
  intern_doc.txt
