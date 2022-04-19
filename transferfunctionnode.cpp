#include "transferfunctionnode.h"

TransferFunctionNode::TransferFunctionNode(QWidget *parent)
    : QWidget{parent}
{

}

void TransferFunctionNode::paintEvent(QPaintEvent * event){
    QRect fullWidgetRect = this->rect();
    QRect paintRect = QRect(0,0,fullWidgetRect.width()-1,fullWidgetRect.height()-1);
    QPainter p;
    p.begin(this);
    p.drawRect(paintRect);
    p.end();
}

