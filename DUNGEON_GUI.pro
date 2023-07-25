QT       += core gui \
            multimedia \
            widgets \
            multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Bomber.cpp \
    Creature.cpp \
    Food.cpp \
    Hero.cpp \
    Item.cpp \
    Position.cpp \
    Scarfy.cpp \
    Trigger.cpp \
    customwindow.cpp \
    gamewindow.cpp \
    main.cpp \
    menuwindow.cpp \
    resultwindow.cpp \
    square.cpp \
    tutorialwindow.cpp

HEADERS += \
    Bomber.h \
    Creature.h \
    Food.h \
    Hero.h \
    Item.h \
    Main.h \
    Position.h \
    Scarfy.h \
    Trigger.h \
    customwindow.h \
    gamewindow.h \
    menuwindow.h \
    resultwindow.h \
    square.h \
    tutorialwindow.h

FORMS += \
    customwindow.ui \
    gamewindow.ui \
    menuwindow.ui \
    resultwindow.ui \
    tutorialwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
