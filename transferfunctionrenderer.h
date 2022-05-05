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
    virtual void resizeEvent(QResizeEvent *event);
signals:
    void updateNodeValuesSignal();
    void nodeSelectedSignal(Node *node);
    void updateTransferFunction();
public slots:
    void updateNodeValuesSlot();
    void createNewNode();
    void nodeSelectedSlot(Node *node = nullptr);
    void updateTransferFunctionSlot();
private:
    void createDefaultNodes();
    Environment * m_environment;

};

#endif // TRANSFERFUNCTIONRENDERER_H
