QT       += core gui
LIBS += -lOpenGL32


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets openglwidgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    environment.cpp \
    geometry.cpp \
    main.cpp \
    mainwindow.cpp \
    renderwidget.cpp \
    volume.cpp

HEADERS += \
    environment.h \
    geometry.h \
    mainwindow.h \
    renderwidget.h \
    volume.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shaders.qrc

DISTFILES += \
    cube-fs.glsl \
    cube-vs.glsl
