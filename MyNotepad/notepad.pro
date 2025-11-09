CONFIG += sdk_no_version_check

TEMPLATE = app
TARGET = mynotepad

QT += widgets printsupport

requires(qtConfig(fontdialog))

SOURCES += \
    main.cpp \
    mynotepad.cpp \
    ../FILE_MANAGER/src/encryption.c

# Header files
HEADERS += \
    mynotepad.h \
    ../FILE_MANAGER/include/encryption.h

FORMS += \
    mynotepad.ui

RESOURCES += \
    mynotepad.qrc

INCLUDEPATH += ../FILE_MANAGER/include

OPENSSL_PATH = $$system(brew --prefix openssl@3)
INCLUDEPATH += $$OPENSSL_PATH/include
LIBS += -L$$OPENSSL_PATH/lib -lcrypto

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.13

target.path = $$[QT_INSTALL_EXAMPLES]/widgets/tutorials/notepad
INSTALLS += target
