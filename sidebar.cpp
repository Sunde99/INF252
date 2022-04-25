#include "sidebar.h"
#include <QBoxLayout>

SideBar::SideBar(Environment *env, QWidget *parent)
    : QWidget{parent},
      m_environment(env)
{
    m_layout = new QBoxLayout(QBoxLayout::TopToBottom,this);

    //CROSS SECTION VIEWER
    m_csRenderer = new CrossSectionRenderer(m_environment,this);
    m_layout->addWidget(m_csRenderer,1);

    // TRANSFER FUNCTION WIDGET
    m_tfWidget = new TransferFunctionWidget(m_environment,this);
    m_layout->addWidget(m_tfWidget,1);

}
