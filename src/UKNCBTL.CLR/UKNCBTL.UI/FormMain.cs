using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace UKNCBTL.UI
{
    public partial class FormMain : Form
    {
        UKNCBTL.Emulator m_Emulator;
        Bitmap m_ScreenBitmap;

        public FormMain()
        {
            InitializeComponent();

            m_ScreenBitmap = new Bitmap(640, 288, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
            pictureBoxScreen.Image = m_ScreenBitmap;

            m_Emulator = new Emulator();
        }

        private void UpdateScreen()
        {
            m_Emulator.RenderScreen(m_ScreenBitmap);

            //TODO: Looks weird, rework needed
            pictureBoxScreen.Image = m_ScreenBitmap;
            pictureBoxScreen.Update();
        }

        private void toolStripButtonRun_Click(object sender, EventArgs e)
        {
            if (m_Emulator.IsRunning())
            {
                timer1.Stop();
                m_Emulator.Stop();
            }
            else
            {
                m_Emulator.LoadROM("uknc_rom.bin");
                m_Emulator.Start();
                timer1.Start();
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (!m_Emulator.IsRunning()) return;

            m_Emulator.SystemFrame();

            UpdateScreen();
        }

        private void toolStripButtonReset_Click(object sender, EventArgs e)
        {
            m_Emulator.Reset();
        }
    }
}
