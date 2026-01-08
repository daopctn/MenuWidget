QT += core gui widgets

CONFIG += c++11

TARGET = MenuWidget
TEMPLATE = app

SOURCES += \
    main.cpp \
    src/MainWindow.cpp \
    src/widgets/MainWidget.cpp \
    src/widgets/CustomWidget.cpp \
    src/widgets/MenuWidget.cpp \
    src/widgets/Container.cpp

HEADERS += \
    src/MainWindow.h \
    src/widgets/MainWidget.h \
    src/widgets/CustomWidget.h \
    src/widgets/MenuWidget.h \
    src/widgets/Container.h

FORMS += \
    src/ui/MainWidget.ui \
    src/ui/MenuWidget.ui

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
