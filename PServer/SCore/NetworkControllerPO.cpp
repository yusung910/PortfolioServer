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
    RegisterTimer(0, std::bind(&NetworkControllerPO::_UpdateHost, this));
}

NetworkControllerPO::~NetworkControllerPO()
{
    TerminateThread();
    if (m_hNetworkControl != INVALID_HANDLE_VALUE
        && m_hNetworkControl != NULL)
    {
        WaitForSingleObject(m_hNetworkControl, INFINITE);
        CloseHandle(m_hNetworkControl);
    }

    AutoLock(m_xHostIDLock);
    m_umHostList.clear();
}

bool NetworkControllerPO::CreateThread()
{
    //쓰레드 핸들 생성
    auto lHandle = (HANDLE)_beginthreadex(nullptr, 0, ExcuteThread, this, 0, nullptr);

    if (lHandle == nullptr
        || lHandle == INVALID_HANDLE_VALUE)
    {
        TerminateThread();
        return false;
    }

    m_hNetworkControl = lHandle;


    //대기
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

unsigned int __stdcall NetworkControllerPO::ExcuteThread(void* _arg)
{
    auto& lParent = *(NetworkControllerPO*)_arg;

    lParent.ProcessThread();
    lParent.TerminateThread();
    return 0;
}

bool NetworkControllerPO::PushThread(NetworkContextPO* _ctxt)
{
    if (_ctxt == nullptr)
    {
        VIEW_WRITE_ERROR("NetworkControllerPO::PushThread() Failed - Parameter is nullptr");
        return false;
    }

    m_oMsgQueue.Push(_ctxt);
    return true;
}

int NetworkControllerPO::GetConnectorHostID(const std::string& _ip, int _port)
{
    for (auto it : m_ConnectorHostList)
    {
        if (true == it.IsSame(_ip, _port))
            return it.m_nHostID;
    }
    return 0;
}

bool NetworkControllerPO::SendPacketToHost(const int& _hostID, Packet::SharedPtr _packet)
{
    if (nullptr == _packet)
        return false;

    auto lHost = _FindHost(_hostID);

    if (nullptr == lHost)
        return false;

    return lHost->Waiting(_packet);
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

        auto lIter = m_umHostList.find(_host->GetHostID());
        if (lIter != m_umHostList.end())
        {
            //m_umHostList에 등록되어 있을 경우 제거하고 함수 종료
            VIEW_WRITE_ERROR(L"NetworkControllerPO::_AddHost() - Failed, Duplicated HostID (%d)", _host->GetHostID());
            _host->Close(ESocketCloseType::AddFailHostMap);
            _host->EventClose();
            NetworkManager::GetInst().ReleaseHost(_host);
            return false;
        }

        m_umHostList[_host->GetHostID()] = _host;
    }

    //소켓 생성
    SOCKET lSocket = _host->GetSocket();
    if (lSocket == INVALID_SOCKET)
    {
        // WSASocket 소켓 생성, 관련 옵션 참조
        // https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-wsasocketw
        lSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);

        if (lSocket == INVALID_SOCKET)
        {
            VIEW_WRITE_ERROR(L"NetworkControllerPO::_AddHost() - Failed, WSASocket:%d", WSAGetLastError());
            _host->Close(ESocketCloseType::SocketCreateFail);
            return false;
        }

        _host->SetSocket(lSocket);

    }
    if (NetworkManager::GetInst().RegisterWorker(_host) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::_AddHost() - Failed Bind error");
        _host->Close(ESocketCloseType::RegisterWorkerFail);

        return false;
    }

    //인자값으로 전달받은 NetworkHostPO를 멤버변수 m_ConnectorHostList에 등록한다
    _AddConnectorHost(_host);

    return true;
}

NetworkHostPO* NetworkControllerPO::_FindHost(int _hostID)
{
    AutoLock(m_xHostIDLock);
    auto lIter = m_umHostList.find(_hostID);
    if (lIter == m_umHostList.end())
        return nullptr;
    return lIter->second;
}

void NetworkControllerPO::_UpdateHost()
{
    int64_t lAppTimeMS = Clock::GetTick64();
    AutoLock(m_xHostIDLock);

    auto lIter = m_umHostList.begin();
    while (lIter != m_umHostList.end())
    {
        auto lHost = lIter->second;
        if (lHost)
        {
            if (lHost->IsAlive())
            {
                lHost->Update(lAppTimeMS);
                lIter++;
                continue;
            }

            lHost->EventClose();
            NetworkManager::GetInst().ReleaseHost(lHost);
        }

        _RemoveConnectorHost(lIter->first);

        //m_umHostList에 잇는 값 중 lIter 위치 한 값을 지우고
        //그 다음에 있는 값을 lIter가 가르키게 한다
        lIter = m_umHostList.erase(lIter);
    }
}

void NetworkControllerPO::_AddConnectorHost(NetworkHostPO* _host)
{
    if (nullptr == _host)
        return;

    for (auto& it : m_ConnectorHostList)
    {
        if (it.m_nHostID == _host->GetHostID())
            return;
    }

    ConnectorTargetInfo lInfo;
    lInfo.m_nHostID = _host->GetHostID();
    lInfo.SetIP(_host->GetIP());
    lInfo.m_nPort = _host->GetPeerPort();
    m_ConnectorHostList.push_back(lInfo);
}

void NetworkControllerPO::_RemoveConnectorHost(int _hostID)
{
    for (auto it = m_ConnectorHostList.begin(); it != m_ConnectorHostList.end(); ++it)
    {
        if (it->m_nHostID == _hostID)
        {
            m_ConnectorHostList.erase(it);
            return;
        }
    }
}

void NetworkControllerPO::ProcessThread()
{
    while (false == m_bIsTerminated)
    {
        //Timer 클래스에 RegisterTimer() 함수를 통해 등록된 함수를 실행한다.
        //보통 클래스 생성자에 RegisterTimer()가 존재한다.
        UpdateTimer();

        auto lCtxt = m_oMsgQueue.Pop();
        if (lCtxt == nullptr)
        {
            Sleep(1);
            continue;
        }

        switch (lCtxt->GetContextType())
        {
        case EContextType::Connect: ProcessConnect(*lCtxt);  break;
        case EContextType::Listen:  ProcessListen(*lCtxt);   break;
        case EContextType::Join:    ProcessJoin(*lCtxt);     break;
        case EContextType::Close:   ProcessClose(*lCtxt);     break;
        default:
        {
            VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessThread() Failed - GetContextType:%d", lCtxt->GetContextType());
        }
        break;
        }

        NetworkManager::GetInst().ReleaseContext(lCtxt);
    }
}

void NetworkControllerPO::ProcessConnect(NetworkContextPO& _ctxt)
{
    //인자값으로 전달받은 NetworkContextPO 객체의 데이터를 지역변수에 저장한다
    NetworkHostPO* lHost = nullptr;
    _ctxt.Read(&lHost, sizeof(lHost));

    //NetworkHost 생성
    if (_AddHost(lHost) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessConnect() - Failed : _AddHost()");
        return;
    }

    //NetworkContextPO 초기화
    _ctxt.ResetBuffer();

    //Connect 요청
    lHost->BeginBaseTask();

    if (lHost->Connect(_ctxt) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessConnect() - Failed : NetworkHostPO->Connect()");

        lHost->EndBaseTask(false);
    }
}

void NetworkControllerPO::ProcessListen(NetworkContextPO& _ctxt)
{
    //인자값으로 전달받은 NetworkContextPO 객체의 데이터를 지역변수에 저장한다
    NetworkHostPO* lHost = nullptr;
    _ctxt.Read(&lHost, sizeof(lHost));

    if (_AddHost(lHost) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessListen() - Failed : _AddHost()");
        return;
    }


    //Host Listen 요청
    if (lHost->Listen() == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessListen() - Failed : Host->Listen()");

        lHost->Close(ESocketCloseType::ListenFail);
        return;
    }
}

void NetworkControllerPO::ProcessJoin(NetworkContextPO& _ctxt)
{
    //인자값으로 전달받은 NetworkContextPO 객체의 데이터를 지역변수에 저장한다
    NetworkHostPO* lHost = nullptr;
    _ctxt.Read(&lHost, sizeof(lHost));

    if (_AddHost(lHost) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessJoin() - Failed : _AddHost()");
        return;
    }

    //접속 이벤트 호출
    lHost->EventConnect(EHostType::Acceptor);

    //Context Reset
    _ctxt.ResetBuffer();

    //Receive
    lHost->BeginBaseTask();

    if (lHost->Receive(_ctxt) == false)
    {
        VIEW_WRITE_ERROR(L"NetworkControllerPO::ProcessJoin() - Failed : Receive()");
        lHost->EndBaseTask(false);
        return;
    }
}

void NetworkControllerPO::ProcessClose(NetworkContextPO& _ctxt)
{
    for (auto& hostID : _ctxt.GetHostIDList())
    {
        _RemoveConnectorHost(hostID);
        auto lHost = _FindHost(hostID);

        if (lHost == nullptr) continue;

        lHost->Close(ESocketCloseType::ProcessClose);
    }
}
