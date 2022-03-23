#include "volume.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <iostream>

VolumeNode::VolumeNode(const Volume & volume, int xMin, int yMin, int zMin)
{
    int xMax = std::min(xMin+BLOCKDIMENSION,volume.width())-1;
    int yMax = std::min(yMin+BLOCKDIMENSION,volume.height())-1;
    int zMax = std::min(zMin+BLOCKDIMENSION,volume.depth())-1;

    float minimumValue = 1.0f;
    float maximumValue = 0.0f;

    for (int z=zMin;z<=zMax;z++)
    {
        for (int y=yMin;y<=yMax;y++)
        {
            for (int x=xMin;x<=xMax;x++)
            {
                float volumeValue = volume.value(x,y,z);
                minimumValue = std::min(volumeValue,minimumValue);
                maximumValue = std::max(volumeValue,maximumValue);
            }
        }
    }

    m_minimum = minimumValue;
    m_maximum = maximumValue;
}

Volume::Volume(QObject *parent) : QObject(parent), m_width(0), m_height(0), m_depth(0), m_volumeTexture(QOpenGLTexture::Target3D), m_updateNeeded(false), m_blockPositionUpdateNeeded(false)
{
}

bool Volume::load(const QString &fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to open " << fileName << "!";
        return false;
    }

    QDataStream stream(&file);
    stream.setByteOrder(QDataStream::LittleEndian);

    unsigned short width = 0, height = 0, depth = 0;
    stream >> width >> height >> depth;

    qDebug() << "Width:" << width;
    qDebug() << "Height:" << height;
    qDebug() << "Depth:" << depth;

    int volumeSize = int(width)*int(height)*int(depth);
    m_volumeData.resize(volumeSize);

    if (stream.readRawData(reinterpret_cast<char*>(m_volumeData.data()),volumeSize*sizeof(unsigned short)) != volumeSize*sizeof(unsigned short))
        return false;

    m_width = int(width);
    m_height = int(height);
    m_depth = int(depth);

    for (int i=0;i<volumeSize;i++)
    {
        m_volumeData[i] *= 16;
    }

    m_updateNeeded = true;


    buildTree();

    QVector<float> opacityTable(512,0.0f);

    for (int i=128;i<opacityTable.size();i++)
        opacityTable[i] = 1.0f;

    // here is how we can compute a 1D summed area table
    // from a opacity lookup table of alpha values
    QVector<float> summedAreaTable(opacityTable.size(),0.0f);

    summedAreaTable[0] = opacityTable[0];

    for (int i=1;i<opacityTable.size();i++)
    {
        summedAreaTable[i] = opacityTable[i]+summedAreaTable[i-1];
    }


    // normally, we would call this whenever the transfer function has changed
    classifyTree(summedAreaTable);

    return true;
}

void Volume::bind()
{
    initializeOpenGLFunctions();

    if (m_updateNeeded)
    {
        if (m_volumeTexture.isCreated())
            m_volumeTexture.destroy();

        m_volumeTexture.setBorderColor(0,0,0,0);
        m_volumeTexture.setWrapMode(QOpenGLTexture::ClampToBorder);
        m_volumeTexture.setFormat(QOpenGLTexture::R32F);
        m_volumeTexture.setMinificationFilter(QOpenGLTexture::Linear);
        m_volumeTexture.setMagnificationFilter(QOpenGLTexture::Linear);
        m_volumeTexture.setAutoMipMapGenerationEnabled(false);
        m_volumeTexture.setSize(m_width,m_height,m_depth);
        m_volumeTexture.allocateStorage();

        void *data = reinterpret_cast<void*>(m_volumeData.data());
        m_volumeTexture.setData(0,0,0,m_width,m_height,m_depth,QOpenGLTexture::Red,QOpenGLTexture::UInt16,data);
        m_updateNeeded = false;

    }

    if (m_volumeTexture.isCreated())
    {
        m_volumeTexture.bind();
    }
}

void Volume::release()
{
    if (m_volumeTexture.isCreated())
        m_volumeTexture.release();
}

void Volume::buildTree()
{
    // here we build the first (bottom) level of our octree from fixed-size blocks
    m_blocks.clear();

    // compute the number of blocks in each dimensions (useful for some operations on the block grid)
    // we are rounding up since the volume dimensions may not be a multiple of the block size
    m_xBlockCount = int(ceilf(float(m_width) / float(BLOCKDIMENSION)));
    m_yBlockCount = int(ceilf(float(m_height) / float(BLOCKDIMENSION)));
    m_zBlockCount = int(ceilf(float(m_depth) / float(BLOCKDIMENSION)));

    for (int z=0;z<m_depth;z+=BLOCKDIMENSION)
    {
        for (int y=0;y<m_height;y+=BLOCKDIMENSION)
        {
            for (int x=0;x<m_width;x+=BLOCKDIMENSION)
            {
                m_blocks.push_back(VolumeNode(*this,x,y,z));
                //std::cout << "Block " << x << "," << y << "," << z << " : " << m_blocks.back().minimum() << " -- " << m_blocks.back().maximum() << std::endl;
            }
        }
    }

    // reset visibility state of all blocks to true
    m_blocksVisible.fill(true,m_blocks.size());

    // our tree was rebuilt, so our position arrayneeds to be updated
    m_blockPositionUpdateNeeded = true;
}

void Volume::classifyTree(const QVector<float> & summedAreaTable)
{
    int visibleBlockCount = 0;

    for (int i=0;i<m_blocks.size();i++)
    {
        int minimumIndex = int(m_blocks[i].minimum()*float(summedAreaTable.size()-1));
        int maximumIndex = int(m_blocks[i].maximum()*float(summedAreaTable.size()-1));
        float opacitySum = summedAreaTable.at(maximumIndex)-summedAreaTable.at(minimumIndex);

        bool wasVisible = m_blocksVisible[i];

        if (opacitySum == 0.0f)
        {
            m_blocksVisible[i] = false;
        }
        else
        {
            m_blocksVisible[i] = true;
            visibleBlockCount++;
        }

        // we only need to update the geometry if the visibility of any block has changed
        if (wasVisible != m_blocksVisible[i])
            m_blockPositionUpdateNeeded = true;
    }

    if (m_blockPositionUpdateNeeded)
    {
        m_visibleBlockPositions.resize(visibleBlockCount);
        int blockPositionIndex = 0;

        for (int i=0;i<m_blocks.size();i++)
        {
            if (m_blocksVisible[i])
            {
                // here is how to compute node's block coordinates from its index
                int blockZ = i / (m_xBlockCount*m_yBlockCount);
                int blockY = (i - blockZ*m_xBlockCount*m_yBlockCount) / m_xBlockCount;
                int blockX = i - blockZ*m_xBlockCount*m_yBlockCount - blockY*m_xBlockCount;

                // to convert to volume grid coordinates, we multily by the block dimension
                int volumeX = blockX*BLOCKDIMENSION;
                int volumeY = blockY*BLOCKDIMENSION;
                int volumeZ = blockZ*BLOCKDIMENSION;

                //std::cout << "Block " << volumeX << "," << volumeY << "," << volumeZ << " : " << m_blocksVisible[i] << std::endl;
                m_visibleBlockPositions[blockPositionIndex] = QVector3D(volumeX,volumeY,volumeZ);
                blockPositionIndex++;
            }
        }

        m_blockPositionUpdateNeeded = false;
        std::cout << "Total Blocks: " << m_blocks.size() << std::endl;
        std::cout << "Visible Blocks: " << m_visibleBlockPositions.size() << std::endl;
    }
}
