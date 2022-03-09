#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <QOpenGLBuffer>

class Geometry
{
protected:
    Geometry();

public:
    static Geometry * instance();

    void bindQuad();
    void drawQuad();

    void bindCube();
    void drawCube();

private:
    QOpenGLBuffer m_quadVertexBuffer;
    QOpenGLBuffer m_quadIndexBuffer;

    QOpenGLBuffer m_cubeVertexBuffer;
    QOpenGLBuffer m_cubeIndexBuffer;
};

#endif // GEOMETRY_H
