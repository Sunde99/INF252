#include "transferfunctiontexturegenerator.h"

TransferFunctionTextureGenerator::TransferFunctionTextureGenerator(QVector<Node*> nodes, int resolution):
    m_nodes(nodes),
    m_size(resolution+nodes.size())
{
    QVector<float> zero = {0,0,0,0,0};
    QVector<float> one = {1,1,1,1,1};
    QVector<float> half = interpolate(zero,one,0.5);
    //WHEN SUBMITTING CHANGES I  GET A LIST INDEX OUT OF BOUNDS
}

QVector<float> TransferFunctionTextureGenerator::generateTextureData(){
    // CONVERT TO QVector<QVector<float>>
    m_nodeData = QVector<QVector<float>>();
    for (int i=0; i<m_nodes.size(); i++){
        QVector<float> data;
        data.append(m_nodes.at(i)->getPos().x());
        QVector4D info = m_nodes.at(i)->getInfo();
        for (int j=0; j<4; j++){
            data.append(info[j]);
        }
        m_nodeData.append(data);
    }
    // SORT
    sortNodesByXCoord();
    // PLACE MIDDLE VALUES
    placeMiddleValues();

    // CONVERT TO QVector<float> (REMEMBER TO REMOVE X VALUES)
    QVector<float>textureData;
    for (int i=0; i<m_nodeData.size(); i++){
        for (int j=1; j<5; j++){ //SKIPS X VALUES
            textureData.append(m_nodeData[i][j]);
        }
    }
    return textureData;
}

/**
 * @brief TransferFunctionTextureGenerator::compare
 * @param a index of element to compare
 * @param b index of element to compare with
 * @return if x values of element at index a is larger than b
 */
bool TransferFunctionTextureGenerator::compare(int a, int b){
    return m_nodeData[a][0] > m_nodeData[b][0];
}

void TransferFunctionTextureGenerator::sortNodesByXCoord(){
    int i, j;
    bool swapped;
    for (i = 0; i < m_nodes.size()-1; i++){
        swapped = false;
        for (j = 0; j < m_nodes.size()-i-1; j++){
            if (compare(j,j+1)){
                swap(j,j+1);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}


void TransferFunctionTextureGenerator::placeMiddleValues(){
    //MAKE TEMPORARY QVector<QVector<float>>
    QVector<QVector<float>> temp;
    int currNodeIdx = 0;
    //CREATE X0 VALUE
    QVector<float> x0 = {0,0,0,0,0};
    if (m_nodes[0]->x() == .0){
        currNodeIdx++;
    }
    temp.append(x0);
    const int numMiddleValues = m_size-m_nodes.size();

    for (int i=0; i<numMiddleValues; i++){
        // SET X VALUE
        float x = float(i)/float(numMiddleValues);
        // CHECK IF CURRENT X LARGER THAN currNodeIdx.x()
        if (currNodeIdx+1<m_nodeData.size() && x>m_nodeData[currNodeIdx][0]){
            temp.append(m_nodeData[currNodeIdx]);
            currNodeIdx++;
        }
        QVector<float> midVal(5);
        midVal[0] = x;
        //CASE ON CURRNODE = 0
        if (currNodeIdx==0){
            //INTERPOLATE BETWEEN X0 AND m_nodedata[0]
            midVal = interpolate(x0,m_nodeData[0],x);
        }
        else if (currNodeIdx >= m_nodes.size()){ //IF PAST LAST NODE
            midVal = m_nodeData[m_nodes.size()-1];
        }
        else{
            midVal = interpolate(currNodeIdx-1,currNodeIdx,x);
        }
        temp.append(midVal);
    }
    // SET M_NODEDATA TO TEMP
    m_nodeData = temp;
}

float TransferFunctionTextureGenerator::linearInterpolation(float a, float b, float t){
    return a + (b-a)*t;
}


QVector<float> TransferFunctionTextureGenerator::interpolate(QVector<float> node1, QVector<float> node2, float x){
    const float x0 = node1[0];
    const float x1 = node2[0];
    const float t = (x-x0)/(x1-x0);
    return {
        x,
        linearInterpolation(node1[1],node2[1],t),
        linearInterpolation(node1[2],node2[2],t),
        linearInterpolation(node1[3],node2[3],t),
        linearInterpolation(node1[4],node2[4],t),
    };
}

QVector<float> TransferFunctionTextureGenerator::interpolate(int nodeIdx1, int nodeIdx2, float x){
    return interpolate(
        m_nodeData[nodeIdx1],
        m_nodeData[nodeIdx2],
        x
    );
}

void TransferFunctionTextureGenerator::swap(int a, int b){
    m_nodeData.swapItemsAt(a,b);
}
