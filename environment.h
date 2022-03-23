#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QObject>
#include "volume.h"
#include <QSlider>

class Environment : public QObject
{
    Q_OBJECT
public:
    explicit Environment(QObject *parent = nullptr);

    Volume *volume();
signals:

private:
    Volume *m_volume;
};

#endif // ENVIRONMENT_H
