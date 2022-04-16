#include "renderwidget.h"
#include <QMouseEvent>
#include <QtMath>
#include <QVector>
#include "geometry.h"
#include "transferfunctiontexturegenerator.h"

RenderWidget::RenderWidget(Environment *env, QWidget *parent, Qt::WindowFlags f) :
    QOpenGLWidget(parent,f),
    m_environment(env),
    m_volumeTexture(QOpenGLTexture::Target3D),
    m_histogramTexture(QOpenGLTexture::Target2D),
    m_transferFunctionTexture(QOpenGLTexture::Target1D)
{
    m_modelViewMatrix.setToIdentity();
    m_modelViewMatrix.translate(0.0, 1.0, -3.0*sqrt(3.0));
    m_showCompute = false;
    connect(m_environment,SIGNAL(signalTransferFunctionChanged()),this,SLOT(createTransferFunction()));
}


/**
 * @brief RenderWidget::createTransferFunction
 * SLOT
 * Generates transfer function texture to be used in shader
 */
void RenderWidget::createTransferFunction(){
    if (m_transferFunctionTexture.isCreated())
        m_transferFunctionTexture.destroy();

    const QVector<Node*> nodes = m_environment->getNodes();
    const int middleNodes = 100;
    const int width = nodes.size() + middleNodes;
    const int height = 1;
    const int depth = 4;

    TransferFunctionTextureGenerator generator = TransferFunctionTextureGenerator(nodes,middleNodes);
    QVector<float> textureData = generator.generateTextureData();

    m_transferFunctionTexture.setBorderColor(0,0,0,0);
    m_transferFunctionTexture.setWrapMode(QOpenGLTexture::ClampToEdge);
    m_transferFunctionTexture.setFormat(QOpenGLTexture::RGBA16F);
    m_transferFunctionTexture.setMinificationFilter(QOpenGLTexture::Linear);
    m_transferFunctionTexture.setMagnificationFilter(QOpenGLTexture::Linear);
    m_transferFunctionTexture.setAutoMipMapGenerationEnabled(false);
    m_transferFunctionTexture.setSize(width,height,depth);
    m_transferFunctionTexture.allocateStorage();

    void *data = textureData.data();
    m_transferFunctionTexture.setData(0,0,0,width,height,depth,QOpenGLTexture::RGBA,QOpenGLTexture::Float32,data);

    update();
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    m_currentX = qreal(event->x());
    m_currentY = qreal(event->y());

    m_previousX = m_currentX;
    m_previousY = m_currentY;
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_currentX = qreal(event->x());
    m_currentY = qreal(event->y());

    if (event->buttons() & Qt::LeftButton)
    {
        if (m_currentX != m_previousX || m_currentY != m_previousY)
        {
            QVector3D va = arcballVector(m_previousX, m_previousY);
            QVector3D vb = arcballVector(m_currentX, m_currentY);

            if (va != vb)
            {
                qreal angle = acos(qMax(-1.0f, qMin(1.0f, QVector3D::dotProduct(va, vb))));
                QVector3D axis = QVector3D::crossProduct(va,vb);

                QMatrix4x4 inverseModelViewMatrix = m_modelViewMatrix.inverted();
                QVector4D transformedAxis = inverseModelViewMatrix*QVector4D(axis,0.0f);

                m_modelViewMatrix.rotate(qRadiansToDegrees(angle),transformedAxis.toVector3D());
            }
        }
    }

    m_previousX = m_currentX;
    m_previousY = m_currentY;

    update();
}

void RenderWidget::initializeGL()
{
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
    //Raymarching
    if (!m_raymarchingProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/screen-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_raymarchingProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/raymarching-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_raymarchingProgram.link())
        qDebug() << "Could not link shader program!";

    m_histogramTexture.setBorderColor(0,0,0,0);
    m_histogramTexture.setWrapMode(QOpenGLTexture::ClampToEdge);
    m_histogramTexture.setFormat(QOpenGLTexture::R32U);
    m_histogramTexture.setMinificationFilter(QOpenGLTexture::Linear);
    m_histogramTexture.setMagnificationFilter(QOpenGLTexture::Linear);
    m_histogramTexture.setAutoMipMapGenerationEnabled(false);
    m_histogramTexture.setSize(256,256);
    m_histogramTexture.allocateStorage();

    createTransferFunction();
}

void RenderWidget::doCompute()
{
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

    m_showCompute = true;
    update();
}


void RenderWidget::resizeGL(int w, int h)
{
    qreal aspectRatio = qreal(w) / qreal(h);

    qreal nearPlane = 0.5;
    qreal farPlane = 32.0;
    qreal fov = 60.0;

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(fov,aspectRatio,nearPlane,farPlane);
}

void RenderWidget::paintGL()
{
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //BRUH
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // this matrix converts from volume coordinates ([0,0,0]-[width-1,height-1,depth-1]) to our unit cube coordinates [-1,-1,-1]-[1,1,1]
    QMatrix4x4 volumeToUnitCoordinates;
    volumeToUnitCoordinates.translate(-1.0f,-1.0f,-1.0f);
    volumeToUnitCoordinates.scale(2.0f);
    volumeToUnitCoordinates.scale(1.0f/float(m_environment->volume()->width()),1.0f/float(m_environment->volume()->height()),1.0f/float(m_environment->volume()->depth()));

    QMatrix4x4 modelViewProjectionMatrix = m_projectionMatrix*m_modelViewMatrix*volumeToUnitCoordinates;
    QVector3D blockSize = QVector3D(float(BLOCKDIMENSION),float(BLOCKDIMENSION),float(BLOCKDIMENSION));
    QVector3D volumeSize = QVector3D(float(m_environment->volume()->width()),float(m_environment->volume()->height()),float(m_environment->volume()->depth()));

    m_raymarchingProgram.bind();
    m_raymarchingProgram.setUniformValue("MVP",modelViewProjectionMatrix.inverted());
    m_raymarchingProgram.setUniformValue("volumeSpacing",QVector3D(1,1,1));
    m_raymarchingProgram.setUniformValue("volumeScale",volumeSize);

    glActiveTexture(GL_TEXTURE0 + 0);
    m_raymarchingProgram.setUniformValue("volumeTexture",0);

    m_environment->volume()->bind();

    Geometry::instance()->bindQuad();

    int location = m_raymarchingProgram.attributeLocation("vertexPosition");
    m_raymarchingProgram.enableAttributeArray(location);

    m_raymarchingProgram.setAttributeBuffer(location,GL_FLOAT,0,3,sizeof(QVector3D));

    GLuint samplerLocation = m_raymarchingProgram.uniformLocation("transferFunction");
    glUniform1i(samplerLocation, 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_1D, m_transferFunctionTexture.textureId());

    Geometry::instance()->drawQuad();

    glActiveTexture(GL_TEXTURE0);

    m_environment->volume()->release();

    m_raymarchingProgram.release();
    //BRUH

    glViewport(0,0,m_histogramTexture.width(),m_histogramTexture.height());

    m_histogramProgram.bind();
    Geometry::instance()->bindQuad();

    glBindImageTexture(0,m_histogramTexture.textureId(),0,GL_FALSE,0,GL_READ_ONLY,GL_R32UI);

    location = m_histogramProgram.attributeLocation("vertexPosition");
    m_histogramProgram.enableAttributeArray(location);
    m_histogramProgram.setAttributeBuffer(location,GL_FLOAT,0,3,sizeof(QVector3D));

    Geometry::instance()->drawQuad();
    m_histogramProgram.release();

    m_histogramTexture.release();

}

QVector3D RenderWidget::arcballVector(qreal x, qreal y)
{
    QVector3D p = QVector3D(2.0f*float(x) / float(width())-1.0f, -2.0f*float(y) / float(height())+1.0f, 0.0);

    float length2 = p.x()*p.x() + p.y()*p.y();

    if (length2 < 1.0f)
        p.setZ(sqrtf(1.0f - length2));
    else
        p.normalize();

    return p;
}


