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
    m_boxSize = QVector3D(1,1,1);
    m_boxPos = QVector3D(0,0,0);
    m_epsilonSliderValue = 1;
    m_rayMarchStepsSliderValue = 1000;

    m_modelViewMatrix.setToIdentity();
    setFocus();
//    m_modelViewMatrix.translate(0.0, 1.0, -3.0*sqrt(3.0));
    m_modelViewMatrix.translate(0.0, 0, -5);
    m_showCompute = false;
    connect(m_environment,SIGNAL(signalTransferFunctionChanged()),this,SLOT(createTransferFunction()));

    //SLIDER RAYMARCH
    QSlider *sliderRaymarch = new QSlider(Qt::Horizontal, this);
    sliderRaymarch->setMinimum(100);
    sliderRaymarch->setMaximum(2000);
    sliderRaymarch->setValue(m_rayMarchStepsSliderValue);
    connect(sliderRaymarch,SIGNAL(valueChanged(int)),this,SLOT(raymarchStepsSliderChanged(int)));

    //SLIDER EPSILON
    QSlider *sliderEpsilon = new QSlider(Qt::Horizontal, this);
    sliderEpsilon->setMinimum(1);
    sliderEpsilon->setMaximum(1000);
    sliderEpsilon->setValue(m_epsilonSliderValue);
    sliderEpsilon->move(150,0);
    connect(sliderEpsilon,SIGNAL(valueChanged(int)),this,SLOT(epsilonSliderChanged(int)));

    connect(m_environment,SIGNAL(signalBoundingBoxCorner1Changed(QVector3D*)),this,SLOT(slotBoundingBoxCorner1Changed(QVector3D*)));
    connect(m_environment,SIGNAL(signalBoundingBoxCorner2Changed(QVector3D*)),this,SLOT(slotBoundingBoxCorner2Changed(QVector3D*)));
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

void RenderWidget::slotBoundingBoxCorner1Changed(QVector3D *changeVector){
    if (changeVector->x() > .0){
        m_boxSize.setX(changeVector->x());
    }
    else if (changeVector->y() > .0){
        m_boxSize.setY(changeVector->y());
    }
    else if (changeVector->z() > .0){
        m_boxSize.setZ(changeVector->z());
    }
    update();
}
void RenderWidget::slotBoundingBoxCorner2Changed(QVector3D *changeVector){
    if (changeVector->x() > .0){
        m_boxPos.setX(changeVector->x());
    }
    else if (changeVector->y() > .0){
        m_boxPos.setY(changeVector->y());
    }
    else if (changeVector->z() > .0){
        m_boxPos.setZ(changeVector->z());
    }
    update();
}

void RenderWidget::raymarchStepsSliderChanged(int value){
    m_rayMarchStepsSliderValue = value;
    update();
}


void RenderWidget::epsilonSliderChanged(int value){
    m_epsilonSliderValue = value;
    update();
}

void RenderWidget::mousePressEvent(QMouseEvent *event)
{
    setFocus();
    m_currentX = qreal(event->pos().x());
    m_currentY = qreal(event->pos().y());

    m_previousX = m_currentX;
    m_previousY = m_currentY;

    update();
}

void RenderWidget::mouseDoubleClickEvent(QMouseEvent *event) {
    setFocus();
    m_currentX = qreal(event->pos().x());
    m_currentY = qreal(event->pos().y());


    m_lightCoords = QVector3D(m_currentX, m_currentY, 0.0f);
    m_lightCoords[0] = (m_lightCoords[0]/width()) * 2 - 1;
    m_lightCoords[1] = (m_lightCoords[1]/height()) * 2 - 1;

    m_previousX = m_currentX;
    m_previousY = m_currentY;

    update();
}

void RenderWidget::pan(QVector3D direction){
    const float panSpeed = .1f;
    qDebug() << "panning" << direction;
    QVector3D dir = (m_modelViewMatrix.inverted() * QVector4D(direction*panSpeed,0)).toVector3D();
    m_modelViewMatrix.translate(dir);
}

void RenderWidget::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key::Key_W){ //PAN UP
        pan(QVector3D(0,-1,0));
    }
    else if (event->key() == Qt::Key::Key_A){ // PAN LEFT
        pan(QVector3D(1,0,0));
    }
    else if (event->key() == Qt::Key::Key_S){ // PAN DOWN
        pan(QVector3D(0,1,0));
    }
    else if (event->key() == Qt::Key::Key_D){ // PAN RIGHT
        pan(QVector3D(-1,0,0));
    }
    else if (event->key() == Qt::Key::Key_Home){ // RESET CAMERA
        m_modelViewMatrix.setToIdentity();
        m_modelViewMatrix.translate(QVector3D(0,0,-5));
    }
    update();
    QWidget::keyPressEvent(event);
}

void RenderWidget::wheelEvent(QWheelEvent *event){
    if (event->angleDelta().y() != .0){
        const float scrollSpeed = 10000.f;
        const float scrollAmount = 1+event->angleDelta().y() / scrollSpeed;
        m_modelViewMatrix.scale(scrollAmount);

        update();
    }
    if (event->angleDelta().x() != .0){
        const float scrollSpeed = 10000.f;
        const float scrollAmount = event->angleDelta().x() / scrollSpeed;
        m_modelViewMatrix.translate(QVector3D(scrollAmount,0,0));

        update();
    }
}

void RenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_currentX = qreal(event->x());
    m_currentY = qreal(event->y());
//    m_lightCoords = QVector3D(m_currentX, m_currentY, 0.0f);
//    m_lightCoords[0] = (m_lightCoords[0]/width()) * 2 - 1;
//    m_lightCoords[1] = (m_lightCoords[1]/height()) * 2 - 1;

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
        qDebug() << "Could not load histogram vertex shader!";

    if (!m_histogramProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/histogram-fs.glsl"))
        qDebug() << "Could not load histogram fragment shader!";

    if (!m_histogramProgram.link())
        qDebug() << "Could not link histogram shader program!";
    //Compute
    if (!m_computeProgram.addShaderFromSourceFile(QOpenGLShader::Compute,":/shaders/volume-cs.glsl"))
        qDebug() << "Could not load compute shader!";

    if (!m_computeProgram.link())
        qDebug() << "Could not link compute shader program!";
    //Raymarching
    if (!m_raymarchingProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/screen-vs.glsl"))
        qDebug() << "Could not load raymarch  vertex shader!";

    if (!m_raymarchingProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/raymarching-fs.glsl"))
        qDebug() << "Could not load raymarch fragment shader!";

    if (!m_raymarchingProgram.link())
        qDebug() << "Could not link raymarch shader program!";

    //Lightballthingy
    if (!m_lightProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/light-vs.glsl"))
        qDebug() << "Could not load light vertex shader!";

    if (!m_lightProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/light-fs.glsl"))
        qDebug() << "Could not load light fragment shader!";

    if (!m_lightProgram.link())
        qDebug() << "Could not link light shader program!";

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


void RenderWidget::resizeGL(int w, int h)
{
    qreal aspectRatio = qreal(w) / qreal(h);

    qreal nearPlane = 0.5;
    qreal farPlane = 32.0;
    qreal fov = 60.0;

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(fov,aspectRatio,nearPlane,farPlane);
}

void RenderWidget::drawSlice(){

    glBegin(GL_QUADS);

    glVertex2f(-1.f,-1.f);
    glVertex2f(1.f,-1.f);

    glVertex2f(1.f,1.f);
    glVertex2f(-1.f,1.f);
    glEnd();
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
//    volumeToUnitCoordinates.scale(1.0f/float(m_environment->volume()->width()),1.0f/float(m_environment->volume()->height()),1.0f/float(m_environment->volume()->depth()));

    QMatrix4x4 modelViewProjectionMatrix = m_projectionMatrix*m_modelViewMatrix*volumeToUnitCoordinates;
    // QVector3D blockSize = QVector3D(float(BLOCKDIMENSION),float(BLOCKDIMENSION),float(BLOCKDIMENSION));
    QVector3D volumeSize = QVector3D(float(m_environment->volume()->width()),float(m_environment->volume()->height()),float(m_environment->volume()->depth()));

    m_raymarchingProgram.bind();
    m_raymarchingProgram.setUniformValue("MVP",modelViewProjectionMatrix.inverted());
    m_raymarchingProgram.setUniformValue("volumeSpacing",m_iniScale);

    m_raymarchingProgram.setUniformValue("volumeScale",volumeSize);

    m_backgroundColor = QVector4D(0.0,0.0,1.0,1);
    m_raymarchingProgram.setUniformValue("backgroundColor", m_backgroundColor);
    GLuint samplerLocation1 = m_raymarchingProgram.uniformLocation("volumeTexture");
    glUniform1i(samplerLocation1, 0);

    m_raymarchingProgram.setUniformValue("lightDir",m_lightCoords);
    m_raymarchingProgram.setUniformValue("RAYMARCH_STEPS",m_rayMarchStepsSliderValue);
    float epsilon = float(m_epsilonSliderValue) / 1000.0f;
    m_raymarchingProgram.setUniformValue("EPSILON",epsilon);
    m_raymarchingProgram.setUniformValue("boxSize",m_boxSize);
    m_raymarchingProgram.setUniformValue("boxPos",m_boxPos);

    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_3D, m_volumeTexture.textureId());

    glEnable(GL_TEXTURE_3D);
    m_environment->volume()->bind();

    Geometry::instance()->bindQuad();

//    int location = m_raymarchingProgram.attributeLocation("vertexPosition");
//    m_raymarchingProgram.enableAttributeArray(location);

//    m_raymarchingProgram.setAttributeBuffer(location,GL_FLOAT,0,3,sizeof(QVector3D));


    GLuint samplerLocation = m_raymarchingProgram.uniformLocation("transferFunction");
    glUniform1i(samplerLocation, 1);
    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_1D, m_transferFunctionTexture.textureId());

    Geometry::instance()->drawQuad();

//    glActiveTexture(GL_TEXTURE0);

    m_environment->volume()->release();

    m_raymarchingProgram.release();
    //BRUH

    m_lightProgram.bind();
    m_lightProgram.setUniformValue("inverseModelViewMatrix",modelViewProjectionMatrix);
    m_lightProgram.setUniformValue("lightPos", m_lightCoords);
    drawSlice();
    m_lightProgram.release();

    int location = m_histogramProgram.attributeLocation("vertexPosition");
    m_histogramProgram.enableAttributeArray(location);
    m_histogramProgram.setAttributeBuffer(location,GL_FLOAT,0,3,sizeof(QVector3D));

    glUseProgram(0);
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


