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
INCLUDEPATH += databaselogic

SOURCES += \
        curlWrapper/jw78curlwrapper.cpp \
        curlWrapper/jw78emailwrapper.cpp \
        curlWrapper/jw78imapwrapper.cpp \
        curlWrapper/jw78pop3wrapper.cpp \
        curlWrapper/jw78smtpwrapper.cpp \
        databaselogic/databaselogicappuser.cpp \
        databaselogic/databaselogicmessages.cpp \
        databaselogic/databaselogictables.cpp \
        databaselogic/databaselogicuserandapp.cpp \
        databaselogic/tablenames.cpp \
        emaillogic.cpp \
        handler/handlerappuserloggedin.cpp \
        handler/handlerappuserlogin.cpp \
        handler/handlerappuserregister.cpp \
        handler/handlerappuserverify.cpp \
        handler/handlergetallapps.cpp \
        handler/handlergetapp.cpp \
        handler/handleruserloggedin.cpp \
        handler/handleruserlogin.cpp \
        handler/handleruserregister.cpp \
        handler/handlerverifyuser.cpp \
        handlerLoggedIn/handlerloggedininterface.cpp \
        handlerLoggedIn/handleruploadapp.cpp \
        interfaces/extpistache.cpp \
        interfaces/pistachehandlerinterface.cpp \
        interfaces/pistacheserverinterface.cpp \
        loggedinappuserscontainer.cpp \
        loggedinuserscontainer.cpp \
        logstat/filelogger.cpp \
        logstat/loggerstatterinterface.cpp \
        logstat/logstatcontroller.cpp \
        postgres/pgcommandtransactor.cpp \
        postgres/pgconnection.cpp \
        postgres/pgexecutor.cpp \
        postgres/pgoidloader.cpp \
        postgres/pgoidstorer.cpp \
        postgres/pgsqlstring.cpp \
        postgres/pgutils.cpp \
        serverHeader/loginemailheader.cpp \
        serverHeader/logintokenheader.cpp \
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
  databaselogic/databaselogicappuser.h \
  databaselogic/databaselogicmessages.h \
  databaselogic/databaselogictables.h \
  databaselogic/databaselogicuserandapp.h \
  databaselogic/tablenames.h \
  emaillogic.h \
  handler/handlerappuserloggedin.h \
  handler/handlerappuserlogin.h \
  handler/handlerappuserregister.h \
  handler/handlerappuserverify.h \
  handler/handlergetallapps.h \
  handler/handlergetapp.h \
  handler/handleruserloggedin.h \
  handler/handleruserlogin.h \
  handler/handleruserregister.h \
  handler/handlerverifyuser.h \
  handlerLoggedIn/handlerloggedininterface.h \
  handlerLoggedIn/handleruploadapp.h \
  interfaces/extpistache.h \
  interfaces/pistachehandlerinterface.h \
  interfaces/pistacheserverinterface.h \
  loggedinappuserscontainer.h \
  loggedinuserscontainer.h \
  logstat/filelogger.h \
  logstat/loggerstatterinterface.h \
  logstat/logstatcontroller.h \
  postgres/pgcommandtransactor.h \
  postgres/pgconnection.h \
  postgres/pgconnectionpool.h \
  postgres/pgexecutor.h \
  postgres/pgoidloader.h \
  postgres/pgoidstorer.h \
  postgres/pgsqlstring.h \
  postgres/pgutils.h \
  serverHeader/loginemailheader.h \
  serverHeader/logintokenheader.h \
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
  .gitignore \
  YACAppServerConfig.json \
  curl-format.txt \
  deployToJW78.sh \
  intern_doc.txt

contains(DEFINES,WITH_INFLUX){
SOURCES += logstat/influxlogger.cpp
HEADERS += logstat/influxlogger.h
}
