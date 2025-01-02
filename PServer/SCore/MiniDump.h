#pragma once
#include "SCoreAPI.h"

class SCoreAPI MiniDump
{
public:
    MiniDump() = delete;
    ~MiniDump() = delete;

    /*!
     *  덤프파일 생성 Handler
     *      @param [in] _autoRestart If true, auto restart. Otherwise not auto restart.
     */
    static void SetupExceptionHandler(bool _autoRestart = false);
};

