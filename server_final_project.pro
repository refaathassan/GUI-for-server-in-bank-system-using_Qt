QT = core
QT += network
CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += "C:/Program Files/OpenSSL-Win64/include"

# Path to OpenSSL libraries directory
LIBS += -L"C:/Program Files/OpenSSL-Win64/lib/VC/x64/MD" -lssl -lcrypto

SOURCES += \
        adduserhandler.cpp \
        databasehandler.cpp \
        deletehandler.cpp \
        getaccounhandler.cpp \
        handler.cpp \
        loghandler.cpp \
        main.cpp \
        maketransactionhandler.cpp \
        server.cpp \
        serverhandler.cpp \
        transferamounthandler.cpp \
        updateuserhandler.cpp \
        viewaccountbalancehandler.cpp \
        viewdatabase.cpp \
        viewtransactionhistoryhandler.cpp \
        qaesencryption.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    adduserhandler.h \
    databasehandler.h \
    deletehandler.h \
    getaccounhandler.h \
    handler.h \
    loghandler.h \
    maketransactionhandler.h \
    server.h \
    serverhandler.h \
    transferamounthandler.h \
    updateuserhandler.h \
    viewaccountbalancehandler.h \
    viewdatabase.h \
    viewtransactionhistoryhandler.h \
    aesni/aesni-key-init.h \
    qaesencryption.h \
    aesni/aesni-key-exp.h \
    aesni/aesni-enc-ecb.h \
    aesni/aesni-enc-cbc.h
