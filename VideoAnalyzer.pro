QT += quick multimedia core gui

CONFIG += c++17

SOURCES += \
    main.cpp \
    videocontroller.cpp

HEADERS += \
    videocontroller.h \
    ffmpeg_wrapper.h

RESOURCES += qml.qrc

# 添加包含路径
INCLUDEPATH += $$PWD/include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target 