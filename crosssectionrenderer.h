#ifndef CROSSSECTIONRENDERER_H
#define CROSSSECTIONRENDERER_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include "environment.h"

class CrossSectionRenderer : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit CrossSectionRenderer(Environment *env, QWidget *parent = nullptr);

signals:

public slots:
    void sliderChanged(int);
    void sliderOrientationChanged(int);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    QVector3D arcballVector(qreal x, qreal y);

private:
    void drawSlice();
    Environment * m_environment;
    QOpenGLShaderProgram m_crosssectionprogram;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    int m_slidervalue;
    int m_sliderOrientation;

};

#endif // CROSSSECTIONRENDERER_H
