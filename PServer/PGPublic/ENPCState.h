#pragma once

enum class ENPCState : int
{
    None = 0,   

    Idle,       //휴식
    Move,       //이동
    Chase,      //추적
    Attack,     //공격

    ToReturn,   //귀환

    PreDead,    //죽기 전
    Dead,       //죽음
    Disappear,  //사라진 상태

    Hold,       //고정(외부 영향에 따른)
    Casting,    //시전 중


    Max

};