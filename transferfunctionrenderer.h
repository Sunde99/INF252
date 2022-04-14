#ifndef TRANSFERFUNCTIONRENDERER_H
#define TRANSFERFUNCTIONRENDERER_H

#include <QWidget>
#include "environment.h"

class TransferFunctionRenderer : public QWidget
{
    Q_OBJECT
public:
    explicit TransferFunctionRenderer(Environment *env, QWidget *parent = nullptr);

protected:
    virtual void paintEvent(QPaintEvent *event);
signals:
    void signalUpdateNodeValues();
public slots:
    void slotUpdateNodeValues();
private:
    Environment * m_environment;

};

#endif // TRANSFERFUNCTIONRENDERER_H
