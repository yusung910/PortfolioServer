#include "stdafx.hxx"
#include "NetworkSupporterPO.hxx"

static NetworkSupporterPO G_INST;

NetworkSupporterPO::NetworkSupporterPO()
{
    /*!
     *  WSADATA ����ü
     *  Windows ���� ������ ���� ������ ���ԵǾ� �ִ� ����ü
     *  https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/ns-winsock2-wsadata
     */
    WSADATA localWSA = {};
    /*!
     *  Winsock�� ����ϱ� ���� ���� �Լ�
     *  https://learn.microsoft.com/ko-kr/windows/win32/api/winsock/nf-winsock-wsastartup
     */
    [[maybe_unused]] int localnRet = WSAStartup(MAKEWORD(2, 2), &localWSA);

    // C28193, localnRet �� ������ ���� �˻��ؾ� �մϴ�.
    switch (localnRet)
    {
    case S_OK:
        break;
    case WSASYSNOTREADY:		// ��Ʈ��ũ ����� �غ���� ���� ����.
    case WSAVERNOTSUPPORTED:	// �� �ý��ۿ��� �ش� ���� ������ �������� ����. (2.2)
    case WSAEINPROGRESS:		// blocking Socket 1.1 �۾��� �������̶�� ��...
    case WSAEPROCLIM:			// Windows���� ��� ������ ���� �۾� ���ѿ� ������.
    case WSAEFAULT:				// WSADATA ���� �̻��ϴٰ� ��. 
        return;
    }
    // end, C28193
}

NetworkSupporterPO::~NetworkSupporterPO()
{
    /*!
     *  Winsock ����� �����ϴ� �Լ�.
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
     * gethostname() ���� ��ǻ���� ǥ�� ȣ��Ʈ �̸��� �˻�.
     * https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-gethostname
     */
    if (gethostname(localName, 256) != SOCKET_ERROR)
    {
        /*!
         *  getaddrinfo() ANSI ȣ��Ʈ �̸����� �ּҷ� �������� ������ ��ȯ�� ����
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
                 *  inet_ntop() IPv4 �Ǵ� IPv6 ���ͳ� ��Ʈ��ũ �ּҸ� ���ͳ� ǥ�� ������ ���ڿ��� ��ȯ�մϴ�. �� �Լ��� ANSI ������ inet_ntop.
                 */
                inet_ntop(localRslt.sin_family, &localRslt.sin_addr, localIPStr, INET_ADDRSTRLEN);
                _addr.push_back(localIPStr);
            }

            /*!
             *  getaddrinfo() �Լ��� addrinfo �������� �������� �Ҵ��ϴ� �ּ� ������ ����
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
         *  isalpha() ���ڰ����� ���޹��� int�� �����ڸ� ��Ÿ���� ���������� Ȯ���ϴ� �Լ�
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
                 *  inet_pton() �ؽ�Ʈ ���������̼� ������ IPv4 �Ǵ� IPv6 ���ͳ� ��Ʈ��ũ �ּҸ� ���� ���� �������� ��ȯ
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
