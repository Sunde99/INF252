#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QVector>
#include <QColor>
#include <QVector2D>
#include "transferfunctionrenderer.h"
#include "node.h"
#include "environment.h"
#include "node.h"

TransferFunctionRenderer::TransferFunctionRenderer(Environment *env, QWidget *parent)
    : QWidget{parent},
      m_environment(env)
{
    Node *node1 = new Node(this);
    node1->setColor(QColor(255,0,0));
    node1->setPos(QVector2D(.2,0));

    Node *node2 = new Node(this);
    node2->setColor(QColor(255,255,0));
    node2->setPos(QVector2D(.4,.4));

    Node *node3 = new Node(this);
    node3->setColor(QColor(0,255,0));
    node3->setPos(QVector2D(.6,0.6));

    Node *node4 = new Node(this);
    node4->setColor(QColor(0,255,255));
    node4->setPos(QVector2D(.8,.8));

    Node *node5 = new Node(this);
    node5->setColor(QColor(0,0,255));
    node5->setPos(QVector2D(1,1));

    QVector<Node*> nodes;
    nodes.emplaceBack(node1);
    nodes.emplaceBack(node2);
    nodes.emplaceBack(node3);
    nodes.emplaceBack(node4);
    nodes.emplaceBack(node5);
    m_environment->setNodes(nodes);

    connect(
        parent, SIGNAL(updateNodeValues()),
        this, SLOT(slotUpdateNodeValues())
    );
    //TODO find out if having each node be a QWidget works
    //put an array of nodes in environment.h so it is accessible
    //fix renderwidget so it uses env->nodes()
    //make a for loop that makes each node paint itself
    //make each node able to be clicked
    //make nodes moveable
    //think about how to add remove nodes (a +/- button at first?)
}

void TransferFunctionRenderer::slotUpdateNodeValues(){
    qDebug() << "renderer got pinged!";
    emit signalUpdateNodeValues();
}

void TransferFunctionRenderer::paintEvent(QPaintEvent * event){
    QRect fullWidgetRect = this->rect();
    QRect paintRect = QRect(0,0,fullWidgetRect.width()-1,fullWidgetRect.height()-1);
    QPainter p;
    p.begin(this);
    p.drawRect(paintRect);
    p.end();
}
