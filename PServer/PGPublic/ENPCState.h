#pragma once

enum class ENPCState : int
{
    None = 0,   

    Idle,       //�޽�
    Move,       //�̵�
    Chase,      //����
    Attack,     //����

    ToReturn,   //��ȯ

    PreDead,    //�ױ� ��
    Dead,       //����
    Disappear,  //����� ����

    Hold,       //����(�ܺ� ���⿡ ����)
    Casting,    //���� ��


    Max

};