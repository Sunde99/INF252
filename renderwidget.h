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
    QVector3D m_iniScale = QVector3D(1, 1, 1);

public slots:
    virtual void createTransferFunction();
    void raymarchStepsSliderChanged(int);
    void epsilonSliderChanged(int);
    void slotBoundingBoxCorner1Changed(QVector3D *changeVector);
    void slotBoundingBoxCorner2Changed(QVector3D *changeVector);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void drawSlice();
    virtual void paintGL();
    virtual void wheelEvent(QWheelEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    QVector3D arcballVector(qreal x, qreal y);

private:
    void pan(QVector3D direction);


    Environment * m_environment;
    QOpenGLShaderProgram m_histogramProgram;
    QOpenGLShaderProgram m_computeProgram;
    QOpenGLShaderProgram m_raymarchingProgram;
    QOpenGLShaderProgram m_lightProgram;
    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_modelViewMatrix;
    QVector3D m_lightCoords = QVector3D(1, 1, 0);
    QVector4D m_backgroundColor;
    qreal m_currentX,m_currentY;
    qreal m_previousX,m_previousY;
    QOpenGLTexture m_volumeTexture;
    QOpenGLTexture m_histogramTexture;
    bool m_showCompute;
    QOpenGLTexture m_transferFunctionTexture;
    int m_rayMarchStepsSliderValue;
    int m_epsilonSliderValue;
    QVector3D m_boxSize;
    QVector3D m_boxPos;

};

#endif // RENDERWIDGET_H
