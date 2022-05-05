#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QVector>
#include <QColor>
#include <QVector2D>
#include <QVBoxLayout>
#include "transferfunctionrenderer.h"
#include "node.h"
#include "environment.h"
#include "histogramwidget.h"

TransferFunctionRenderer::TransferFunctionRenderer(Environment *env, QWidget *parent)
    : QWidget{parent},
      m_environment(env)
{
    createDefaultNodes();

    QSizePolicy sizePolicy = QSizePolicy();
    sizePolicy.setHorizontalPolicy(QSizePolicy::Policy::Preferred);
    sizePolicy.setVerticalPolicy(QSizePolicy::Policy::Preferred);
    setSizePolicy(sizePolicy);

    connect(
        parent, SIGNAL(updateNodeValues()),
        this, SLOT(updateNodeValuesSlot())
    );
    connect(
        parent, SIGNAL(createNewNodeSignal()),
        this, SLOT(createNewNode())
    );
    connect(
        this, SIGNAL(nodeSelectedSignal(Node*)),
        parent, SLOT(nodeSelectedSlot(Node*))
    );
    connect(
        this, SIGNAL(updateTransferFunction()),
        m_environment,SLOT(slotTransferFunctionChanged()) //Why don't this work??
    );
}

/**
 * @brief TransferFunctionRenderer::createDefaultNodes
 * Creates a set of 5 default nodes
 */
void TransferFunctionRenderer::createDefaultNodes(){
    Node *node1 = new Node(
        this,
        QVector2D(.1,.1),
        QColor(255,0,0)
    );

    Node *node2 = new Node(
        this,
        QVector2D(.4,.4),
        QColor(255,255,0)
    );
    Node *node3 = new Node(
        this,
        QVector2D(.6,.6),
        QColor(0,255,0)
    );

    Node *node4 = new Node(
        this,
        QVector2D(.8,.8),
        QColor(0,255,255)
    );

    Node *node5 = new Node(
        this,
        QVector2D(1,1),
        QColor(0,0,255)
    );

    QVector<Node*> nodes;
    nodes.emplaceBack(node1);
    nodes.emplaceBack(node2);
    nodes.emplaceBack(node3);
    nodes.emplaceBack(node4);
    nodes.emplaceBack(node5);
    m_environment->setNodes(nodes);
}

void TransferFunctionRenderer::nodeSelectedSlot(Node *node){
    emit nodeSelectedSignal(node);
}

void TransferFunctionRenderer::updateTransferFunctionSlot() {
    emit updateTransferFunction();
}

//SLOT
void TransferFunctionRenderer::createNewNode(){
    Node *newNode = new Node(this);
    newNode->setColor(QColor(255,0,255));
    newNode->setPos(QVector2D(0,0));
    QVector<Node*> oldNodes = m_environment->getNodes();
    oldNodes.append(newNode);
    m_environment->setNodes(oldNodes);
    newNode->show();
    qDebug() << "new Node" << newNode->pos();
}

/**
 * @brief TransferFunctionRenderer::slotUpdateNodeValues
 * SLOT
 * Trigggered when submit button is clicked in the TransferFunctionWidget
 * Signals nodes to update their values
 */
void TransferFunctionRenderer::updateNodeValuesSlot(){
    emit updateNodeValuesSignal();
}
/**
 * @brief TransferFunctionRenderer::paintEvent
 * Paints the container which holds the transfer function nodes
 * @param event
 */
void TransferFunctionRenderer::paintEvent(QPaintEvent * event){
    QRect fullWidgetRect = this->rect();
    QRect paintRect = QRect(1,1,fullWidgetRect.width()-2,fullWidgetRect.height()-2);
    QPen gridPen = QPen(QColor::fromRgbF(.7,.7,.7));
    QPen defaultPen = QPen(QColor::fromRgbF(0,0,0));
    QPainter p;
    p.begin(this);
    p.fillRect(fullWidgetRect,QColor::fromRgbF(1,1,1));
    p.setPen(defaultPen);
//    p.drawRect(paintRect);
    const float horizontalSteps = fullWidgetRect.width()/10.f;
    p.setPen(gridPen);
    for (float x=.0; x<fullWidgetRect.width(); x+=horizontalSteps){
        p.drawLine(QLine(x,0,x,fullWidgetRect.height()));
    }
    const float verticalSteps = fullWidgetRect.height()/10.f;
    for (float y=.0; y<fullWidgetRect.height(); y+=verticalSteps){
        p.drawLine(QLine(0,y,fullWidgetRect.width(),y));
    }
    QVector<Node*> nodes = m_environment->getNodes();
    QLine lineFromZeroToNodeOne = QLine(QPoint(0,0),nodes.at(0)->pos());
    p.drawLine(lineFromZeroToNodeOne);
    for (int i=1; i<nodes.size(); i++){
        QPoint node1Pos = nodes.at(i-1)->pos();
        QSize nodeSize = nodes.at(i-1)->size();
        QPoint node1MidPos = QPoint(node1Pos.x()+nodeSize.width()/2.f,node1Pos.y()+nodeSize.height()/2.f);
        QPoint node2Pos = nodes.at(i)->pos();
        QPoint node2MidPos = QPoint(node2Pos.x()+nodeSize.width()/2.f,node2Pos.y()+nodeSize.height()/2.f);
        QLine lineBetweenNodes = QLine(node1MidPos,node2MidPos);
        p.drawLine(lineBetweenNodes);
    }
    QPoint lastNodePos = nodes.at(nodes.size()-1)->pos();
    QPoint lastPoint = QPoint(fullWidgetRect.width(),lastNodePos.y());
    QLine lineFromLastNodeToEnd = QLine(lastNodePos,lastPoint);
    p.drawLine(lineFromLastNodeToEnd);
    p.end();
}

void TransferFunctionRenderer::resizeEvent(QResizeEvent *event){
    QVector<Node*> nodes = m_environment->getNodes();
    if (event->size().width() != event->oldSize().width()){
        for (int i=0; i<nodes.size(); i++){
            Node* node = nodes.at(i);
            node->rendererResized(event->oldSize(), event->size());
        }
    }
    QWidget::resizeEvent(event);
}

