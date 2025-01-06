/**
 *  @file PGameConst.h
 *  @author YS
 *  @date 2024-11-25
 *  @project PGPublic
 *
 *  클라이언트와 직접 공유하는게 아니면서 직접 접근으로 값을 사용하는 경우
 *  선언될 상수 클래스
 */
#pragma once
#include <RefSingleton.h>
class PConst : public RefSingleton<PConst>
{

public:
    PConst();
    virtual ~PConst();
};

