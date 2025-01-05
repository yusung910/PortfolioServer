#pragma once

enum class ELoginState
{
    None = 0,
    PlatformAuthorize = 1,
    DBProcess = 2,

    DuplicateKick = 3,
    WaitingQueue = 4,
    Done,
    Max,
};