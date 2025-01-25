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
    WSADATA lWSA = {};
    /*!
     *  Winsock�� ����ϱ� ���� ���� �Լ�
     *  https://learn.microsoft.com/ko-kr/windows/win32/api/winsock/nf-winsock-wsastartup
     */
    [[maybe_unused]] int lnRet = WSAStartup(MAKEWORD(2, 2), &lWSA);

    // C28193, lnRet �� ������ ���� �˻��ؾ� �մϴ�.
    switch (lnRet)
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
    addrinfo* lpInfos = nullptr;
    addrinfo lHints = {};
    lHints.ai_family = AF_INET;
    lHints.ai_socktype = SOCK_STREAM;
    lHints.ai_protocol = IPPROTO_TCP;

    char lName[256] = {};
    /*!
     * gethostname() ���� ��ǻ���� ǥ�� ȣ��Ʈ �̸��� �˻�.
     * https://learn.microsoft.com/ko-kr/windows/win32/api/winsock2/nf-winsock2-gethostname
     */
    if (gethostname(lName, 256) != SOCKET_ERROR)
    {
        /*!
         *  getaddrinfo() ANSI ȣ��Ʈ �̸����� �ּҷ� �������� ������ ��ȯ�� ����
         *  https://learn.microsoft.com/ko-kr/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo
         */
        getaddrinfo(lName, nullptr, &lHints, &lpInfos);
        if (lpInfos)
        {
            for (addrinfo* ptr = lpInfos; ptr != nullptr; ptr = ptr->ai_next)
            {
                SOCKADDR_IN lRslt = {};
                memcpy(&lRslt, ptr->ai_addr, ptr->ai_addrlen);

                char lIPStr[INET_ADDRSTRLEN] = {};
                /*!
                 *  inet_ntop() IPv4 �Ǵ� IPv6 ���ͳ� ��Ʈ��ũ �ּҸ� ���ͳ� ǥ�� ������ ���ڿ��� ��ȯ�մϴ�. �� �Լ��� ANSI ������ inet_ntop.
                 */
                inet_ntop(lRslt.sin_family, &lRslt.sin_addr, lIPStr, INET_ADDRSTRLEN);
                _addr.push_back(lIPStr);
            }

            /*!
             *  getaddrinfo() �Լ��� addrinfo �������� �������� �Ҵ��ϴ� �ּ� ������ ����
             *  https://learn.microsoft.com/ko-kr/windows/win32/api/ws2tcpip/nf-ws2tcpip-freeaddrinfo
             */
            freeaddrinfo(lpInfos);
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
    SOCKADDR_IN lRslt = {};
    lRslt.sin_family = AF_INET;

    if (_ip.size() > 0)
    {

        /*!
         *  isalpha() ���ڰ����� ���޹��� int�� �����ڸ� ��Ÿ���� ���������� Ȯ���ϴ� �Լ�
         *
         */
        if (isalpha(static_cast<int>(_ip[0])))
        {
            addrinfo* lInfos = nullptr;
            addrinfo lHints = {};

            lHints.ai_family = lRslt.sin_family;
            lHints.ai_socktype = SOCK_STREAM;
            lHints.ai_protocol = IPPROTO_TCP;

            getaddrinfo(_ip.c_str(), nullptr, &lHints, &lInfos);
            if (lInfos)
            {
                memcpy(&lRslt, lInfos->ai_addr, lInfos->ai_addrlen);
                freeaddrinfo(lInfos);
            }
            else
            {
                /*!
                 *  inet_pton() �ؽ�Ʈ ���������̼� ������ IPv4 �Ǵ� IPv6 ���ͳ� ��Ʈ��ũ �ּҸ� ���� ���� �������� ��ȯ
                 */
                inet_pton(lRslt.sin_family, "127.0.0.1", &lRslt.sin_addr);
            }
        }
        else
        {
            inet_pton(lRslt.sin_family, _ip.c_str(), &lRslt.sin_addr);
        }
    }
    lRslt.sin_port = htons((unsigned short)_port);
    return lRslt;
}
