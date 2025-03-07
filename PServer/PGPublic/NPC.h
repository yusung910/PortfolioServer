#pragma once
#include "BaseObject.h"
#include "PGObject.h"

class NPC : public BaseObject, public PGObject<NPC>
{
private:

    Position m_oAggroDetectedPos; //추적 감지 위치
};

