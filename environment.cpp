#include "environment.h"

Environment::Environment(QObject *parent) : QObject(parent)
{
    m_volume = new Volume(this);
}

Volume * Environment::volume()
{
    return m_volume;
}
