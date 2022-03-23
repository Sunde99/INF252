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
    const int sliderValue = m_slidervalue;
    //fix for non-zero min
    const float depth = float(sliderValue)/100.f;
    glBegin(GL_QUADS);
    switch (m_sliderOrientation){
        case 0:
            glVertex2f(-1.f,-1.f);
            glTexCoord3d(
                depth,
                0.0f,
                0.0f
            );

            glVertex2f(1.f,-1.f);
            glTexCoord3d(
                depth,
                .0f,
                1.f
            );

            glVertex2f(1.f,1.f);
            glTexCoord3d(
                depth,
                1.0f,
                1.0f
            );

            glVertex2f(-1.f,1.f);
            glTexCoord3d(
                depth,
                1.0f,
                0.0f
            );
            break;
        case 1:
            glVertex2f(-1.f,-1.f);
            glTexCoord3d(
                0.0f,
                depth,
                0.0f
            );

            glVertex2f(1.f,-1.f);
            glTexCoord3d(
                .0f,
                depth,
                1.f
            );

            glVertex2f(1.f,1.f);
            glTexCoord3d(
                1.0f,
                depth,
                1.0f
            );

            glVertex2f(-1.f,1.f);
            glTexCoord3d(
                1.0f,
                depth,
                0.0f
            );
            break;
        case 2:
            glVertex2f(-1.f,-1.f);
            glTexCoord3d(
                0.0f,
                0.0f,
                depth
            );

            glVertex2f(1.f,-1.f);
            glTexCoord3d(
                .0f,
                1.f,
                depth
            );

            glVertex2f(1.f,1.f);
            glTexCoord3d(
                1.0f,
                1.0f,
                depth
            );

            glVertex2f(-1.f,1.f);
            glTexCoord3d(
                1.0f,
                0.0f,
                depth
            );
        break;
    }
    glEnd();
}

void CrossSectionRenderer::paintGL(){
    glClearColor(1.0f,1.0f,1.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glColor4f(1.0f,1.0f,1.0f,1.0f);

    glEnable(GL_TEXTURE_3D);
    m_environment->volume()->bind();

    drawSlice();
}

QVector3D arcballVector(qreal x, qreal y);
