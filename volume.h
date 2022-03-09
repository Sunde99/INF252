#ifndef VOLUME_H
#define VOLUME_H

#include <QObject>
#include <QVector>
#include <QOpenGLTexture>
#include <QOpenGLExtraFunctions>
#include <QVector3D>
#include <limits>

const int BLOCKDIMENSION = (1 << 4); // = 2^4
const int BLOCKSIZE = BLOCKDIMENSION*BLOCKDIMENSION*BLOCKDIMENSION;

class Volume;

class VolumeNode
{
public:
    VolumeNode(const Volume & volume, int xMin, int yMin, int zMin);

    float minimum() const
    {
        return m_minimum;
    };

    float maximum() const
    {
        return m_maximum;
    };

private:
    float m_minimum;
    float m_maximum;
};

class Volume : public QObject, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    explicit Volume(QObject *parent = nullptr);
    bool load(const QString &fileName);

    void bind();
    void release();

    int width() const
    {
        return m_width;
    };

    int height() const
    {
        return m_height;
    };

    int depth() const
    {
        return m_depth;
    };

    float value(int x, int y, int z) const
    {
        int index = x+y*m_width+z*m_width*m_height;
        float volumeValue = float(m_volumeData.at(index)) / std::numeric_limits<unsigned short>::max(); // return 65535
        return volumeValue;
    };

    const QVector<QVector3D> visibleBlockPositions() const
    {
        return m_visibleBlockPositions;
    };

signals:

private:


    void buildTree();
    void classifyTree(const QVector<float> & summedAreaTable);

    QVector<unsigned short> m_volumeData;
    int m_width, m_height, m_depth;

    QOpenGLTexture m_volumeTexture;
    bool m_updateNeeded;

    int m_xBlockCount, m_yBlockCount, m_zBlockCount;
    QVector<VolumeNode> m_blocks;
    QVector<bool> m_blocksVisible;
    bool m_blockPositionUpdateNeeded;
    QVector<QVector3D> m_visibleBlockPositions;
};

#endif // VOLUME_H
