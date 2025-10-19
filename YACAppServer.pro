TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += YACAPPServer
DEFINES += ORMCPPTypes
DEFINES += WITH_RAPIDJSON

include("JWUtils/JWUtils.pri")
include("JWLogStat/JWLogStat.pri")

DEFINES += ONLY_INSERT_DB_CREATED_BY_COLUMN=created_by_t0002_id
DEFINES += ONLY_INSERT_DB_DELETE_BY_COLUMN=deleted_by_t0002_id
include("JWORM/JWORM.pri")
DEFINES += WITH_JWORM
include("JWPostgresLib/JWPostgresLib.pri")

include("yacAppAndServer/yacAppAndServer.pri")

LIBS += \
    -lpistache \
    -L/usr/local/lib \
    -lpqxx -lpq \
    -lcurl -lpthread
LIBS += -lopencv_core -lopencv_imgcodecs -lopencv_highgui -lopencv_imgproc

INCLUDEPATH += utils
INCLUDEPATH += postgres
INCLUDEPATH += curlWrapper
INCLUDEPATH += interfacesJWSpecula
INCLUDEPATH += databaselogic
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
        databaselogic/databaselogicappuser.cpp \
        databaselogic/databaselogicelement.cpp \
        databaselogic/databaselogicimagetable.cpp \
        databaselogic/databaselogicinterface.cpp \
        databaselogic/databaselogicmessages.cpp \
        databaselogic/databaselogicrightgroup.cpp \
        databaselogic/databaselogics.cpp \
        databaselogic/databaselogicspaces.cpp \
        databaselogic/databaselogictables.cpp \
        databaselogic/databaselogicthirdparty.cpp \
        databaselogic/databaselogicworktime.cpp \
        emaillogic.cpp \
        handler/handlerapp.cpp \
        handler/handlerpages.cpp \
        handler/handleruser.cpp \
        handlerLoggedIn/handler_t0030_documents.cpp \
        handlerLoggedIn/handlerappuserfetchimage.cpp \
        handlerLoggedIn/handlerappuserfetchmessageupdates.cpp \
        handlerLoggedIn/handlerappuserfetchprofile.cpp \
        handlerLoggedIn/handlerappusergetworktimestate.cpp \
        handlerLoggedIn/handlerappuserinsertworktime.cpp \
        handlerLoggedIn/handlerappuserrightgroup.cpp \
        handlerLoggedIn/handlerappusersearchprofiles.cpp \
        handlerLoggedIn/handlerappuserspace.cpp \
        handlerLoggedIn/handlerappuserupdatemessagestates.cpp \
        handlerLoggedIn/handlerappuserupdateprofile.cpp \
        handlerLoggedIn/handlerloggedininterface.cpp \
        handlerLoggedIn/handlerormobjects.cpp \
        handlerLoggedIn/handlerstoremessage.cpp \
        handlerLoggedIn/handleruploadapp.cpp \
        interfaces/extpistache.cpp \
        interfaces/pistachehandlerinterface.cpp \
        interfaces/pistacheserverinterface.cpp \
        loggedincontainer/loggedinappuserscontainer.cpp \
        loggedincontainer/loggedincontainerinterface.cpp \
        opencvwrapper/opencvwrapper.cpp \
        orm-mapper/orm2postgres.cpp \
        orm-mapper/orm2rapidjson.cpp \
        postgres/pgormpersistence.cpp \
        rightsLogic/rightslogic.cpp \
        serverHeader/appidheader.cpp \
        serverHeader/loginemailheader.cpp \
        serverHeader/logintokenheader.cpp \
        serverHeader/mandantheader.cpp \
        serverHeader/thirdheader.cpp \
        tests/testdatabaselogicappuser.cpp \
        tests/testdatabaselogicimagetable.cpp \
        tests/testdatabaselogicmessages.cpp \
        tests/testdatabaselogics.cpp \
        tests/testextstring.cpp \
        tests/testorm2postgres.cpp \
        tests/testtimepoint.cpp \
        tests/testviacurl.cpp \
        thirdparties/thirdcurlrequests.cpp \
        utils/extmap.cpp \
        main.cpp \
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
  databaselogic/databaselogicappuser.h \
  databaselogic/databaselogicelement.h \
  databaselogic/databaselogicimagetable.h \
  databaselogic/databaselogicinterface.h \
  databaselogic/databaselogicmessages.h \
  databaselogic/databaselogicrightgroup.h \
  databaselogic/databaselogics.h \
  databaselogic/databaselogicspaces.h \
  databaselogic/databaselogictables.h \
  databaselogic/databaselogicthirdparty.h \
  databaselogic/databaselogicworktime.h \
  emaillogic.h \
  handler/handlerapp.h \
  handler/handlerpages.h \
  handler/handleruser.h \
  handlerLoggedIn/handler_t0030_documents.h \
  handlerLoggedIn/handlerappuserfetchimage.h \
  handlerLoggedIn/handlerappuserfetchmessageupdates.h \
  handlerLoggedIn/handlerappuserfetchprofile.h \
  handlerLoggedIn/handlerappusergetworktimestate.h \
  handlerLoggedIn/handlerappuserinsertworktime.h \
  handlerLoggedIn/handlerappuserrightgroup.h \
  handlerLoggedIn/handlerappusersearchprofiles.h \
  handlerLoggedIn/handlerappuserspace.h \
  handlerLoggedIn/handlerappuserupdatemessagestates.h \
  handlerLoggedIn/handlerappuserupdateprofile.h \
  handlerLoggedIn/handlerloggedininterface.h \
  handlerLoggedIn/handlerormobjects.h \
  handlerLoggedIn/handlerstoremessage.h \
  handlerLoggedIn/handleruploadapp.h \
  interfaces/extpistache.h \
  interfaces/pistachehandlerinterface.h \
  interfaces/pistacheserverinterface.h \
  loggedincontainer/loggedinappuserscontainer.h \
  loggedincontainer/loggedincontainerinterface.h \
  opencvwrapper/opencvwrapper.h \
  orm-mapper/orm2postgres.h \
  orm-mapper/orm2rapidjson.h \
  postgres/pgormpersistence.h \
  rightsLogic/rightslogic.h \
  serverHeader/appidheader.h \
  serverHeader/loginemailheader.h \
  serverHeader/logintokenheader.h \
  serverHeader/mandantheader.h \
  serverHeader/thirdheader.h \
  tests/testdatabaselogicappuser.h \
  tests/testdatabaselogicimagetable.h \
  tests/testdatabaselogicmessages.h \
  tests/testdatabaselogics.h \
  tests/testextstring.h \
  tests/testorm2postgres.h \
  tests/testtimepoint.h \
  tests/testviacurl.h \
  thirdparties/thirdcurlrequests.h \
  utils/extmap.h \
  utils/jw78Macros.h \
  utils/threadsafequeue.h \
  yacappserver.h

DISTFILES += \
  .gitignore \
  YACAppServerConfig.json \
  curl-format.txt \
  deployToJW78.sh \
  go_psql_local.sh \
  intern_doc.txt \
  t0004_user_logintoken.json \
  updateProfile.json
