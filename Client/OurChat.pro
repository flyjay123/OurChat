QT       += core gui network
DEFINES += RC_ICONS
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfriend.cpp \
    chatwindow.cpp \
    friendiconlabel.cpp \
    friendinfowidget.cpp \
    frienditem.cpp \
    logging.cpp \
    main.cpp \
    client.cpp \
    selfinfowidget.cpp \
    sendtextedit.cpp \
    stringtool.cpp \
    tcpclient.cpp

HEADERS += \
    addfriend.h \
    chatwindow.h \
    client.h \
    friendiconlabel.h \
    friendinfowidget.h \
    frienditem.h \
    logging.h \
    selfinfowidget.h \
    sendtextedit.h \
    stringtool.h \
    tcpclient.h

FORMS += \
    addfriend.ui \
    chatwindow.ui \
    client.ui \
    friendinfowidget.ui \
    frienditem.ui \
    logging.ui \
    selfinfowidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc

DISTFILES += \
    wechat.ico icon.ico

RC_FILE = icon.rc
