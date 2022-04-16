#include "node.h"
#include <QPainter>
#include <QRect>
#include <QPoint>
#include <QBrush>
#include "transferfunctionrenderer.h"

Node::Node(QWidget *parent, QVector2D pos, QColor color)
    : QWidget{parent},
      m_pos(pos),
      m_color(color)
{
    move(m_pos.toPoint());

    connect(
        parent, SIGNAL(signalUpdateNodeValues()),
        this, SLOT(updateValues())
    );

    connect(
        this, SIGNAL(nodeSelected(Node*)),
        parent, SLOT(slotNodeSelected(Node*))
    );
}

QSize Node::sizeHint() const{
    return QSize(15,15);
}

/**
 * @brief Node::paintEvent
 * paints a circle and a colored square at the nodes position
 * @param event
 */
void Node::paintEvent(QPaintEvent *event){
    setPos(m_pos);
    QPainter p;
    QPen pen = QPen();
    QRect fullWidgetRect = this->rect();
    QRect paintRect = QRect(0,0,fullWidgetRect.width()-1,fullWidgetRect.height()-1);
    p.begin(this);
    p.setPen(pen);
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
    //MOVING THE YELLOW NODE MAKES THE TRANSFER FUNCTION NOT WORK SOMEHOW??s
    QPoint newWidgetPos = this->pos();
    QRect parentRect = parentWidget()->rect();
    QVector2D newPos = QVector2D(
        float(newWidgetPos.x())/float(parentRect.width()),
        float(newWidgetPos.y())/float(parentRect.height())
    );
    setPos(newPos);
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
    parentWidget()->repaint();
}

void Node::mousePressEvent(QMouseEvent *event){
    m_isSelected = true;
    emit nodeSelected(this);
}

void Node::mouseReleaseEvent(QMouseEvent *event){
    m_isSelected = false;
}
