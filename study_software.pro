QT       += core gui sql multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog_tablesetting.cpp \
    dialog_timesetting.cpp \
    main.cpp \
    mainwindow.cpp \
    widget_audioplayer.cpp \
    widget_course.cpp \
    widget_coursefile.cpp \
    widget_coursemanager.cpp \
    widget_notice.cpp

HEADERS += \
    dialog_tablesetting.h \
    dialog_timesetting.h \
    mainwindow.h \
    widget_audioplayer.h \
    widget_course.h \
    widget_coursefile.h \
    widget_coursemanager.h \
    widget_notice.h

FORMS += \
    dialog_tablesetting.ui \
    dialog_timesetting.ui \
    mainwindow.ui \
    widget_audioplayer.ui \
    widget_course.ui \
    widget_coursefile.ui \
    widget_coursemanager.ui \
    widget_notice.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
