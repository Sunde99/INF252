#include "renderwidget.h"
#include "geometry.h"
#include <QMouseEvent>
#include <QtMath>

RenderWidget::RenderWidget(Environment *env, QWidget *parent, Qt::WindowFlags f) : QOpenGLWidget(parent,f), m_environment(env)
{
    m_modelViewMatrix.setToIdentity();
    m_modelViewMatrix.translate(0.0, 0.0, -2.0*sqrt(3.0));
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

    if (!m_cubeProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/cube-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_cubeProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/cube-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_cubeProgram.link())
        qDebug() << "Could not link shader program!";

    if (!m_blockProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/block-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_blockProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/block-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_blockProgram.link())
        qDebug() << "Could not link shader program!";
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

/*
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   // glColor4f(1.0f,0.0f,0.0f,1.0f);

    glEnable(GL_TEXTURE_3D);
    //m_environment->volume()->bind();



    glBegin(GL_QUADS);
    glVertex2f(-0.5f,-0.5f);
    glTexCoord3d(0.0f,0.0f,0.5f);

    glVertex2f(0.5f,-0.5f);
    glTexCoord3d(1.0f,0.0f,0.5f);

    glVertex2f(0.5f,0.5f);
    glTexCoord3d(1.0f,1.0f,0.5f);

    glVertex2f(-0.5f,0.5f);
    glTexCoord3d(0.0f,1.0f,0.5f);

    glEnd();
*/
/*
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    QMatrix4x4 modelViewProjectionMatrix = m_projectionMatrix*m_modelViewMatrix;


    m_cubeProgram.bind();
    m_cubeProgram.setUniformValue("modelViewProjectionMatrix",modelViewProjectionMatrix);

    glActiveTexture(GL_TEXTURE0);
    m_cubeProgram.setUniformValue("volumeTexture",0);
    m_environment->volume()->bind();

    Geometry::instance()->bindCube(); // Geometry::instance()->bindQuad();

    int location = m_cubeProgram.attributeLocation("vertexPosition");
    m_cubeProgram.enableAttributeArray(location);
    m_cubeProgram.setAttributeBuffer(location,GL_FLOAT,0,3,sizeof(QVector3D));

    Geometry::instance()->drawCube(); // Geometry::instance()->drawQuad();

    glActiveTexture(GL_TEXTURE0);
    m_environment->volume()->release();

    m_cubeProgram.release();
*/

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

    m_blockProgram.bind();
    m_blockProgram.setUniformValue("modelViewProjectionMatrix",modelViewProjectionMatrix);
    m_blockProgram.setUniformValue("blockSize",blockSize);
    m_blockProgram.setUniformValue("volumeSize",volumeSize);

    glActiveTexture(GL_TEXTURE0);
    m_blockProgram.setUniformValue("volumeTexture",0);
    m_environment->volume()->bind();

    Geometry::instance()->bindCube(); // Geometry::instance()->bindQuad();

    int location = m_cubeProgram.attributeLocation("vertexPosition");
    m_blockProgram.enableAttributeArray(location);

    m_blockProgram.setAttributeBuffer(location,GL_FLOAT,0,3,sizeof(QVector3D));

    // this is only a quick & dirty example of how to render our block geometry
    // a much better way to do this without having to iterate on the CPU is
    // to use instancing, see e.g. https://learnopengl.com/Advanced-OpenGL/Instancing
    const QVector<QVector3D> & visibleBlockPositions = m_environment->volume()->visibleBlockPositions();

    for (int i=0;i<visibleBlockPositions.size();i++)
    {
        m_blockProgram.setUniformValue("blockPosition",visibleBlockPositions[i]);
        Geometry::instance()->drawCube(); // Geometry::instance()->drawQuad();
    }


    glActiveTexture(GL_TEXTURE0);
    m_environment->volume()->release();

    m_blockProgram.release();

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


