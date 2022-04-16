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
    void signalCreateNewNode();

public slots:
    void setColorName(QColor color);
    void setColorButtonColor(QColor color);
    void updateTransferFunction();
    void createNewNode();
    void nodeSelected(Node *node);
private:
    Environment * m_environment;
    QBoxLayout *m_layout;
    QColorDialog *m_colorDialog;
    QLabel *m_colorNameTextLabel;
    QPushButton *m_colorDialogButton;
    TransferFunctionRenderer *m_transferFunctionBox;
    QPushButton *m_submitTransferFunctionChangesButton;
    QPushButton *m_addNodeButton;
    Node *m_selectedNode;
};

#endif // TRANSFERFUNCTIONWIDGET_H
