#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>

MainWindow::MainWindow(Environment *env, QWidget *parent)
    : QMainWindow(parent), m_environment(env)
{
    /* we take ownership of the enviroment instance
     * so it will be propertly destroyed when the main
     * window closes */
    env->setParent(this);
    m_mainWidget = new QWidget(this);
    m_layout = new QHBoxLayout(m_mainWidget);

    setCentralWidget(m_mainWidget);
    addWidget();

    QMenu *fileMenu = new QMenu("File");

    QAction *fileOpenAction = new QAction("Open",this);
    connect(fileOpenAction,SIGNAL(triggered()),this,SLOT(fileOpen()));
    fileMenu->addAction(fileOpenAction);

    QAction *fileAddWidgetAction = new QAction("Add Widget",this);
    connect(fileAddWidgetAction,SIGNAL(triggered()),this,SLOT(addWidget()));
    fileMenu->addAction(fileAddWidgetAction);

    QAction *fileRemoveWidgetAction = new QAction("Remove Widget",this);
    connect(fileRemoveWidgetAction,SIGNAL(triggered()),this,SLOT(removeWidget()));
    fileMenu->addAction(fileRemoveWidgetAction);

    QAction *fileComputeAction = new QAction("Compute",this);
    connect(fileComputeAction,SIGNAL(triggered()),m_renderWidgets.first(),SLOT(doCompute()));
    fileMenu->addAction(fileComputeAction);

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

// WORKAROUND

void MainWindow::addWidget()
{
    bool createWidget = false;

    if (m_renderWidgets.size()==1)
    {
         RenderWidget *widget = m_renderWidgets.back();

         if (widget->isVisible())
             createWidget = true;
         else
         {
             widget->show();
            // TODO: reset any state of widget, since it was not newly created
         }
    }
    else
    {
        createWidget = true;
    }

    if (createWidget)
    {
        RenderWidget *widget = new RenderWidget(m_environment,m_mainWidget);
        m_layout->addWidget(widget);
        m_renderWidgets.push_back(widget);
    }
}

void MainWindow::removeWidget()
{
    if (!m_renderWidgets.empty())
    {
        RenderWidget *widget = m_renderWidgets.back();
        m_renderWidgets.pop_back();

        if (m_renderWidgets.empty())
            widget->hide();
        else
            delete widget;
   }
}
