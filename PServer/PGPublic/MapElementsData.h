#pragma once

/******************************************************************

������ ������ ����ó�� �ǰ��ִ�. y�� ������ �����൵ �ɵ�.

 -y	|
    |
    |
    |
    |
  0	|
    |
    |
    |
    |
 +y	|
    +---------------------------
      -x          0          +x

********************************************************************/


class MapElementsData
{
private:
    int m_nLayerID = 0;
    size_t m_nWidth = 0;
    size_t m_nHeight = 0;

    size_t m_nLeft = 0;
    size_t m_nLeftBottom = 0;

    size_t m_nBufferSize = 0;

    float m_fMinX = 0.0f;
    float m_fMaxX = 0.0f;

    float m_fMinY = 0.0f;
    float m_fMaxY = 0.0f;

    int* m_oLayerData = nullptr;    //Width * Height

public:
    MapElementsData(int _layerID, size_t _width, size_t _height);
    ~MapElementsData();

    int GetMapElements(const float& _x, const float& _y) const;
    bool IsElementsPosistion(const int& _elemID, const float&_x, const float& _y) const;

    const int& GetLayerID() const;
    const size_t& GetWidth() const;
    const size_t& GetHeight() const;
    const int& GetLeft() const;
    const int& GetLeftBottom() const;

    int* GetLayerData();



private:
    bool _Valid(const float& _x, const float& _y) const;
};

