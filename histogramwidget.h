#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLExtraFunctions>
#include "environment.h"
#include "geometry.h"

class HistogramWidget : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit HistogramWidget(Environment *env, QWidget *parent = nullptr);
public slots:
    void doCompute();


signals:
protected:
    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:
    Environment *m_environment;
    QOpenGLTexture m_volumeTexture;
    QOpenGLTexture m_histogramTexture;
    QOpenGLShaderProgram m_histogramProgram;
    QOpenGLShaderProgram m_computeProgram;

};

#endif // HISTOGRAMWIDGET_H
