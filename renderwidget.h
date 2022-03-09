#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <QOpenGLExtraFunctions>
#include "environment.h"

class RenderWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    RenderWidget(Environment *env, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();
    QVector3D arcballVector(qreal x, qreal y);

private:
    Environment * m_environment;
    QOpenGLShaderProgram m_cubeProgram;
    QOpenGLShaderProgram m_blockProgram;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    qreal m_currentX,m_currentY;
    qreal m_previousX,m_previousY;
};

#endif // RENDERWIDGET_H
