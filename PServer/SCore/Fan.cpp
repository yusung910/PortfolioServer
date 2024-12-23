#include "stdafx.hxx"
#include "Fan.h"
#include "CmnMath.h"
#include <DirectXMath.h>
#include "Random.h"

Fan::Fan()
    : IShape(EShape::Fan)
{

}

Fan::~Fan()
{
    SafeDeleteArray(m_pVertexList);
}
