/**
 *  @file ServerConst.h
 *  @author YS
 *  @date 2025-01-24
 *  @project PGPublic
 *
 *  DB�� ����� ���� ������ �������� Ŭ����
 */
#pragma once
#include <RefSingleton.h>
#include "EServerConst.h"

class DBService;

class ServerConst : public RefSingleton<ServerConst>
{
private:
    std::map<EServerConst, int> m_mConstIntMap;
    std::map<EServerConst, std::wstring> m_mConstStringMap;


public:
    ServerConst();
    virtual ~ServerConst() = default;

    const int GetInt(const EServerConst& _id) const;
    const std::wstring* GetString(const EServerConst& _id) const;

    bool LoadServerConst(DBService* _service);
    void SetDefault();

private:
    bool _IsIntConst(const EServerConst& _id);
};

