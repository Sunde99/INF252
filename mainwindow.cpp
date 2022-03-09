#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

MainWindow::MainWindow(Environment *env, QWidget *parent)
    : QMainWindow(parent), m_environment(env)
{
    /* we take ownership of the enviroment instance
     * so it will be propertly destroyed when the main
     * window closes */
    env->setParent(this);

    m_renderWidget = new RenderWidget(env,parent);
    setCentralWidget(m_renderWidget);

    QMenu *fileMenu = new QMenu("File");
    QAction *fileOpenAction = new QAction("Open",this);
    connect(fileOpenAction,SIGNAL(triggered()),this,SLOT(fileOpen()));
    fileMenu->addAction(fileOpenAction);

    menuBar()->addMenu(fileMenu);

}

MainWindow::~MainWindow()
{
}

void MainWindow::fileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open Volume File",QString(),"*.dat");

    if (!fileName.isEmpty())
    {
        m_environment->volume()->load(fileName);
    }

}

