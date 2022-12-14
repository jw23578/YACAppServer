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
LIBS += -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

INCLUDEPATH += utils
INCLUDEPATH += postgres
INCLUDEPATH += curlWrapper
INCLUDEPATH += interfaces
INCLUDEPATH += databaselogic
INCLUDEPATH += rapidjson/include
INCLUDEPATH += /usr/include/opencv4

SOURCES += \
        curlWrapper/jw78curlwrapper.cpp \
        curlWrapper/jw78emailwrapper.cpp \
        curlWrapper/jw78imapwrapper.cpp \
        curlWrapper/jw78pop3wrapper.cpp \
        curlWrapper/jw78smtpwrapper.cpp \
        databaselogic/databaselogicappuser.cpp \
        databaselogic/databaselogicimagetable.cpp \
        databaselogic/databaselogicmessages.cpp \
        databaselogic/databaselogics.cpp \
        databaselogic/databaselogictables.cpp \
        databaselogic/databaselogicuserandapp.cpp \
        databaselogic/databaselogicworktime.cpp \
        databaselogic/tablefields.cpp \
        databaselogic/tablenames.cpp \
        emaillogic.cpp \
        handler/handlerappuserloggedin.cpp \
        handler/handlerappuserlogin.cpp \
        handler/handlerappuserregister.cpp \
        handler/handlerappuserrequestupdatepassword.cpp \
        handler/handlerappuserupdatepassword.cpp \
        handler/handlerappuserverify.cpp \
        handler/handlergetallapps.cpp \
        handler/handlergetapp.cpp \
        handler/handleruserloggedin.cpp \
        handler/handleruserlogin.cpp \
        handler/handleruserregister.cpp \
        handler/handlerverifyuser.cpp \
        handlerLoggedIn/handlerappuserfetchimage.cpp \
        handlerLoggedIn/handlerappuserfetchmessageupdates.cpp \
        handlerLoggedIn/handlerappuserfetchprofile.cpp \
        handlerLoggedIn/handlerappusergetworktimestate.cpp \
        handlerLoggedIn/handlerappuserinsertworktime.cpp \
        handlerLoggedIn/handlerappusersearchprofiles.cpp \
        handlerLoggedIn/handlerappuserupdatemessagestates.cpp \
        handlerLoggedIn/handlerappuserupdateprofile.cpp \
        handlerLoggedIn/handlerloggedininterface.cpp \
        handlerLoggedIn/handlerstoremessage.cpp \
        handlerLoggedIn/handleruploadapp.cpp \
        interfaces/extpistache.cpp \
        interfaces/pistachehandlerinterface.cpp \
        interfaces/pistacheserverinterface.cpp \
        loggedincontainer/loggedinappuserscontainer.cpp \
        loggedincontainer/loggedincontainerinterface.cpp \
        loggedincontainer/loggedinuserscontainer.cpp \
        logstat/filelogger.cpp \
        logstat/loggerstatterinterface.cpp \
        logstat/logstatcontroller.cpp \
        opencvwrapper/opencvwrapper.cpp \
        postgres/pgcolumnandtype.cpp \
        postgres/pgcommandtransactor.cpp \
        postgres/pgconnection.cpp \
        postgres/pgexecutor.cpp \
        postgres/pgoidloader.cpp \
        postgres/pgoidstorer.cpp \
        postgres/pgsqlstring.cpp \
        postgres/pgutils.cpp \
        serverHeader/appidheader.cpp \
        serverHeader/loginemailheader.cpp \
        serverHeader/logintokenheader.cpp \
        sole/sole.cpp \
        tests/testdatabaselogicappuser.cpp \
        tests/testdatabaselogicmessages.cpp \
        tests/testdatabaselogicworktime.cpp \
        tests/testextstring.cpp \
        tests/testtimepoint.cpp \
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
  databaselogic/databaselogicimagetable.h \
  databaselogic/databaselogicmessages.h \
  databaselogic/databaselogics.h \
  databaselogic/databaselogictables.h \
  databaselogic/databaselogicuserandapp.h \
  databaselogic/databaselogicworktime.h \
  databaselogic/tablefields.h \
  databaselogic/tablenames.h \
  emaillogic.h \
  handler/handlerappuserloggedin.h \
  handler/handlerappuserlogin.h \
  handler/handlerappuserregister.h \
  handler/handlerappuserrequestupdatepassword.h \
  handler/handlerappuserupdatepassword.h \
  handler/handlerappuserverify.h \
  handler/handlergetallapps.h \
  handler/handlergetapp.h \
  handler/handleruserloggedin.h \
  handler/handleruserlogin.h \
  handler/handleruserregister.h \
  handler/handlerverifyuser.h \
  handlerLoggedIn/handlerappuserfetchimage.h \
  handlerLoggedIn/handlerappuserfetchmessageupdates.h \
  handlerLoggedIn/handlerappuserfetchprofile.h \
  handlerLoggedIn/handlerappusergetworktimestate.h \
  handlerLoggedIn/handlerappuserinsertworktime.h \
  handlerLoggedIn/handlerappusersearchprofiles.h \
  handlerLoggedIn/handlerappuserupdatemessagestates.h \
  handlerLoggedIn/handlerappuserupdateprofile.h \
  handlerLoggedIn/handlerloggedininterface.h \
  handlerLoggedIn/handlerstoremessage.h \
  handlerLoggedIn/handleruploadapp.h \
  interfaces/extpistache.h \
  interfaces/pistachehandlerinterface.h \
  interfaces/pistacheserverinterface.h \
  loggedincontainer/loggedinappuserscontainer.h \
  loggedincontainer/loggedincontainerinterface.h \
  loggedincontainer/loggedinuserscontainer.h \
  logstat/filelogger.h \
  logstat/loggerstatterinterface.h \
  logstat/logstatcontroller.h \
  opencvwrapper/opencvwrapper.h \
  postgres/pgcolumnandtype.h \
  postgres/pgcommandtransactor.h \
  postgres/pgconnection.h \
  postgres/pgconnectionpool.h \
  postgres/pgexecutor.h \
  postgres/pgoidloader.h \
  postgres/pgoidstorer.h \
  postgres/pgsqlstring.h \
  postgres/pgutils.h \
  serverHeader/appidheader.h \
  serverHeader/loginemailheader.h \
  serverHeader/logintokenheader.h \
  sole/sole.hpp \
  tests/testdatabaselogicappuser.h \
  tests/testdatabaselogicmessages.h \
  tests/testdatabaselogicworktime.h \
  tests/testextstring.h \
  tests/testtimepoint.h \
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
  intern_doc.txt \
  updateProfile.json

contains(DEFINES,WITH_INFLUX){
SOURCES += logstat/influxlogger.cpp
HEADERS += logstat/influxlogger.h
}
