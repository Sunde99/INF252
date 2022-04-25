#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "environment.h"
#include "renderwidget.h"

class QHBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Environment *env, QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void doCompute();

public slots:
    void fileOpen();
    void addWidget();
    void removeWidget();
    void computeActionTriggered();

private:
    Environment *m_environment;
    QWidget *m_mainWidget;
    QHBoxLayout *m_layout;
    QVector<RenderWidget*> m_renderWidgets;
    QOpenGLWidget *m_hiddenWidget;
};
#endif // MAINWINDOW_H
