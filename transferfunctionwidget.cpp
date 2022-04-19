#include "transferfunctionwidget.h"
#include <QFrame>
#include <QPushButton>
#include <QColorDialog>
#include <QLabel>
#include <QBoxLayout>
#include <QColor>

TransferFunctionWidget::TransferFunctionWidget(Environment *env, QWidget *parent)
    : QWidget{parent},
    m_environment(env)
{
    m_layout = new QBoxLayout(QBoxLayout::TopToBottom,this);
    setLayout(m_layout);

    m_colorDialogButton = new QPushButton("Color",this);
    m_colorDialog = new QColorDialog(this);
    m_colorNameTextLabel = new QLabel("None",this);
    m_transferFunctionBox = new TransferFunctionRenderer(m_environment,this);
    m_submitTransferFunctionChangesButton = new QPushButton("Submit",this);
    m_addNodeButton = new QPushButton("+",this);

    //Clicking the colog dialog button opens the color menu
    connect(
        m_colorDialogButton, SIGNAL(clicked()),
        m_colorDialog, SLOT(open())
    );
    //sets the color text label to the color selected after confirming color menu
    connect(
        m_colorDialog, SIGNAL(colorSelected(QColor)),
        this, SLOT(setColorName(QColor))
    );
    //Changes the colorDialogButton's color when a color is selected
    connect(
        m_colorDialog, SIGNAL(colorSelected(QColor)),
        this, SLOT(setColorButtonColor(QColor))
    );
    connect(
        m_submitTransferFunctionChangesButton, SIGNAL(clicked()),
        this, SLOT(updateTransferFunction())
    );
    //Trigger environment to make renderers update the tf texture
    connect(
        this, SIGNAL(updateTransferFunctionTexture()),
        m_environment,SLOT(slotTransferFunctionChanged())
    );
    //Adds a new node when + button is clicked
    connect(
        m_addNodeButton, SIGNAL(clicked()),
        this, SLOT(createNewNode())
    );

    m_layout->addWidget(m_colorDialogButton);
    m_layout->addWidget(m_colorDialog);
    m_layout->addWidget(m_colorNameTextLabel);
    m_layout->addWidget(m_addNodeButton);
    m_layout->addWidget(m_transferFunctionBox);
    m_layout->addWidget(m_submitTransferFunctionChangesButton);
}

//SLOT
void TransferFunctionWidget::createNewNode(){
    emit signalCreateNewNode();
}

//SLOT
void TransferFunctionWidget::nodeSelected(Node *node){
    m_selectedNode = node;
    setColorName(m_selectedNode->getColor());
    setColorButtonColor(m_selectedNode->getColor());
}

//SLOT
void TransferFunctionWidget::updateTransferFunction(){
    emit updateNodeValues();
    emit updateTransferFunctionTexture();
}

//SLOT
void TransferFunctionWidget::setColorName(QColor color){
    m_colorNameTextLabel->setText(color.name());
    m_selectedNode->setColor(color);
}

//SLOT
void TransferFunctionWidget::setColorButtonColor(QColor color){
    m_colorDialogButton->setPalette(QPalette(color));
}
