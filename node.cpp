#include "node.h"
#include <QPainter>
#include <QRect>
#include <QPoint>
#include <QBrush>
#include <QSizePolicy>
#include "transferfunctionrenderer.h"

Node::Node(QWidget *parent, QVector2D pos, QColor color)
    : QWidget{parent},
      m_pos(pos),
      m_color(color)
{
    QSizePolicy sizePolicy = QSizePolicy();
    sizePolicy.setHorizontalPolicy(QSizePolicy::Policy::Preferred);
    sizePolicy.setVerticalPolicy(QSizePolicy::Policy::Preferred);
    setSizePolicy(sizePolicy);
    moveWithinBoundaries(m_pos);

    connect(
        parent, SIGNAL(updateNodeValuesSignal()),
        this, SLOT(updateValues())
    );

    connect(
        this, SIGNAL(nodeSelected(Node*)),
        parent, SLOT(nodeSelectedSlot(Node*))
    );

    connect(
        this, SIGNAL(updateTransferFunctionSignal()),
        parent, SLOT(updateTransferFunctionSlot())
    );
}

QSize Node::sizeHint() const{
    return QSize(10,10);
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
    moveWithinBoundaries(pos.x(),pos.y());
    parentWidget()->repaint();
    updateValues();
    emit updateTransferFunctionSignal();
}

void Node::mousePressEvent(QMouseEvent *event){
    m_isSelected = true;
    emit nodeSelected(this);
}

void Node::mouseReleaseEvent(QMouseEvent *event){
    m_isSelected = false;
}

void Node::moveWithinBoundaries(QVector2D pos){
    float maxWidth = parentWidget()->rect().width()-rect().width();
    float maxHeight = parentWidget()->rect().height()-rect().height();
    if (pos.x()<.0){
        pos.setX(0);
    }
    if (pos.x()>maxWidth){
        pos.setX(maxWidth);
    }
    if (pos.y()>maxHeight){
        pos.setY(maxHeight);
    }
    if (pos.y()<.0){
        pos.setY(0);
    }
    move(pos.toPoint());
}

void Node::rendererResized(QSize oldSize, QSize newSize){
    setFixedSize(this->sizeHint());
    float relativeWidthIncrease;
    float relativeHeightIncrease;
    float posX;
    float posY;
    if (oldSize.width() == -1){
        relativeWidthIncrease = newSize.width();
        relativeHeightIncrease = newSize.height();
        posX = m_pos.x();
        posY = m_pos.y();
    }
    else{
        relativeWidthIncrease = float(newSize.width())/float(oldSize.width());
        relativeHeightIncrease = float(newSize.height())/float(oldSize.height());
        posX = pos().x();
        posY = pos().y();
    }
    moveWithinBoundaries(posX*relativeWidthIncrease, posY*relativeHeightIncrease);
}
