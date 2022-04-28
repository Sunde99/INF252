#include "transferfunctionwidget.h"
#include <QFrame>
#include <QPushButton>
#include <QColorDialog>
#include <QLabel>
#include <QBoxLayout>
#include <QColor>
#include <QPalette>
#include "histogramwidget.h"
#include "crosssectionrenderer.h"

TransferFunctionWidget::TransferFunctionWidget(Environment *env, QWidget *parent)
    : QWidget{parent},
    m_environment(env)
{

    m_layout = new QBoxLayout(QBoxLayout::TopToBottom,this);

    // TRANSFER FUNCTION
    m_transferFunctionBox = new TransferFunctionRenderer(m_environment,this);
    // SUBMIT TF CHANGES
    m_submitTransferFunctionChangesButton = new QPushButton("Submit",this);
    m_buttonBar = new TransferFunctionButtonBar(m_environment, this);
    connect(
        m_submitTransferFunctionChangesButton, SIGNAL(clicked()),
        this, SLOT(updateTransferFunction())
    );
    //Trigger environment to make renderers update the tf texture
    connect(
        this, SIGNAL(updateTransferFunctionTexture()),
        m_environment,SLOT(slotTransferFunctionChanged())
    );

    //HISTOGRAM
    HistogramWidget *hgWidget = new HistogramWidget(m_environment, this);
    connect(this->parentWidget()->parentWidget()->parentWidget(),SIGNAL(doCompute()),hgWidget,SLOT(doCompute()));

    m_layout->addWidget(m_buttonBar,1);
    m_layout->addWidget(m_transferFunctionBox,5);
    m_layout->addWidget(hgWidget,1);
    m_layout->addWidget(m_submitTransferFunctionChangesButton,1);

    m_buttonBar->show();
}

//SLOT
void TransferFunctionWidget::createNewNodeSlot(){
    emit createNewNodeSignal();
}

//SLOT
void TransferFunctionWidget::updateTransferFunction(){
    emit updateNodeValues();
    emit updateTransferFunctionTexture();
}

void TransferFunctionWidget::nodeSelectedSlot(Node *node){
    m_selectedNode = node;
    emit nodeSelectedSignal(node);
}
