QT       += core gui
QT       += qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


SOURCES += \
    imagegenerator.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    imagegenerator.h \
    widget.h

FORMS += \
    widget.ui

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    keys \
    черновик
