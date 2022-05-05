#include "environment.h"
#include <vector>

Environment::Environment(QObject *parent) : QObject(parent)
{
    m_volume = new Volume(this);
    m_nodes = QVector<Node*>();
}

void Environment::setNodes(QVector<Node*> nodes){
    m_nodes = nodes;
}

//SLOT
void Environment::slotTransferFunctionChanged(){
    emit signalTransferFunctionChanged();
}

