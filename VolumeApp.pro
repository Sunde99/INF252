QT       += core gui
LIBS += -lOpenGL32


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets openglwidgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    crosssectionrenderer.cpp \
    environment.cpp \
    geometry.cpp \
    main.cpp \
    mainwindow.cpp \
    node.cpp \
    renderwidget.cpp \
    transferfunctionrenderer.cpp \
    transferfunctiontexturegenerator.cpp \
    transferfunctionwidget.cpp \
    volume.cpp

HEADERS += \
    crosssectionrenderer.h \
    environment.h \
    geometry.h \
    mainwindow.h \
    node.h \
    renderwidget.h \
    transferfunctionrenderer.h \
    transferfunctiontexturegenerator.h \
    transferfunctionwidget.h \
    volume.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    shaders.qrc

DISTFILES += \
    cube-fs.glsl \
    cube-vs.glsl \
    histogram-fs.glsl \
    histogram-vs.glsl \
    image-slice-fs.glsl \
    screen-vs.glsl \
    volume-cs.glsl
