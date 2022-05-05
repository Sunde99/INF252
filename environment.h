#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QObject>
#include "volume.h"
#include <QSlider>
#include <QVector>
#include "node.h"

class Environment : public QObject
{
    Q_OBJECT
public:
    explicit Environment(QObject *parent = nullptr);

    Volume *volume() {return m_volume;};
    QVector<Node*> getNodes() {return m_nodes;};
    void setNodes(QVector<Node*> nodes);
    bool hasTransferFunctionChanged() {return m_hasTransferFunctionChanged;};
signals:
    void signalTransferFunctionChanged();
    void signalBoundingBoxCorner1Changed(QVector3D *changeVector);
    void signalBoundingBoxCorner2Changed(QVector3D *changeVector);
public slots:
    void slotTransferFunctionChanged();
    void slotBoundingBoxCorner1Changed(QVector3D *changeVector);
    void slotBoundingBoxCorner2Changed(QVector3D *changeVector);


private:
    Volume *m_volume;
    QVector<Node*> m_nodes;
    bool m_hasTransferFunctionChanged;
};

#endif // ENVIRONMENT_H
