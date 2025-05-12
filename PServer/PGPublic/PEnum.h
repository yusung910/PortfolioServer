#pragma once
enum class EDBResult : int
{
    None = -1,          //���ν��� ȣ�� ����
    Exception = -2,     //���ν��� Exception �߻� (MSSQL ���ν��� Error ����)
    Failure = -1,       //���ν��� ���� ����
    Success = 0,        //���ν��� ���� ����

    //--
    AccountCreateFailed = 1,    //���� ���� ����
    DuplicateLogin = 2,         //���� ���� ����
    PermanentBlock = 3,         //���� ���� ���� 
    DurationBlock = 4,          //���� ���� ���� 

};