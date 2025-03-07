#pragma once

enum class EMachineState : int
{
    None = 0,   

    Idle,
    Move,
    Chase,
    Attack,

    ToReturn,

    Dead,
    Disappear,

    Max

};