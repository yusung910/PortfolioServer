#pragma once
enum class EDBResult : int
{
    None = -1,          //프로시저 호출 에러
    Exception = -2,     //프로시저 Exception 발생 (MSSQL 프로시저 Error 참조)
    Failure = -1,       //프로시저 실행 실패
    Success = 0,        //프로시저 실행 성공

    //--
    AccountCreateFailed = 1,    //계정 생성 실패
    DuplicateLogin = 2,         //계정 이중 접속
    PermanentBlock = 3,         //계정 영구 정지 
    DurationBlock = 4,          //계정 영구 정지 

};