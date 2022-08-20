QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RBTree
TEMPLATE = app


SOURCES += main.cpp\
    RBTree.cpp \
        mainpanel.cpp \
    renderarea.cpp

HEADERS  += mainpanel.h \
    RBTree.h \
    renderarea.h

FORMS    +=
