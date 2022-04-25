#include "transferfunctionbuttonbar.h"
#include <QPushButton>

TransferFunctionButtonBar::TransferFunctionButtonBar(Environment *env, QWidget *parent)
    : QWidget{parent},
      m_environment(env)
{
    m_layout = new QBoxLayout(QBoxLayout::LeftToRight,this);
    // Color  Dialog Button
    m_colorDialogButton = new QPushButton("Color",this);
    m_layout->addWidget(m_colorDialogButton);
    // Color Dialog
    m_colorDialog = new QColorDialog(this);
    m_layout->addWidget(m_colorDialog);
    // Hex color text label
    m_colorNameTextLabel = new QLabel("None",this);
    m_layout->addWidget(m_colorNameTextLabel);
    // Add node button
    m_addNodeButton = new QPushButton("+",this);
    m_layout->addWidget(m_addNodeButton);

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
    //Adds a new node when + button is clicked
    connect(
        m_addNodeButton, SIGNAL(clicked()),
        parent, SLOT(createNewNodeSlot())
    );
    connect(
        parent, SIGNAL(nodeSelectedSignal(Node*)),
        this, SLOT(nodeSelectedSlot(Node*))
    );

}

//SLOT
void TransferFunctionButtonBar::nodeSelectedSlot(Node *node){
    setColorName(node->getColor());
    setColorButtonColor(node->getColor());
}

//SLOT
void TransferFunctionButtonBar::setColorName(QColor color){
    m_colorNameTextLabel->setText(color.name());
}

//SLOT
void TransferFunctionButtonBar::setColorButtonColor(QColor color){
    m_colorDialogButton->setPalette(QPalette(color));
}
