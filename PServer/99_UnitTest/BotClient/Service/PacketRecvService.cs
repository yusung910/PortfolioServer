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
        ClientSocket m_targetSocket;

        Mutex m_lock = new Mutex(false, "SetClientSocket");

        public void SetClientSocket(ClientSocket _socket)
        {
            m_lock.WaitOne();

            m_targetSocket = _socket;

            m_lock.ReleaseMutex();
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
                Thread.Sleep(100);
                Console.WriteLine("set Packet LogData");
            }
        }
    }
}
