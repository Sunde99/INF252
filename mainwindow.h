#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "environment.h"
#include "renderwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Environment *env, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void fileOpen();

private:
    Environment *m_environment;
    RenderWidget *m_renderWidget;
};
#endif // MAINWINDOW_H
