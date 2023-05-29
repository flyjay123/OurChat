QT       += core gui network openglwidgets
DEFINES += RC_ICONS
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

FORMS += \
    Adding/addfriend.ui \
    Adding/systemmessage.ui \
    Adding/verificationitem.ui \
    Chatting/chatwindow.ui \
    IconSetting/iconselect.ui \
    Rewriting/frienditem.ui \
    Tools/imagepreview.ui \
    client.ui \
    friendinfowidget.ui \
    logging.ui \
    selfinfowidget.ui

HEADERS += \
    Adding/addfriend.h \
    Adding/systemmessage.h \
    Adding/verificationitem.h \
    Chatting/chatlistwidget.h \
    Chatting/chatwindow.h \
    Chatting/emojiselector.h \
    IconSetting/iconselect.h \
    Rewriting/friendiconlabel.h \
    Rewriting/frienditem.h \
    Rewriting/sendtextedit.h \
    Rewriting/smoothscrolllistwidget.h \
    Tools/imagepreview.h \
    Tools/stringtool.h \
    Tools/tcpclient.h \
    client.h \
    friendinfowidget.h \
    logging.h \
    selfinfowidget.h

SOURCES += \
    Adding/addfriend.cpp \
    Adding/systemmessage.cpp \
    Adding/verificationitem.cpp \
    Chatting/chatlistwidget.cpp \
    Chatting/chatwindow.cpp \
    Chatting/emojiselector.cpp \
    IconSetting/iconselect.cpp \
    Rewriting/friendiconlabel.cpp \
    Rewriting/smoothscrolllistwidget.cpp \
    Tools/imagepreview.cpp \
    friendinfowidget.cpp \
    Rewriting/frienditem.cpp \
    Rewriting/sendtextedit.cpp \
    Tools/stringtool.cpp \
    Tools/tcpclient.cpp \
    client.cpp \
    logging.cpp \
    main.cpp \
    selfinfowidget.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    wechat.ico icon.ico
RESOURCES += \
    src.qrc

INCLUDEPATH += "$${PWD}/Adding" "$${PWD}/Chatting" "$${PWD}/IconSetting" "$${PWD}/Tools" "$${PWD}/Rewriting"

#LIBS += $$PWD/libs/libeay32.dll


