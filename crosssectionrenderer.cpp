#include "crosssectionrenderer.h"
#include "geometry.h"

CrossSectionRenderer::CrossSectionRenderer(Environment *env, QWidget *parent)
    : QOpenGLWidget{parent}, m_environment(env)
{
    //SLIDER
    QSlider *slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimum(0);
    slider->setMaximum(100);
    slider->setValue(50);
    connect(slider,SIGNAL(valueChanged(int)),this,SLOT(sliderChanged(int)));

    //SLIDER ORIENTATION
    QSlider *sliderOrientation = new QSlider(Qt::Horizontal, this);
    sliderOrientation->setMinimum(0);
    sliderOrientation->setMaximum(2);
    sliderOrientation->setValue(0);
    sliderOrientation->move(150,0);
    connect(sliderOrientation,SIGNAL(valueChanged(int)),this,SLOT(sliderOrientationChanged(int)));
    m_projectionMatrix.setToIdentity();
}

void CrossSectionRenderer::mousePressEvent(QMouseEvent *event){
    update();
}

void CrossSectionRenderer::mouseMoveEvent(QMouseEvent *event){
    update();
}

void CrossSectionRenderer::sliderChanged(int value){
    m_slidervalue = value;
    update();
}

void CrossSectionRenderer::sliderOrientationChanged(int value){
    m_sliderOrientation = value;
    update();
}

void CrossSectionRenderer::initializeGL(){
    initializeOpenGLFunctions();
    Geometry::instance();
    if (!m_crosssectionprogram.addShaderFromSourceFile(QOpenGLShader::Vertex,":/shaders/screen-vs.glsl"))
        qDebug() << "Could not load vertex shader!";

    if (!m_crosssectionprogram.addShaderFromSourceFile(QOpenGLShader::Fragment,":/shaders/image-slice-fs.glsl"))
        qDebug() << "Could not load fragment shader!";

    if (!m_crosssectionprogram.link())
        qDebug() << "Could not link shader program!";
}

void CrossSectionRenderer::resizeGL(int w, int h){
    qreal aspectRatio = qreal(w) / qreal(h);

    qreal nearPlane = 0.5;
    qreal farPlane = 32.0;
    qreal fov = 60.0;

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(fov,aspectRatio,nearPlane,farPlane);
}

void CrossSectionRenderer::drawSlice(){

    glBegin(GL_QUADS);

    glVertex2f(-1.f,-1.f);
    glVertex2f(1.f,-1.f);

    glVertex2f(1.f,1.f);
    glVertex2f(-1.f,1.f);
    glEnd();
}

void CrossSectionRenderer::paintGL(){
    glClearColor(1.0f,0.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glColor4f(1.0f,1.0f,1.0f,1.0f);

    // this matrix converts from volume coordinates ([0,0,0]-[width-1,height-1,depth-1]) to our unit cube coordinates [-1,-1,-1]-[1,1,1]
    QMatrix4x4 volumeToUnitCoordinates;
    volumeToUnitCoordinates.translate(-1.0f,-1.0f,-1.0f);
    volumeToUnitCoordinates.scale(2.0f);
    volumeToUnitCoordinates.scale(1.0f/float(m_environment->volume()->width()),1.0f/float(m_environment->volume()->height()),1.0f/float(m_environment->volume()->depth()));

    QMatrix4x4 modelViewProjectionMatrix = m_projectionMatrix*m_modelViewMatrix*volumeToUnitCoordinates;
    QVector3D blockSize = QVector3D(float(BLOCKDIMENSION),float(BLOCKDIMENSION),float(BLOCKDIMENSION));
    QVector3D volumeSize = QVector3D(float(m_environment->volume()->width()),float(m_environment->volume()->height()),float(m_environment->volume()->depth()));


    m_crosssectionprogram.bind();
    glActiveTexture(GL_TEXTURE0);
    m_crosssectionprogram.setUniformValue("volumeTexture",0);

    glEnable(GL_TEXTURE_3D);
    m_environment->volume()->bind();
    m_crosssectionprogram.setUniformValue("MVP",modelViewProjectionMatrix.inverted());
    m_crosssectionprogram.setUniformValue("volumeSpacing",QVector3D(1,1,1));
    m_crosssectionprogram.setUniformValue("volumeScale",volumeSize);
    const int sliderValue = m_slidervalue;
    //fix for non-zero min
    const float depth = float(sliderValue)/100.f;
    m_crosssectionprogram.setUniformValue("depth",depth);
    m_crosssectionprogram.setUniformValue("orientation",m_sliderOrientation);

    drawSlice();
    m_crosssectionprogram.release();
}

QVector3D arcballVector(qreal x, qreal y);
