#include "stdafx.hxx"
#include "NetworkSupporterPO.hxx"

static NetworkSupporterPO G_INST;

NetworkSupporterPO::NetworkSupporterPO()
{
    /*!
     *  WSADATA 구조체
     *  Windows 소켓 구현에 대한 정보가 포함되어 있는 구조체
     *  https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/ns-winsock2-wsadata
     */
    WSADATA localWSA = {};
    /*!
     *  Winsock를 사용하기 위한 시작 함수
     *  https://learn.microsoft.com/ko-kr/windows/win32/api/winsock/nf-winsock-wsastartup
     */
    [[maybe_unused]] int localnRet = WSAStartup(MAKEWORD(2, 2), &localWSA);

    // C28193, localnRet 이 보유한 값을 검사해야 합니다.
    switch (localnRet)
    {
    case S_OK:
        break;
    case WSASYSNOTREADY:		// 네트워크 통신이 준비되지 않은 상태.
    case WSAVERNOTSUPPORTED:	// 이 시스템에서 해당 버전 소켓을 지원하지 않음. (2.2)
    case WSAEINPROGRESS:		// blocking Socket 1.1 작업이 진행중이라고 함...
    case WSAEPROCLIM:			// Windows에서 사용 가능한 소켓 작업 제한에 도달함.
    case WSAEFAULT:				// WSADATA 값이 이상하다고 함. 
        return;
    }
    // end, C28193
}

NetworkSupporterPO::~NetworkSupporterPO()
{
    /*!
     *  Winsock 사용을 종료하는 함수.
     *  https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-wsacleanup
     */
    WSACleanup();
}

void NetworkSupporterPO::GetLocalIPAddress(std::vector<std::string>& _addr)
{
    addrinfo* localpInfos = nullptr;
    addrinfo localHints = {};
    localHints.ai_family = AF_INET;
    localHints.ai_socktype = SOCK_STREAM;
    localHints.ai_protocol = IPPROTO_TCP;

    char localName[256] = {};
    /*!
     * gethostname() 로컬 컴퓨터의 표준 호스트 이름을 검색.
     * https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-gethostname
     */
    if (gethostname(localName, 256) != SOCKET_ERROR)
    {
        /*!
         *  getaddrinfo() ANSI 호스트 이름에서 주소로 프로토콜 독립적 변환을 제공
         *  https://learn.microsoft.com/ko-kr/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo
         */
        getaddrinfo(localName, nullptr, &localHints, &localpInfos);
        if (localpInfos)
        {
            for (addrinfo* ptr = localpInfos; ptr != nullptr ; ptr = ptr->ai_next)
            {
                SOCKADDR_IN localRslt = {};
                memcpy(&localRslt, ptr->ai_addr, ptr->ai_addrlen);

                char localIPStr[INET_ADDRSTRLEN] = {};
                /*!
                 *  inet_ntop() IPv4 또는 IPv6 인터넷 네트워크 주소를 인터넷 표준 형식의 문자열로 변환합니다. 이 함수의 ANSI 버전은 inet_ntop.
                 */
                inet_ntop(localRslt.sin_family, &localRslt.sin_addr, localIPStr, INET_ADDRSTRLEN);
                _addr.push_back(localIPStr);
            }

            /*!
             *  getaddrinfo() 함수가 addrinfo 구조에서 동적으로 할당하는 주소 정보를 해제
             *  https://learn.microsoft.com/ko-kr/windows/win32/api/ws2tcpip/nf-ws2tcpip-freeaddrinfo
             */
            freeaddrinfo(localpInfos);
        }
        else
        {
            _addr.push_back("127.0.0.1");
        }
    }
    else
    {
        _addr.push_back("127.0.0.1");
    }
}

SOCKADDR_IN NetworkSupporterPO::GetAddressInfo(std::string _ip, int _port)
{
    SOCKADDR_IN localRslt = {};
    localRslt.sin_family = AF_INET;

    if (_ip.size() > 0)
    {

        /*!
         *  isalpha() 인자값으로 전달받은 int가 영문자를 나타내는 영문자인지 확인하는 함수
         *  
         */
        if (isalpha(static_cast<int>(_ip[0])))
        {
            addrinfo* localInfos = nullptr;
            addrinfo localHints = {};

            localHints.ai_family = localRslt.sin_family;
            localHints.ai_socktype = SOCK_STREAM;
            localHints.ai_protocol = IPPROTO_TCP;

            getaddrinfo(_ip.c_str(), nullptr, &localHints, &localInfos);
            if (localInfos)
            {
                memcpy(&localRslt, localInfos->ai_addr, localInfos->ai_addrlen);
                freeaddrinfo(localInfos);
            }
            else
            {
                /*!
                 *  inet_pton() 텍스트 프레젠테이션 형식의 IPv4 또는 IPv6 인터넷 네트워크 주소를 숫자 이진 형식으로 변환
                 */
                inet_pton(localRslt.sin_family, "127.0.0.1", &localRslt.sin_addr);
            }
        }
        else
        {
            inet_pton(localRslt.sin_family, _ip.c_str(), &localRslt.sin_addr);
        }
    }
    localRslt.sin_port = htons((unsigned short)_port);
    return localRslt;
}
