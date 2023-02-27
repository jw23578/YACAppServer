TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

include("yacAppAndServer/yacAppAndServer.pri")

DEFINES += RAPIDJSON_HAS_STDSTRING
DEFINES += YACAPPServer

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
        caches/cacheinterface.cpp \
        caches/devicetokencache.cpp \
        curlWrapper/jw78curlwrapper.cpp \
        curlWrapper/jw78emailwrapper.cpp \
        curlWrapper/jw78firebasewrapper.cpp \
        curlWrapper/jw78imapwrapper.cpp \
        curlWrapper/jw78pop3wrapper.cpp \
        curlWrapper/jw78smtpwrapper.cpp \
        databaselogic/databaselogicappointments.cpp \
        databaselogic/databaselogicappuser.cpp \
        databaselogic/databaselogicelement.cpp \
        databaselogic/databaselogicimagetable.cpp \
        databaselogic/databaselogicmessages.cpp \
        databaselogic/databaselogicrightgroup.cpp \
        databaselogic/databaselogics.cpp \
        databaselogic/databaselogictables.cpp \
        databaselogic/databaselogicuserandapp.cpp \
        databaselogic/databaselogicworktime.cpp \
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
        handlerLoggedIn/handlerappuserappointments.cpp \
        handlerLoggedIn/handlerappuserfetchimage.cpp \
        handlerLoggedIn/handlerappuserfetchmessageupdates.cpp \
        handlerLoggedIn/handlerappuserfetchprofile.cpp \
        handlerLoggedIn/handlerappusergetworktimestate.cpp \
        handlerLoggedIn/handlerappuserinsertworktime.cpp \
        handlerLoggedIn/handlerappuserrightgroup.cpp \
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
        rightsLogic/rightslogic.cpp \
        serverHeader/appidheader.cpp \
        serverHeader/loginemailheader.cpp \
        serverHeader/logintokenheader.cpp \
        sole/sole.cpp \
        tests/testdatabaselogicappuser.cpp \
        tests/testdatabaselogicimagetable.cpp \
        tests/testdatabaselogicmessages.cpp \
        tests/testdatabaselogics.cpp \
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
  caches/cacheinterface.h \
  caches/devicetokencache.h \
  curlWrapper/jw78curlwrapper.h \
  curlWrapper/jw78emailwrapper.h \
  curlWrapper/jw78firebasewrapper.h \
  curlWrapper/jw78imapwrapper.h \
  curlWrapper/jw78pop3wrapper.h \
  curlWrapper/jw78smtpwrapper.h \
  databaselogic/databaselogicappointments.h \
  databaselogic/databaselogicappuser.h \
  databaselogic/databaselogicelement.h \
  databaselogic/databaselogicimagetable.h \
  databaselogic/databaselogicmessages.h \
  databaselogic/databaselogicrightgroup.h \
  databaselogic/databaselogics.h \
  databaselogic/databaselogictables.h \
  databaselogic/databaselogicuserandapp.h \
  databaselogic/databaselogicworktime.h \
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
  handlerLoggedIn/handlerappuserappointments.h \
  handlerLoggedIn/handlerappuserfetchimage.h \
  handlerLoggedIn/handlerappuserfetchmessageupdates.h \
  handlerLoggedIn/handlerappuserfetchprofile.h \
  handlerLoggedIn/handlerappusergetworktimestate.h \
  handlerLoggedIn/handlerappuserinsertworktime.h \
  handlerLoggedIn/handlerappuserrightgroup.h \
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
  rightsLogic/rightslogic.h \
  serverHeader/appidheader.h \
  serverHeader/loginemailheader.h \
  serverHeader/logintokenheader.h \
  sole/sole.hpp \
  tests/testdatabaselogicappuser.h \
  tests/testdatabaselogicimagetable.h \
  tests/testdatabaselogicmessages.h \
  tests/testdatabaselogics.h \
  tests/testdatabaselogicworktime.h \
  tests/testextstring.h \
  tests/testtimepoint.h \
  utils/base64.h \
  utils/definitions.h \
  utils/extmap.h \
  utils/extrapidjson.h \
  utils/extstring.h \
  utils/extvector.h \
  utils/jw78Macros.h \
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
