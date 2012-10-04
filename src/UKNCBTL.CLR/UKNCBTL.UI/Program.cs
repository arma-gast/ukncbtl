using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace UKNCBTL.UI
{
    internal class EmulatorWorker
    {
        bool m_okStopWorker;
        UKNCBTL.Emulator m_Emulator;

        public UKNCBTL.Emulator GetEmulator() { return m_Emulator; }

        public EmulatorWorker()
        {
            m_Emulator = new Emulator();
        }

        public void StopWorker()
        {
            m_okStopWorker = true;
        }

        public void DoWork()
        {
            m_okStopWorker = false;
            while (!m_okStopWorker)
            {
                if (m_Emulator.IsRunning())
                {
                    m_Emulator.SystemFrame();
                    System.Threading.Thread.Sleep(1);
                }
                else
                {
                    System.Threading.Thread.Sleep(50);
                }
            }
        }
    }

    internal static class Program
    {
        private static FormMain m_MainWindow;
        private static EmulatorWorker m_EmulatorWorker;

        public static Emulator GetEmulator() { return m_EmulatorWorker.GetEmulator(); }

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            m_MainWindow = new FormMain();
            m_EmulatorWorker = new EmulatorWorker();
            System.Threading.Thread workerThread = new System.Threading.Thread(m_EmulatorWorker.DoWork);
            workerThread.Priority = System.Threading.ThreadPriority.AboveNormal;
            workerThread.Start();
            
            Application.Run(m_MainWindow);

            m_EmulatorWorker.StopWorker();
            while (!workerThread.IsAlive) ;
        }
    }
}
