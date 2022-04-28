#ifndef TRANSFERFUNCTIONBUTTONBAR_H
#define TRANSFERFUNCTIONBUTTONBAR_H

#include <QWidget>
#include "environment.h"
#include <QBoxLayout>
#include <QColorDialog>
#include <QLabel>

class TransferFunctionButtonBar : public QWidget
{
    Q_OBJECT
public:
    explicit TransferFunctionButtonBar(Environment *env, QWidget *parent = nullptr);

signals:
public slots:
    void setColor(QColor color);
    void nodeSelectedSlot(Node* = nullptr);

private:
    Environment * m_environment;
    QBoxLayout *m_layout;
    QColorDialog *m_colorDialog;
    QLabel *m_colorNameTextLabel;
    QPushButton *m_colorDialogButton;
    QPushButton *m_addNodeButton;
    Node *m_selectedNode;

};

#endif // TRANSFERFUNCTIONBUTTONBAR_H
