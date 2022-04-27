#ifndef TRANSFERFUNCTIONWIDGET_H
#define TRANSFERFUNCTIONWIDGET_H

#include <QObject>
#include <QWidget>
#include <QColorDialog>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include "transferfunctionrenderer.h"
#include "environment.h"
#include "transferfunctionbuttonbar.h"

class TransferFunctionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TransferFunctionWidget(Environment *env, QWidget *parent = nullptr);
protected:
//    virtual void paintEvent(QPaintEvent *event);

signals:
    void updateNodeValues();
    void updateTransferFunctionTexture();
    void createNewNodeSignal();
    void nodeSelectedSignal(Node *node);

public slots:
    void updateTransferFunction();
    void createNewNodeSlot();
    void nodeSelectedSlot(Node *node);
private:
    Environment * m_environment;
    TransferFunctionButtonBar * m_buttonBar;
    QBoxLayout * m_layout;
    TransferFunctionRenderer *m_transferFunctionBox;
    QPushButton *m_submitTransferFunctionChangesButton;
    Node *m_selectedNode;
};

#endif // TRANSFERFUNCTIONWIDGET_H
