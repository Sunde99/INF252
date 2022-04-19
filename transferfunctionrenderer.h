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
    void signalNodeSelected(Node *node);
public slots:
    void slotUpdateNodeValues();
    void createNewNode();
    void slotNodeSelected(Node *node = nullptr);
private:
    Environment * m_environment;

};

#endif // TRANSFERFUNCTIONRENDERER_H
