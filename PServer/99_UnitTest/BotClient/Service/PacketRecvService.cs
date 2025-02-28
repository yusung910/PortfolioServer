using BotClient.Network.Util;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace BotClient.Service
{
    public class PacketRecvService
    {
        private ClientSocket m_targetSocket;

        public SocketPacketLogList socketPacketLogList;

        Mutex m_lock = new Mutex(false, "SetClientSocket");

        public void SetClientSocket(ClientSocket _socket)
        {
            m_lock.WaitOne();

            m_targetSocket = _socket;
            m_lock.ReleaseMutex();
        }

        public ClientSocket GetTargetClientSocket()
        {
            return m_targetSocket;
        }

        public PacketRecvService() { }
        ~PacketRecvService()
        {
            m_MainThread.Abort();
        }

        private Thread m_MainThread;

        private bool m_isRunning;

        public bool IsRunning
        {
            get {return m_isRunning;}
            set { m_isRunning = value; }
        }

        public void Start()
        {
            m_MainThread = new Thread(new ThreadStart(Run));
            m_MainThread.Start();
            m_isRunning = true;
        }

        private void Run()
        {
            while (m_isRunning)
            {
                if (m_targetSocket != null)
                {
                    Thread.Sleep(100);
                    socketPacketLogList(m_targetSocket.HostID);
                }
            }
        }
    }
}
