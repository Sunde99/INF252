#include "node.h"
#include <QPainter>
#include <QRect>
#include <QPoint>

Node::Node(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(10,10);
    setPos(QVector2D(0,0));
    m_color = QColor(255,0,255);
    connect(
        parent, SIGNAL(signalUpdateNodeValues()),
        this, SLOT(updateValues())
    );
}

/**
 * @brief Node::paintEvent
 * paints a circle and a colored square at the nodes position
 * @param event
 */
void Node::paintEvent(QPaintEvent *event){
    setPos(m_pos);
    QPainter p;
    QRect fullWidgetRect = this->rect();
    QRect paintRect = QRect(0,0,fullWidgetRect.width()-1,fullWidgetRect.height()-1);
    p.begin(this);
    p.fillRect(paintRect, m_color);
    p.drawArc(paintRect,0,5760);
    p.end();
}

//SLOT
/**
 * @brief Node::updateValues
 * SLOT
 * is triggered when the Submit button in TransferFunctionWidget is clicked
 */
void Node::updateValues(){
    qDebug() << "node got pinged!";
    //MOVING THE YELLOW NODE MAKES THE TRANSFER FUNCTION NOT WORK SOMEHOW??s
    QPoint newWidgetPos = this->pos();
    QRect parentRect = parentWidget()->rect();
    QVector2D newPos = QVector2D(
        float(newWidgetPos.x())/float(parentRect.width()),
        float(newWidgetPos.y())/float(parentRect.height())
    );
    setPos(newPos);
    qDebug() << "On Update: Node" << m_color.name() << ": " << m_pos.y();
}

/**
 * @brief Node::getInfo
 * @return the color and alpha value to be used in transfer function texture generation
 */
QVector4D Node::getInfo(){
    QVector4D info = QVector4D(
        m_color.redF(),
        m_color.greenF(),
        m_color.blueF(),
        m_pos.y()
    );
    qDebug() << "Node" << m_color.name() << ": " << m_pos.y();
    return info;
}

/**
 * @brief Node::mouseMoveEvent
 * When clicking and dragging the mouse over a Node
 * the node will follow the mouse within the container until the mouse is released
 * @param event
 */
void Node::mouseMoveEvent(QMouseEvent *event){
    auto pos = this->pos() + event->pos();
    move(pos);
}
