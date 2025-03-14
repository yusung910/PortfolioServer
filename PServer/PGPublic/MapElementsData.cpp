#include "PGPPrivate.h"
#include "MapElementsData.h"

MapElementsData::MapElementsData(int _layerID, size_t _width, size_t _height)
{
    m_nLayerID = _layerID;
    m_nWidth = _width;
    m_nHeight = _height;

    m_nLeft = int(m_nWidth / 2) * -1;
    m_nLeftBottom = int(m_nHeight / 2) * -1;

    m_nBufferSize = m_nWidth * m_nHeight;

    if (m_nBufferSize > 0)
        m_oLayerData = new int[m_nBufferSize] {0, };

    m_fMinX = m_nLeft * 1.0f;
    m_fMinY = m_nLeftBottom * 1.0f;


    m_fMinY = m_fMinX + m_nWidth;
    m_fMaxY = m_fMaxY + m_nHeight;

}

MapElementsData::~MapElementsData()
{
    SafeDeleteArray(m_oLayerData);
}

int MapElementsData::GetMapElements(const float& _x, const float& _y) const
{
    if (false == _Valid(_x, _y))
        return -1;

    int lCheckX = (int)_x - m_nLeft;
    int lCheckY = (int)_y - m_nLeftBottom;

    int lCheckIndex = lCheckY * (int)m_nWidth + lCheckX;

    return m_oLayerData[lCheckIndex];
}

bool MapElementsData::IsElementsPosistion(const int& _elemID, const float& _x, const float& _y) const
{
    if (false == _Valid(_x, _y))
        return false;

    return _elemID == GetMapElements(_x, _y);
}

const int& MapElementsData::GetLayerID() const
{
    return m_nLayerID;
}

const size_t& MapElementsData::GetWidth() const
{
    return m_nWidth;
}

const size_t& MapElementsData::GetHeight() const
{
    return m_nHeight;
}

const int& MapElementsData::GetLeft() const
{
    return m_nLeft;
}

const int& MapElementsData::GetLeftBottom() const
{
    return m_nLeftBottom;
}

int* MapElementsData::GetLayerData()
{
    return m_oLayerData;
}

bool MapElementsData::_Valid(const float& _x, const float& _y) const
{
    if (nullptr == m_oLayerData)
        return false;

    if (_x < m_fMinX
        || _x >= m_fMaxX
        || _y < m_fMinY
        || _y >= m_fMaxY)
        return false;

    return true;
}
