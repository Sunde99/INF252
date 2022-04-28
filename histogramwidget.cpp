#include "histogramwidget.h"

HistogramWidget::HistogramWidget(Environment *env, QWidget *parent)
    : QOpenGLWidget{parent},
      m_environment(env),
      m_volumeTexture(QOpenGLTexture::Target3D),
      m_histogramTexture(QOpenGLTexture::Target2D)
{
    m_width = 1;
}


void HistogramWidget::initializeGL(){
    initializeOpenGLFunctions();

    // initialize geometry
    Geometry::instance();

    //Histogram
    if (!m_histogramProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/histogram-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_histogramProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/histogram-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_histogramProgram.link())
        qDebug() << "Could not link shader program!";
    //Compute
    if (!m_computeProgram.addShaderFromSourceFile(QOpenGLShader::Compute,":/shaders/volume-cs.glsl"))
        qDebug() << "Could not load compute shader!";

    if (!m_computeProgram.link())
        qDebug() << "Could not link shader program!";

    m_histogramTexture.setBorderColor(0,0,0,0);
    m_histogramTexture.setWrapMode(QOpenGLTexture::ClampToEdge);
    m_histogramTexture.setFormat(QOpenGLTexture::R32U);
    m_histogramTexture.setMinificationFilter(QOpenGLTexture::Linear);
    m_histogramTexture.setMagnificationFilter(QOpenGLTexture::Linear);
    m_histogramTexture.setAutoMipMapGenerationEnabled(false);
    m_histogramTexture.setSize(256,256);
    m_histogramTexture.allocateStorage();
}


void HistogramWidget::resizeGL(int w, int h){
    qDebug() << "bruh" << w << h << size().width() << size().height();
    m_width = w;
    update();
}


void HistogramWidget::paintGL(){
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //BRUH
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    m_histogramProgram.bind();
    Geometry::instance()->bindQuad();

    glBindImageTexture(0,m_histogramTexture.textureId(),0,GL_FALSE,0,GL_READ_ONLY,GL_R32UI);

    int location = m_histogramProgram.attributeLocation("vertexPosition");
    m_histogramProgram.enableAttributeArray(location);
    m_histogramProgram.setAttributeBuffer(location,GL_FLOAT,0,3,sizeof(QVector3D));
    float widthMult = m_width/227.f;
    qDebug() << widthMult;
    m_histogramProgram.setUniformValue("width",widthMult);

    Geometry::instance()->drawQuad();
    m_histogramProgram.release();

    m_histogramTexture.release();
}


void HistogramWidget::doCompute(){
    if (m_volumeTexture.width() != m_environment->volume()->width() || m_volumeTexture.height() != m_environment->volume()->height() || m_volumeTexture.depth() != m_environment->volume()->depth())
    {
        if (m_volumeTexture.isCreated())
            m_volumeTexture.destroy();

        m_volumeTexture.setBorderColor(0,0,0,0);
        m_volumeTexture.setWrapMode(QOpenGLTexture::ClampToBorder);
        m_volumeTexture.setFormat(QOpenGLTexture::R32F);
        m_volumeTexture.setMinificationFilter(QOpenGLTexture::Linear);
        m_volumeTexture.setMagnificationFilter(QOpenGLTexture::Linear);
        m_volumeTexture.setAutoMipMapGenerationEnabled(false);
        m_volumeTexture.setSize(m_environment->volume()->width(),m_environment->volume()->height(),m_environment->volume()->depth());
        m_volumeTexture.allocateStorage();
    }


    int groupSizeX = 4, groupSizeY = 4, groupSizeZ = 4;
    int numGroupsX = ceilf(float(m_environment->volume()->width())/float(groupSizeX));
    int numGroupsY = ceilf(float(m_environment->volume()->height())/float(groupSizeY));
    int numGroupsZ = ceilf(float(m_environment->volume()->depth())/float(groupSizeZ));

    m_computeProgram.bind();
    m_environment->volume()->bind();
    glBindImageTexture(0,m_environment->volume()->volumeTexture().textureId(),0,GL_TRUE,0,GL_READ_ONLY,GL_R32F);
    glBindImageTexture(1,m_volumeTexture.textureId(),0,GL_TRUE,0,GL_WRITE_ONLY,GL_R32F);
    glBindImageTexture(2,m_histogramTexture.textureId(),0,GL_FALSE,0,GL_READ_WRITE,GL_R32UI);
    glDispatchCompute(numGroupsX,numGroupsY,numGroupsZ);
    m_environment->volume()->release();
    m_computeProgram.release();

    update();
}
