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

public slots:
    virtual void doCompute();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void drawSlice();
    virtual void paintGL();
    virtual void createTransferFunction();
//    virtual void pointLight();
    QVector3D arcballVector(qreal x, qreal y);

private:
    Environment * m_environment;
    QOpenGLShaderProgram m_cubeProgram;
    QOpenGLShaderProgram m_blockProgram;
    QOpenGLShaderProgram m_histogramProgram;
    QOpenGLShaderProgram m_computeProgram;
    QOpenGLShaderProgram m_raymarchingProgram;
    QOpenGLShaderProgram m_lightProgram;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    QVector3D m_lightCoords;
    qreal m_currentX,m_currentY;
    qreal m_previousX,m_previousY;
    QOpenGLTexture m_volumeTexture;
    QOpenGLTexture m_histogramTexture;
    bool m_showCompute;
    QOpenGLTexture m_transferFunctionTexture;

};

#endif // RENDERWIDGET_H
