#ifndef TRANSFERFUNCTIONTEXTUREGENERATOR_H
#define TRANSFERFUNCTIONTEXTUREGENERATOR_H

#include <qvector.h>
#include <qvector4d.h>
#include "node.h"

class TransferFunctionTextureGenerator
{
public:
    TransferFunctionTextureGenerator(QVector<Node*> nodes, int size);
    QVector<float> generateTextureData();
private:
    void swap(int a, int b);
    void sortNodesByXCoord();
    void placeMiddleValues();
    bool compare(int a, int b);
    QVector<float> interpolate(QVector<float> node1, QVector<float> node2, float x);
    QVector<float> interpolate(int nodeIdx1, int nodeIdx2, float x);
    float linearInterpolation(float a, float b, float t);

    QVector<Node*> m_nodes;
    QVector<QVector<float>> m_nodeData;
    int m_size;
};

#endif // TRANSFERFUNCTIONTEXTUREGENERATOR_H
