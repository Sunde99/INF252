#include "mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSettings>
#include "crosssectionrenderer.h"
#include "transferfunctionwidget.h"
#include "renderwidget.h"
#include "histogramwidget.h"
#include "sidebar.h"

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
//    addWidget();
    m_renderWidget = new RenderWidget(m_environment,m_mainWidget);
    m_layout->addWidget(m_renderWidget,2);

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
    fileMenu->addAction(fileComputeAction);

    menuBar()->addMenu(fileMenu);

    // SIDE BAR
    SideBar *sideBar = new SideBar(m_environment,m_mainWidget);
    m_layout->addWidget(sideBar,1);

    connect(fileComputeAction,SIGNAL(triggered()),this,SLOT(computeActionTriggered()));
    fileMenu->addAction(fileComputeAction);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::askForIni()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "ini", "Add ini?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        return true;
    } else {

        return false;
    }
}

void MainWindow::fileOpenIni()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open Volume File",QString(),"*.ini");

    if (!fileName.isEmpty())
    {
        QSettings settings(fileName, QSettings::IniFormat);
        settings.beginGroup("DatFile");
        const QStringList childKeys = settings.childKeys();
        int i = 0;
        QVector3D iniScale = m_renderWidget->m_iniScale;
        foreach (const QString &childKey, childKeys) {
            qDebug() << "Loop";
            iniScale[i++] = settings.value(childKey).toFloat();
        }
        m_renderWidget->m_iniScale = iniScale;
        settings.endGroup();
    }
}

void MainWindow::fileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Open Volume File",QString(),"*.dat");

    if (!fileName.isEmpty())
    {
        if (askForIni()) {
            qDebug() << "You clicked YES";
            fileOpenIni();
        } else {
            qDebug() << "You did NOT clicked yes";
        }
        m_environment->volume()->load(fileName);
    }

}

void MainWindow::computeActionTriggered(){
    emit doCompute();
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
        m_renderWidget = widget;
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

