#include "sidebar.h"
#include <QBoxLayout>
#include <QLabel>

SideBar::SideBar(Environment *env, QWidget *parent)
    : QWidget{parent},
      m_environment(env)
{
    m_layout = new QBoxLayout(QBoxLayout::TopToBottom,this);

    createBoxSizeSliders();
    createBoxPosSliders();

    //CROSS SECTION VIEWER
    m_csRenderer = new CrossSectionRenderer(m_environment,this);
    m_layout->addWidget(m_csRenderer,1);

    // TRANSFER FUNCTION WIDGET
    m_tfWidget = new TransferFunctionWidget(m_environment,this);
    m_layout->addWidget(m_tfWidget,1);

    connect(
        this, SIGNAL(signalBoundingBoxCorner1Changed(QVector3D*)),
        m_environment, SLOT(slotBoundingBoxCorner1Changed(QVector3D*))
    );
    connect(
        this, SIGNAL(signalBoundingBoxCorner2Changed(QVector3D*)),
        m_environment, SLOT(slotBoundingBoxCorner2Changed(QVector3D*))
    );
}

void SideBar::createBoxSizeSliders(){
    //BOX SIZE
    QWidget *boxSizeContainer = new QWidget(this);
    QLabel *labelX = new QLabel(boxSizeContainer);
    labelX->setText("Corner 1");
    QBoxLayout *boxSizeContainerLayout = new QBoxLayout(QBoxLayout::LeftToRight,boxSizeContainer);
    //SLIDER BOX SIZE X
    QSlider *sliderBoxSizeX = new QSlider(Qt::Horizontal, boxSizeContainer);
    sliderBoxSizeX->setMinimum(1);
    sliderBoxSizeX->setMaximum(100);
    sliderBoxSizeX->setValue(100);
    connect(sliderBoxSizeX,SIGNAL(valueChanged(int)),this,SLOT(boxSizeXSliderChanged(int)));

    //SLIDER BOX SIZE Y
    QSlider *sliderBoxSizeY = new QSlider(Qt::Horizontal, boxSizeContainer);
    sliderBoxSizeY->setMinimum(1);
    sliderBoxSizeY->setMaximum(100);
    sliderBoxSizeY->setValue(100);
    connect(sliderBoxSizeY,SIGNAL(valueChanged(int)),this,SLOT(boxSizeYSliderChanged(int)));

    //SLIDER BOX SIZE Z
    QSlider *sliderBoxSizeZ = new QSlider(Qt::Horizontal, boxSizeContainer);
    sliderBoxSizeZ->setMinimum(1);
    sliderBoxSizeZ->setMaximum(100);
    sliderBoxSizeZ->setValue(100);
    connect(sliderBoxSizeZ,SIGNAL(valueChanged(int)),this,SLOT(boxSizeZSliderChanged(int)));
    m_layout->addWidget(boxSizeContainer);

    boxSizeContainerLayout->addWidget(labelX);
    boxSizeContainerLayout->addWidget(sliderBoxSizeX);
    boxSizeContainerLayout->addWidget(sliderBoxSizeY);
    boxSizeContainerLayout->addWidget(sliderBoxSizeZ);

    boxSizeContainer->show();

}

void SideBar::createBoxPosSliders(){
    //BOX POS
    QWidget *boxPosContainer = new QWidget(this);
    QLabel *labelX = new QLabel(boxPosContainer);
    labelX->setText("Corner 2");
    QBoxLayout *boxPosContainerLayout = new QBoxLayout(QBoxLayout::LeftToRight,boxPosContainer);
    //SLIDER BOX POS X
    QSlider *sliderBoxPosX = new QSlider(Qt::Horizontal, boxPosContainer);
    sliderBoxPosX->setMinimum(0);
    sliderBoxPosX->setMaximum(200);
    sliderBoxPosX->setValue(0);
    connect(sliderBoxPosX,SIGNAL(valueChanged(int)),this,SLOT(boxPosXSliderChanged(int)));

    //SLIDER BOX Pos Y
    QSlider *sliderBoxPosY = new QSlider(Qt::Horizontal, boxPosContainer);
    sliderBoxPosY->setMinimum(0);
    sliderBoxPosY->setMaximum(200);
    sliderBoxPosY->setValue(0);
    connect(sliderBoxPosY,SIGNAL(valueChanged(int)),this,SLOT(boxPosYSliderChanged(int)));

    //SLIDER BOX Pos Z
    QSlider *sliderBoxPosZ = new QSlider(Qt::Horizontal, boxPosContainer);
    sliderBoxPosZ->setMinimum(0);
    sliderBoxPosZ->setMaximum(200);
    sliderBoxPosZ->setValue(0);
    connect(sliderBoxPosZ,SIGNAL(valueChanged(int)),this,SLOT(boxPosZSliderChanged(int)));

    boxPosContainerLayout->addWidget(labelX);
    boxPosContainerLayout->addWidget(sliderBoxPosX);
    boxPosContainerLayout->addWidget(sliderBoxPosY);
    boxPosContainerLayout->addWidget(sliderBoxPosZ);

    m_layout->addWidget(boxPosContainer);
    boxPosContainer->show();
}

void SideBar::boxSizeXSliderChanged(int value){
    float valuef = float(value)/100.0f;
    QVector3D *changeVector = new QVector3D(valuef,0,0);
    emit signalBoundingBoxCorner1Changed(changeVector);
}

void SideBar::boxSizeYSliderChanged(int value){
    float valuef = float(value)/100.0f;
    QVector3D *changeVector = new QVector3D(0,valuef,0);
    emit signalBoundingBoxCorner1Changed(changeVector);
}

void SideBar::boxSizeZSliderChanged(int value){
    float valuef = float(value)/100.0f;
    QVector3D *changeVector = new QVector3D(0,0,valuef);
    emit signalBoundingBoxCorner1Changed(changeVector);
}

void SideBar::boxPosXSliderChanged(int value){
    float valuef = float(value)/100.0f;
    QVector3D *changeVector = new QVector3D(valuef,0,0);
    emit signalBoundingBoxCorner2Changed(changeVector);
}

void SideBar::boxPosYSliderChanged(int value){
    float valuef = float(value)/100.0f;
    QVector3D *changeVector = new QVector3D(0,valuef,0);
    emit signalBoundingBoxCorner2Changed(changeVector);
}

void SideBar::boxPosZSliderChanged(int value){
    float valuef = float(value)/100.0f;
    QVector3D *changeVector = new QVector3D(0,0,valuef);
    emit signalBoundingBoxCorner2Changed(changeVector);
}
