using System;
using System.Drawing;
using System.Windows.Forms;

namespace PKI.Puncher.CameraTestBench {
 
    public partial class ImageViewer : Form {

        public ImageViewer() {
            InitializeComponent();
            lblDrawingSurface.Visible = false;
        }
        public ImageViewer(Size imageSize) {
            InitializeComponent();

            pictureBox1.Visible = false;
            ResizeControls(imageSize);

            cmdBrowse.Visible = false;
            label1.Visible = false;
            txtFilename.Visible = false;
            cmdSave.Visible = false;

        }

        public IntPtr GetDrawingSurface() {
            return lblDrawingSurface.Handle;
        }

        public static void ShowImage(Bitmap bmp) {

            ImageViewer iv = new ImageViewer();
            iv.ResizeControls(bmp.Size);
            iv.pictureBox1.Image = bmp;

            iv.lblDrawingSurface.Visible = false;

            iv.Show();

        }

        private void ResizeControls(Size imageSize) {

            ClientSize = new Size(
              imageSize.Width + (2 * pictureBox1.Left),
              imageSize.Height + cmdClose.Height + (3 * pictureBox1.Top));

            cmdClose.Left = pictureBox1.Left + imageSize.Width - cmdClose.Width;

            int t = (2 * pictureBox1.Top) + imageSize.Height;
            cmdClose.Top = t;

            cmdBrowse.Top = t;
            cmdSave.Top = t;
            txtFilename.Top = t;
            label1.Top = t;

            lblDrawingSurface.Location = pictureBox1.Location;
            lblDrawingSurface.Size = imageSize;

        }

        private void cmdClose_Click(object sender, EventArgs e) {
            Close();
        }
        private void cmdBrowse_Click(object sender, EventArgs e) {

            try {
                SaveFileDialog dlg = new SaveFileDialog();
                if (dlg.ShowDialog() == DialogResult.OK) {
                    txtFilename.Text = dlg.FileName;
                }
            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdSave_Click(object sender, EventArgs e) {

            try {
                if (txtFilename.Text.Length == 0) {
                    MessageBox.Show("Please specify the filename");
                } else {
                    pictureBox1.Image.Save(txtFilename.Text);
                }
            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }

    }

}