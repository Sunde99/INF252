#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QWidget>
#include <qboxlayout.h>
#include "environment.h"
#include "transferfunctionwidget.h"
#include "crosssectionrenderer.h"


class SideBar : public QWidget
{
    Q_OBJECT
public:
    explicit SideBar(Environment *env, QWidget *parent = nullptr);

signals:
    void signalBoundingBoxCorner1Changed(QVector3D *changeVector);
    void signalBoundingBoxCorner2Changed(QVector3D *changeVector);
public slots:
    void boxSizeXSliderChanged(int);
    void boxSizeYSliderChanged(int);
    void boxSizeZSliderChanged(int);
    void boxPosXSliderChanged(int);
    void boxPosYSliderChanged(int);
    void boxPosZSliderChanged(int);

private:
    void createBoxSizeSliders();
    void createBoxPosSliders();
    Environment *m_environment;
    QBoxLayout *m_layout;
    TransferFunctionWidget *m_tfWidget;
    CrossSectionRenderer *m_csRenderer;

};

#endif // SIDEBAR_H
