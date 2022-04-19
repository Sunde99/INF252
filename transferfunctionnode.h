#ifndef TRANSFERFUNCTIONNODE_H
#define TRANSFERFUNCTIONNODE_H

#include <QWidget>

class TransferFunctionNode : public QWidget
{
    Q_OBJECT
public:
    explicit TransferFunctionNode(QWidget *parent = nullptr);
protected:
    virtual void paintEvent(QPaintEvent *event);

signals:

};

#endif // TRANSFERFUNCTIONNODE_H
