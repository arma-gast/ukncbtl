using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace UKNCBTL.UI
{
    public partial class FormMain : Form
    {
        Bitmap m_ScreenBitmap;

        public FormMain()
        {
            InitializeComponent();

            m_ScreenBitmap = new Bitmap(640, 288, System.Drawing.Imaging.PixelFormat.Format32bppRgb);
            pictureBoxScreen.Image = m_ScreenBitmap;
        }

        private void UpdateScreen()
        {
            Program.GetEmulator().RenderScreen(m_ScreenBitmap);

            //TODO: Looks weird, rework needed
            pictureBoxScreen.Image = m_ScreenBitmap;
            pictureBoxScreen.Update();
        }

        private void toolStripButtonRun_Click(object sender, EventArgs e)
        {
            if (Program.GetEmulator().IsRunning())
            {
                timer1.Stop();
                Program.GetEmulator().Stop();
            }
            else
            {
                Program.GetEmulator().LoadROM("uknc_rom.bin");
                Program.GetEmulator().Start();
                timer1.Start();
            }
        }

        private void toolStripButtonReset_Click(object sender, EventArgs e)
        {
            Program.GetEmulator().Reset();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            UpdateScreen();

            toolStripStatusLabel1.Text = String.Format("{0:0.00}", Program.GetEmulator().GetUptime());
        }
    }
}
