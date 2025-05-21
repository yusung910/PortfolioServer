#pragma once

struct TileLocation
{
    int m_nCol = 0;
    int m_nRow = 0;

    void Reset()
    {
        m_nCol = 0;
        m_nRow = 0;
    }

    void Set(const int& _c, const int& _r)
    {
        m_nCol = _c;
        m_nRow = _r;
    }

    int TotalGridCount()
    {
        return m_nCol * m_nRow;
    }
};
