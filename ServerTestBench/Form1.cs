using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

using ServerTestBench.Properties;
using PKI.Puncher;

namespace ServerTestBench {

    public partial class Form1 : Form {

        private SpotLightServer m_Server = null;

        public Form1() {

            InitializeComponent();

            String type = Settings.Default.InstrumentType;
            m_Server = new SpotLightServer(type);

        }

        private void cmdInitialize_Click(object sender, EventArgs e) {

            try {

                //m_Server.ImagePort = lblImage.Handle;
                //m_Server.StartUp();

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
    }
}
