#include "stdafx.hxx"
#include "NetworkControllerPO.hxx"

#include "NetworkManager.h"
#include "NetworkContextPO.hxx"
#include "NetworkHostPO.hxx"

#include "Clock.h"

NetworkControllerPO::NetworkControllerPO()
    : m_bIsTerminated(false)
    , m_hNetworkControl(INVALID_HANDLE_VALUE)
{
    
}

NetworkControllerPO::~NetworkControllerPO()
{
}

bool NetworkControllerPO::CreateThread()
{
    //������ �ڵ� ����
    auto localHandle = (HANDLE)_beginthreadex(nullptr, 0, ExcuteThread, this, 0, nullptr);
    
    if (localHandle == nullptr
        || localHandle == INVALID_HANDLE_VALUE)
    {
        TerminateThread();
        return false;
    }

    m_hNetworkControl = localHandle;
    

    //���
    Sleep(100);

    if (true == m_bIsTerminated)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::CreateThread() - Failed");
        return false;
    }

    return true;
}

void NetworkControllerPO::TerminateThread()
{
    m_bIsTerminated = true;
}

bool NetworkControllerPO::_AddHost(NetworkHostPO* _host)
{
    if (_host == nullptr)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::_AddHost() - Failed, Parameter is nullptr");

        return false;
    }

    {
        AutoLock(m_xHostIDLock);

        auto localIter = m_oHostList.find(_host->GetHostID());
        if (localIter != m_oHostList.end())
        {
            //m_oHostList�� ��ϵǾ� ���� ��� �����ϰ� �Լ� ����
            VIEW_WRITE_ERROR(L"NetworkControllerPO::_AddHost() - Failed, Duplicated HostID (%d)", _host->GetHostID());
            _host->Close(ESocketCloseType::AddFailHostMap);
            _host->EventClose();
            NetworkManager::GetInst().ReleaseHost(_host);
            return false;
        }

        m_oHostList[_host->GetHostID()] = _host;
    }

    //���� ����
    SOCKET localSocket = _host->GetSocket();
    if (localSocket == INVALID_SOCKET)
    {
        // WSASocket ���� ����, ���� �ɼ� ����
        // https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-wsasocketw
        localSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

        if (localSocket == INVALID_SOCKET)
        {
            VIEW_WRITE_ERROR(L"NetworkControllerPO::_AddHost() - Failed, WSASocket:%d", WSAGetLastError());
            _host->Close(ESocketCloseType::SocketCreateFail);
            return false;
        }

        _host->SetSocket(localSocket);

        //if(NetworkManager::GetInst())
    }


    return true;
}
