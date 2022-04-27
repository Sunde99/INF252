#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>
#include "volume.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat f = QSurfaceFormat::defaultFormat();
    f.setMajorVersion(4);
    f.setMinorVersion(5);
    f.setRenderableType(QSurfaceFormat::OpenGL);
    f.setProfile(QSurfaceFormat::CompatibilityProfile);
    f.setOption(QSurfaceFormat::DeprecatedFunctions,true);
    QSurfaceFormat::setDefaultFormat(f);

    Environment *env = new Environment();
   env->volume()->load("../hand/hand.dat");
    MainWindow w(env);
    w.resize(768,768);

    w.show();

    return a.exec();
}
