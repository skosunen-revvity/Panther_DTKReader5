using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Reflection;

using PKI.GSP.Processor.Library;
using PKI.Puncher.CameraWrapper;
using PKI.Puncher.Library;

namespace PKI.Puncher.CameraTestBench {

    public partial class Form1 : Form {

        #region Types

        private delegate void UpdateBloodcardStatusDelegate(string status);
        private UpdateBloodcardStatusDelegate UpdateBloodcardStatus = null;

        private delegate void UpdateBarcodesDelegate();
        private UpdateBarcodesDelegate UpdateBarcodes;

        private delegate void AddErrorDelegate(string desc);
        private AddErrorDelegate AddError;

        #endregion


        #region Private data

        /// <summary>
        /// Viewer which is used when testing drawing surface changing
        /// </summary>
        private ImageViewer m_SwitchedSurfaceDialog = null;

        private IPuncherCamera m_PuncherCamera = null;
        private PunchPositions m_PunchPositions = new PunchPositions();
        private int m_CurrentlySelectedIndex = -1;

        #endregion


        #region Constructors

        public Form1() {
            InitializeComponent();

            UpdateBloodcardStatus = UpdateBloodcardStatusDelegateHandler;
            UpdateBarcodes = UpdateBarcodesHandler;
            AddError = AddErrorHandler;
        }

        #endregion


        #region Event handlers

        private void Form1_Load(object sender, EventArgs e) {

            try {

                lblPunchSimStartCol.BackColor = Color.Yellow;
                lblPunchSimEndCol.BackColor = Color.Green;

                lblPunchSimStartColTxt.BackColor = Color.Black;
                lblPunchSimEndColText.BackColor = Color.White;

                txtPunchSimDuration.Text = "1000";


                txtDSCameraDriver.Text = @"C:\Users\2018691.REVVITY.000\source\Workspaces\OnSpot\SpotLight\Development\Instr\Master\Imager\DSCameraDriver\Debug\DSCameraDriver.dll";
                txtDTKBarcodeReader.Text = @"C:\Users\2018691.REVVITY.000\source\Workspaces\OnSpot\SpotLight\Development\Instr\Master\Imager\DTKBarcodeReader\DTKBarReader5.dll";


                cmdClose.Text = string.Format("E{0}X{0}I{0}T", Environment.NewLine);

                txtDefRadius.Text = "12";
                txtCalibrationImage.Text = "";

                lblSAColor.BackColor = Color.Orange;
                lblBloodMin.BackColor = Color.Black;
                lblBloodMax.BackColor = Color.FromArgb(100, 100, 100);

                chkUseSecondPass.Checked = true;
                lblBloodMin2.BackColor = Color.Black;
                lblBloodMax2.BackColor = Color.FromArgb(130, 130, 130);

                txtHSPercent.Text = "10";
                txtSAPercent.Text = "10";

                txtPDACX.Text = "0";
                txtPDACY.Text = "0";

                chkDrawHotspot.Checked = true;
                chkHSAllInvalid.Checked = false;
                chkBCOLeftToRight.Checked = true;
                lblHotspotColor.BackColor = Color.Yellow;
                txtHSCount.Text = "4";
                txtHSOffsetY.Text = "376";
                txtHSSizeX.Text = "20";
                txtHSSizeY.Text = "20";
                txtHSEmptyMin.Text = "50";
                txtHSEmptyMax.Text = "50";
                txtHSDAX.Text = "0";
                txtHSDAY.Text = "0";

                lblBldColor.BackColor = Color.FromArgb(0, 128, 192);
                chkDrawBlood.Checked = false;

                chkDrawPunchHoles.Checked = true;
                chkDrawSearchArea.Checked = true;
                txtFRI.Text = "50";
                txtFSNL.Text = "2000";

                txtPunchBorder.Text = "2";
                txtSABorderWidth.Text = "1";

                txtPDAX.Text = "1";
                txtPDAY.Text = "1";

                txtIWidth.Text = "752";
                txtIHeight.Text = "480";

                txtSAX.Text = "400";
                txtSAY.Text = "200";
                txtSARadius.Text = "69";

                txtECMin.Text = "50";
                txtECMax.Text = "50";

                txtReqBloodPercent.Text = "10";

                txtBarcodeCount.Text = "2";
                txtBCReadingInterval1.Text = "250";
                txtBCReadingInterval2.Text = "1000";

                lblCardDetectedInfo.Text = "";
                lblCardDetectedInfo.ForeColor = Color.Red;

                cmdRelease.Enabled = false;
                cmdStart.Enabled = false;
                cmdStop.Enabled = false;
                cmdTakePicture.Enabled = false;


                cbPatterns.Items.Add("None (Convert to normal)");
                cbPatterns.Items.Add("Square");
                cbPatterns.Items.Add("Pyramid #1");
                cbPatterns.Items.Add("Pyramid #2");
                cbPatterns.Items.Add("One (center)");
                cbPatterns.SelectedIndex = 0;


                // -----------------------------------------------------------

                txtCWidth.Text = txtIWidth.Text;
                txtCHeight.Text = txtIHeight.Text;

                radCSA_1.Checked = true;

                txtCSAX.Text = "200";
                txtCSAY.Text = "150";

                txtCSAX_2.Text = "400";
                txtCSAY_2.Text = "150";

                txtCSAX_3.Text = "200";
                txtCSAY_3.Text = "300";

                txtCSABorder.Text = "1";

                txtCSARadius.Text = "40";
                txtCSARadius_2.Text = "40";
                txtCSARadius_3.Text = "40";

                cmdCTake1.Enabled = false;
                cmdCTake2.Enabled = false;
                cmdCRelease.Enabled = false;
                cmdCCompare.Enabled = false;

                txtCDOMin.Text = "50";
                txtCDOMax.Text = "50";

                chkCDrawSA.Checked = true;
                chkCCreateResultImage.Checked = true;
                chkCDrawInvalidPixels.Checked = true;
                chkCDrawCenter.Checked = true;

                lblCSAColor.BackColor = Color.Orange;
                lblCInvalidPixelColor.BackColor = Color.FromArgb(0, 128, 192);
                lblCCenterColor.BackColor = Color.Red;

                cmdClose.Select();

                // -----------------------------------------------------------

                txtBDWidth.Text = txtIWidth.Text;
                txtBDHeight.Text = txtIHeight.Text;

                txtBDBOTop.Text = "0";
                txtBDBOLeft.Text = "0";
                txtBDBOBottom.Text = "0";
                txtBDBORight.Text = "0";

                txtBDDAX.Text = "10";
                txtBDDAY.Text = "10";

                lblBDBCMin.BackColor = Color.Gray;
                lblBDBCMax.BackColor = Color.White;

                lblBDRTop.BackColor = Color.Red;
                lblBDRLeft.BackColor = Color.Green;
                lblBDRBottom.BackColor = Color.Blue;
                lblBDRRight.BackColor = Color.Yellow;
                lblBDRRect.BackColor = Color.Orange;

                cmdBDRelease.Enabled = false;
                cmdBDDerect.Enabled = false;

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }

        private void cmdBrowseDSCameraDriver_Click(object sender, EventArgs e) {

            try {

                OpenFileDialog dlg = new OpenFileDialog();
                dlg.FileName = txtDSCameraDriver.Text;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    txtDSCameraDriver.Text = dlg.FileName;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdBrowseDTKBarcodeReader_Click(object sender, EventArgs e) {

            try {

                OpenFileDialog dlg = new OpenFileDialog();
                dlg.FileName = txtDTKBarcodeReader.Text;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    txtDTKBarcodeReader.Text = dlg.FileName;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdBrowseImage_Click(object sender, EventArgs e) {

            try {

                OpenFileDialog dlg = new OpenFileDialog();
                if (dlg.ShowDialog() == DialogResult.OK) {
                    txtShowImagePath.Text = dlg.FileName;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdShowImage_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera != null) {

                    if (txtShowImagePath.Text.Length > 0) {
                        Bitmap bmp = new Bitmap(txtShowImagePath.Text);
                        m_PuncherCamera.DisplayImage(bmp);
                    } else {
                        m_PuncherCamera.DisplayImage(null);
                    }

                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdClearDisplayImage_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera != null) {
                    m_PuncherCamera.DisplayImage(null);
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }


        }
        private void cmdBrowseCI_Click(object sender, EventArgs e) {

            try {

                OpenFileDialog dlg = new OpenFileDialog();
                if (dlg.ShowDialog() == DialogResult.OK) {
                    txtCalibrationImage.Text = dlg.FileName;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cbPatterns_SelectedIndexChanged(object sender, EventArgs e) {

            try {

                bool add = true;
                PunchPositions pp = new PunchPositions();
                PunchPosition p;
                Point center = new Point(Convert.ToInt32(txtSAX.Text), Convert.ToInt32(txtSAY.Text));
                byte padding = Convert.ToByte(txtPunchBorder.Text);
                byte rad = Convert.ToByte(txtDefRadius.Text);

                switch (cbPatterns.SelectedIndex) {

                    case 0:
                        // None
                        add = false;
                        break;


                    case 1:
                        // Square
                        p = new PunchPosition();
                        p.ID = 1;

                        p.BorderColor = Color.Green;
                        p.FillColor = Color.Green;
                        p.TextColor = Color.White;

                        p.Locked = true;
                        p.Position = new Point(center.X - (rad + padding), center.Y - (rad + padding));
                        p.Radius = rad;
                        p.Used = true;
                        pp.Add(p);

                        p = new PunchPosition();
                        p.ID = 2;

                        p.BorderColor = Color.Green;
                        p.FillColor = Color.Green;
                        p.TextColor = Color.White;

                        p.Locked = true;
                        p.Position = new Point(center.X + (rad + padding), center.Y - (rad + padding));
                        p.Radius = rad;
                        p.Used = true;
                        pp.Add(p);

                        p = new PunchPosition();
                        p.ID = 3;

                        p.BorderColor = Color.Green;
                        p.FillColor = Color.Green;
                        p.TextColor = Color.White;

                        p.Locked = true;
                        p.Position = new Point(center.X - (rad + padding), center.Y + (rad + padding));
                        p.Radius = rad;
                        p.Used = true;
                        pp.Add(p);

                        p = new PunchPosition();
                        p.ID = 4;

                        p.BorderColor = Color.Green;
                        p.FillColor = Color.Green;
                        p.TextColor = Color.White;

                        p.Locked = true;
                        p.Position = new Point(center.X + (rad + padding), center.Y + (rad + padding));
                        p.Radius = rad;
                        p.Used = true;
                        pp.Add(p);
                        break;


                    case 2:
                        // Pyramid #1
                        p = new PunchPosition();
                        p.ID = 1;

                        p.BorderColor = Color.Green;
                        p.FillColor = Color.Green;
                        p.TextColor = Color.White;

                        p.Locked = true;
                        p.Position = new Point(center.X, center.Y - (rad + padding));
                        p.Radius = rad;
                        p.Used = true;
                        pp.Add(p);

                        p = new PunchPosition();
                        p.ID = 2;

                        p.BorderColor = Color.Green;
                        p.FillColor = Color.Green;
                        p.TextColor = Color.White;

                        p.Locked = true;
                        p.Position = new Point(center.X - (rad + padding), center.Y + (rad + padding));
                        p.Radius = rad;
                        p.Used = true;
                        pp.Add(p);

                        p = new PunchPosition();
                        p.ID = 3;

                        p.BorderColor = Color.Green;
                        p.FillColor = Color.Green;
                        p.TextColor = Color.White;

                        p.Locked = true;
                        p.Position = new Point(center.X + (rad + padding), center.Y + (rad + padding));
                        p.Radius = rad;
                        p.Used = true;
                        pp.Add(p);
                        break;


                    case 3:
                        // Pyramid #2
                        p = new PunchPosition();
                        p.ID = 1;

                        p.BorderColor = Color.Green;
                        p.FillColor = Color.Green;
                        p.TextColor = Color.White;

                        p.Locked = true;
                        p.Position = new Point(center.X - (rad + padding), center.Y - (rad + padding));
                        p.Radius = rad;
                        p.Used = true;
                        pp.Add(p);

                        p = new PunchPosition();
                        p.ID = 2;

                        p.BorderColor = Color.Green;
                        p.FillColor = Color.Green;
                        p.TextColor = Color.White;

                        p.Locked = true;
                        p.Position = new Point(center.X + (rad + padding), center.Y - (rad + padding));
                        p.Radius = rad;
                        p.Used = true;
                        pp.Add(p);

                        p = new PunchPosition();
                        p.ID = 3;

                        p.BorderColor = Color.Green;
                        p.FillColor = Color.Green;
                        p.TextColor = Color.White;

                        p.Locked = true;
                        p.Position = new Point(center.X, center.Y + (rad + padding));
                        p.Radius = rad;
                        p.Used = true;
                        pp.Add(p);
                        break;


                    case 4:
                        // One (center)
                        p = new PunchPosition();
                        p.ID = 1;

                        p.BorderColor = Color.Green;
                        p.FillColor = Color.Green;
                        p.TextColor = Color.White;

                        p.Locked = true;
                        p.Position = center;
                        p.Radius = rad;
                        p.Used = true;
                        pp.Add(p);
                        break;

                }


                if (add) {
                    punchInfo.SelectedObject = null;
                    m_PunchPositions.Clear();
                    foreach (PunchPosition _p in pp) {
                        m_PunchPositions.Add(_p);
                    }
                } else {
                    foreach (PunchPosition _p in m_PunchPositions) {
                        _p.Locked = false;
                    }
                }

                RefreshList(true);

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdTakePicture_Click(object sender, EventArgs e) {

            try {

                Bitmap bmp = m_PuncherCamera.TakePicture();
                if (bmp != null) {
                    ImageViewer.ShowImage(bmp);
                } else {
                    MessageBox.Show("Failed to take image");
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdReadBarcodesOn_Click(object sender, EventArgs e) {

            try {
                m_PuncherCamera.ReadBarcodes = true;
            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdReadBarcodesOff_Click(object sender, EventArgs e) {

            try {
                m_PuncherCamera.ReadBarcodes = false;
            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdReadBarcodesGet_Click(object sender, EventArgs e) {

            try {
                if (m_PuncherCamera.ReadBarcodes) {
                    lblReadBCState.Text = "ON";
                } else {
                    lblReadBCState.Text = "OFF";
                }
            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdCalibrate_Click(object sender, EventArgs e) {

            try {
                m_PuncherCamera.Calibrate(GetCalibrationImage());
            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdBrowse_Click(object sender, EventArgs e) {

            try {

                OpenFileDialog ofd = new OpenFileDialog();
                ofd.Filter = "Video (*.avi)|*.avi";

                if (ofd.ShowDialog() == DialogResult.OK) {
                    txtFilename.Text = ofd.FileName;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdInitialize_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera == null) {

                    IPuncherCameraSettings camSettings = GetCameraSettings();
                    m_PuncherCamera = GetCamera();
                    m_PuncherCamera.Initialize(camSettings.DrawingSurface, camSettings);

                    lblImage.Location = Point.Empty;
                    lblImage.Size = camSettings.UiImageSize;

                    lblCameraInformation.Text = string.Format("{0}. Resolution: {1} * {2}",
                        camSettings.CameraType, camSettings.CameraImageWidth, camSettings.CameraImageHeight);

                    m_PuncherCamera.PunchPositionCountChanged += m_PuncherCamera_PunchPositionCountChanged;
                    m_PuncherCamera.BloodCardDetected += m_PuncherCamera_BloodCardDetected;
                    m_PuncherCamera.BloodCardRemoved += m_PuncherCamera_BloodCardRemoved;
                    m_PuncherCamera.BarcodesUpdated += m_PuncherCamera_BarcodesUpdated;
                    m_PuncherCamera.CameraDriverExceptionOccured += m_PuncherCamera_CameraDriverExceptionOccured;
                    m_PuncherCamera.FramesSkippedNotification += m_PuncherCamera_FramesSkippedNotification;

                    cmdInitialize.Enabled = false;
                    cmdRelease.Enabled = true;
                    cmdTakePicture.Enabled = true;

                    cmdStart.Enabled = true;
                    cmdStop.Enabled = true;

                    txtIWidth.Enabled = false;
                    txtIHeight.Enabled = false;

                    cmdBrowseDSCameraDriver.Enabled = false;
                    cmdBrowseDTKBarcodeReader.Enabled = false;

                    txtDSCameraDriver.Enabled = false;
                    txtDTKBarcodeReader.Enabled = false;

                    tabControl1.TabPages[1].Enabled = false;
                    tabControl1.TabPages[2].Enabled = false;

                }

            } catch (Exception exc) {
                MessageBox.Show(exc.ToString());

                if (m_PuncherCamera != null) {
                    m_PuncherCamera.Release();
                    m_PuncherCamera = null;
                }


            }

        }
        private void cmdIsCardDetected_Click(object sender, EventArgs e) {

            try {
                lblIsCardDetected.Text = m_PuncherCamera.CardDetected ? "TRUE" : "FALSE";
            } catch (Exception exc) {
                MessageBox.Show(exc.ToString());
            }

        }
        private void cmdRelease_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera != null) {
                    m_PuncherCamera.PunchPositionCountChanged -= m_PuncherCamera_PunchPositionCountChanged;
                    m_PuncherCamera.BloodCardDetected -= m_PuncherCamera_BloodCardDetected;
                    m_PuncherCamera.BloodCardRemoved -= m_PuncherCamera_BloodCardRemoved;
                    m_PuncherCamera.BarcodesUpdated -= m_PuncherCamera_BarcodesUpdated;
                    m_PuncherCamera.CameraDriverExceptionOccured -= m_PuncherCamera_CameraDriverExceptionOccured;
                    m_PuncherCamera.FramesSkippedNotification -= m_PuncherCamera_FramesSkippedNotification;

                    m_PuncherCamera.Release();
                    m_PuncherCamera = null;
                }

                cmdRelease.Enabled = false;
                cmdInitialize.Enabled = true;

                cmdStart.Enabled = false;
                cmdStop.Enabled = false;
                cmdTakePicture.Enabled = false;

                txtIWidth.Enabled = true;
                txtIHeight.Enabled = true;

                cmdBrowseDSCameraDriver.Enabled = true;
                cmdBrowseDTKBarcodeReader.Enabled = true;

                txtDSCameraDriver.Enabled = true;
                txtDTKBarcodeReader.Enabled = true;

                tabControl1.TabPages[1].Enabled = true;
                tabControl1.TabPages[2].Enabled = true;

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdStart_Click(object sender, EventArgs e) {

            try {

                m_PuncherCamera.Start();

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdStop_Click(object sender, EventArgs e) {

            try {

                m_PuncherCamera.Stop();

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdClose_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera != null) {
                    m_PuncherCamera.Release();
                    m_PuncherCamera = null;
                }

                Close();

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblHotspotColor_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblHotspotColor.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblHotspotColor.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBloodMin_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBloodMin.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBloodMin.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBloodMax_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBloodMax.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBloodMax.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBloodMin2_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBloodMin2.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBloodMin2.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBloodMax2_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBloodMax2.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBloodMax2.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblSAColor_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblSAColor.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblSAColor.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBldColor_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBldColor.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBldColor.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdGetPunchPositions_Click(object sender, EventArgs e) {

            try {

                lbPunchPositions.Items.Clear();
                IPunchPositions pp = m_PuncherCamera.PunchPositions;

                m_PunchPositions.Clear();
                m_CurrentlySelectedIndex = -1;
                for (int i = 0; i < pp.Count; i++) {
                    PunchPosition p = (PunchPosition)pp[i];
                    m_PunchPositions.Add(p);
                }

                RefreshList(true);

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdSetPP_Click(object sender, EventArgs e) {

            try {

                m_PuncherCamera.PunchPositions = m_PunchPositions;

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lbPunchPositions_SelectedIndexChanged(object sender, EventArgs e) {

            try {

                if (m_CurrentlySelectedIndex != -1) {
                    if (punchInfo.SelectedObject != null) {
                        m_PunchPositions[m_CurrentlySelectedIndex] = punchInfo.SelectedObject;
                    }
                }

                m_CurrentlySelectedIndex = lbPunchPositions.SelectedIndex;
                if (m_CurrentlySelectedIndex != -1) {
                    punchInfo.SelectedObject = m_PunchPositions[m_CurrentlySelectedIndex];
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdAdd_Click(object sender, EventArgs e) {

            try {

                int firstID = 1;
                foreach (PunchPosition p in m_PunchPositions) {
                    if (p.ID == firstID) {
                        firstID++;
                    } else {
                        break;
                    }
                }

                PunchPosition pos = new PunchPosition();
                pos.FillColor = Color.Yellow;
                pos.BorderColor = Color.Yellow;
                pos.TextColor = Color.Black;

                pos.ID = firstID;
                pos.Used = true;
                pos.Radius = Convert.ToByte(txtDefRadius.Text);
                m_PunchPositions.Add(pos);
                RefreshList(false);

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdDelete_Click(object sender, EventArgs e) {

            try {

                if (lbPunchPositions.SelectedIndex > -1) {
                    m_PunchPositions.RemoveAt(lbPunchPositions.SelectedIndex);
                    punchInfo.SelectedObject = null;
                    m_CurrentlySelectedIndex = -1;
                    RefreshList(true);
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdUpdate_Click(object sender, EventArgs e) {

            try {

                if (lbPunchPositions.SelectedIndex > -1 && punchInfo.SelectedObject != null) {
                    m_PunchPositions[lbPunchPositions.SelectedIndex] = punchInfo.SelectedObject;
                    int id = lbPunchPositions.SelectedIndex;
                    RefreshList(true);
                    lbPunchPositions.SelectedIndex = id;

                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdReadBarcodes_Click(object sender, EventArgs e) {

            try {

                UpdateBarcodesHandler();

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdGetSettings_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera != null) {
                    IPuncherCameraSettings s = m_PuncherCamera.Settings;
                    LoadSettings(s);
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdUpdateSettings_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera != null) {
                    m_PuncherCamera.Settings = GetCameraSettings();
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdClear_Click(object sender, EventArgs e) {

            try {

                ClearSettings();

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdClearErrors_Click(object sender, EventArgs e) {

            try {
                lbErrors.Items.Clear();
            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdSwitchDrawingSurface_Click(object sender, EventArgs e) {

            try {

                if (m_SwitchedSurfaceDialog != null) {
                    m_SwitchedSurfaceDialog.FormClosing -= new FormClosingEventHandler(m_SwitchedSurfaceDialog_FormClosing);
                }

                m_SwitchedSurfaceDialog = new ImageViewer(GetImageSize());
                m_SwitchedSurfaceDialog.FormClosing += new FormClosingEventHandler(m_SwitchedSurfaceDialog_FormClosing);
                m_SwitchedSurfaceDialog.Show();

                m_PuncherCamera.SetDrawingSurface(m_SwitchedSurfaceDialog.GetDrawingSurface());

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }

        private void m_SwitchedSurfaceDialog_FormClosing(object sender, FormClosingEventArgs e) {

            try {

                m_PuncherCamera.SetDrawingSurface(lblImage.Handle);
                m_SwitchedSurfaceDialog.FormClosing -= new FormClosingEventHandler(m_SwitchedSurfaceDialog_FormClosing);
                m_SwitchedSurfaceDialog = null;

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }

        void m_PuncherCamera_BloodCardRemoved() {

            try {

                if (lblCardDetectedInfo.InvokeRequired) {
                    lblCardDetectedInfo.Invoke(UpdateBloodcardStatus, "");
                } else {
                    lblCardDetectedInfo.Text = "";
                }

                if (chkTurnBCROff.Checked) {
                    if (m_PuncherCamera != null) {
                        m_PuncherCamera.ReadBarcodes = false;
                    }

                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        void m_PuncherCamera_BloodCardDetected() {

            try {

                if (lblCardDetectedInfo.InvokeRequired) {
                    lblCardDetectedInfo.Invoke(UpdateBloodcardStatus, "Card detected");
                } else {
                    lblCardDetectedInfo.Text = "Card detected";
                }

                if (chkTurnBCROff.Checked) {
                    if (m_PuncherCamera != null) {
                        m_PuncherCamera.ReadBarcodes = true;
                    }

                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        void m_PuncherCamera_PunchPositionCountChanged() {

            //try {

            //  IPunchPositions pp = m_PuncherCamera.PunchPositions;
            //  int inBlood = 0;
            //  int c = 0;
            //  foreach (IPunchPosition p in pp) {
            //    if (p.Position.X != -1) {
            //      c++;
            //    }

            //    if (p.InBlood) {
            //      inBlood++;
            //    }

            //  }

            //  System.Diagnostics.Debug.Print(string.Format("N {0}, IB {1}", c, inBlood));

            //} catch (Exception exc) {
            //  MessageBox.Show(exc.Message);
            //}

        }
        void m_PuncherCamera_BarcodesUpdated() {

            try {

                if (lbBarcodes.InvokeRequired) {
                    lbBarcodes.Invoke(UpdateBarcodes);
                } else {
                    UpdateBarcodesHandler();
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        void m_PuncherCamera_FramesSkippedNotification() {

            try {

                if (lbErrors.InvokeRequired) {
                    lbErrors.Invoke(AddError, "FRAMES SKIPPED!!!");
                } else {
                    AddErrorHandler("FRAMES SKIPPED!!!");
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        void m_PuncherCamera_CameraDriverExceptionOccured(PuncherCameraException cameraException) {

            try {
                if (lbErrors.InvokeRequired) {
                    lbErrors.Invoke(AddError, cameraException.ToString());
                } else {
                    AddErrorHandler(cameraException.ToString());
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }

        private void cmdCInitialize_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera == null) {
                    IPuncherCameraSettings camSettings = GetCameraSettingsForComparer();

                    m_PuncherCamera = GetCamera();
                    m_PuncherCamera.Initialize(lblLive.Handle, camSettings);
                    m_PuncherCamera.Start();

                    cmdCInitialize.Enabled = false;
                    cmdCRelease.Enabled = true;
                    cmdCTake1.Enabled = true;
                    cmdCTake2.Enabled = true;
                    cmdCCompare.Enabled = true;

                    tabControl1.TabPages[0].Enabled = false;
                    tabControl1.TabPages[2].Enabled = false;

                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdCRelease_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera != null) {

                    m_PuncherCamera.Release();
                    m_PuncherCamera = null;

                    cmdCInitialize.Enabled = true;
                    cmdCRelease.Enabled = false;
                    cmdCTake1.Enabled = false;
                    cmdCTake2.Enabled = false;
                    cmdCCompare.Enabled = false;

                    tabControl1.TabPages[0].Enabled = true;
                    tabControl1.TabPages[2].Enabled = true;

                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdCTake1_Click(object sender, EventArgs e) {

            try {

                Bitmap bmp = m_PuncherCamera.TakePicture();
                if (bmp != null) {
                    picStill1.Image = bmp;
                } else {
                    MessageBox.Show("Failed to take picture");
                }


            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdCTake2_Click(object sender, EventArgs e) {

            try {

                Bitmap bmp = m_PuncherCamera.TakePicture();
                if (bmp != null) {
                    picStill2.Image = bmp;
                } else {
                    MessageBox.Show("Failed to take picture");
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblCSAColor_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblCSAColor.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblCSAColor.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblCInvalidPixelColor_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblCInvalidPixelColor.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblCInvalidPixelColor.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblCCenterColor_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblCCenterColor.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblCCenterColor.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdCCompare_Click(object sender, EventArgs e) {

            try {

                ImageComparerSettings ics;
                ImageComparerResults icr;
                ImageComparer ic = new ImageComparer();

                if (!chkCCompareUsingAll.Checked) {

                    ics = GetImageComparerSettings();
                    icr = ic.Compare(ics);

                    if (icr.Center.X == ImageComparerResults.UNKNOWN_X) {
                        lblCResult.TextAlign = ContentAlignment.MiddleCenter;
                        lblCResult.Text = "SAME";
                    } else {
                        lblCResult.TextAlign = ContentAlignment.MiddleLeft;
                        lblCResult.Text = string.Format("X: {0}{1}Y: {2}{1}Radius: {3}",
                          icr.Center.X, Environment.NewLine, icr.Center.Y, icr.Radius);
                    }

                    picResult.Image = icr.ResultImage;

                } else {

                    Point pt1;
                    Point pt2;
                    Point pt3;

                    int rad1;
                    int rad2;
                    int rad3;

                    radCSA_1.Checked = true;
                    ics = GetImageComparerSettings();
                    icr = ic.Compare(ics);
                    pt1 = icr.Center;
                    rad1 = icr.Radius;

                    radCSA_2.Checked = true;
                    ics = GetImageComparerSettings();
                    icr = ic.Compare(ics);
                    pt2 = icr.Center;
                    rad2 = icr.Radius;

                    radCSA_3.Checked = true;
                    ics = GetImageComparerSettings();
                    icr = ic.Compare(ics);
                    pt3 = icr.Center;
                    rad3 = icr.Radius;

                    lblCResult.TextAlign = ContentAlignment.MiddleLeft;
                    lblCResult.Text = string.Format(
                        "PT 1 (X, Y, R): {0}, {1}, {2}\n" +
                        "PT 2 (X, Y, R): {3}, {4}, {5}\n" +
                        "PT 3 (X, Y, R): {6}, {7}, {8}",
                        pt1.X, pt1.Y, rad1,
                        pt2.X, pt2.Y, rad2,
                        pt3.X, pt3.Y, rad3);





                    radCSA_1.Checked = true;


                }


            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdCUpdateSettings_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera != null) {
                    m_PuncherCamera.Settings = GetCameraSettingsForComparer();
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }

        private void cmdBDInitialize_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera == null) {
                    IPuncherCameraSettings camSettings = new CameraSettings();

                    camSettings.BarcodeReaderLocation = @"C:\SpotLight\Instr\Master\Imager\DTKBarcodeReader\DTKBarReader.dll";
                    camSettings.DriverImplementationFile = @"C:\SpotLight\Common\BIN\DSCameraDriver.dll";

                    camSettings.DrawSearchAreaBorder = false;

                    camSettings.BarcodeCount = 1;

                    m_PuncherCamera = GetCamera();
                    m_PuncherCamera.Initialize(lblBDLive.Handle, camSettings);
                    m_PuncherCamera.Start();


                    cmdBDInitialize.Enabled = false;
                    cmdBDRelease.Enabled = true;
                    cmdBDDerect.Enabled = true;

                    tabControl1.TabPages[0].Enabled = false;
                    tabControl1.TabPages[1].Enabled = false;

                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdBDRelease_Click(object sender, EventArgs e) {

            try {

                if (m_PuncherCamera != null) {

                    m_PuncherCamera.Release();
                    m_PuncherCamera = null;

                    cmdBDInitialize.Enabled = true;
                    cmdBDRelease.Enabled = false;
                    cmdBDDerect.Enabled = false;

                    tabControl1.TabPages[0].Enabled = true;
                    tabControl1.TabPages[1].Enabled = true;

                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdBDDerect_Click(object sender, EventArgs e) {

            try {

                BorderDetectorSettings bds = GetBorderDetectorSettings();
                BorderDetectionResult bdr = BorderDetector.Detect(bds);
                if (bdr.Succeeded) {
                    lblBDResults.Text = string.Format("TOP: {0}\nLeft: {1}\nBottom: {2}\nRight: {3}\n\nWidth: {4}\nHeight: {5}",
                      bdr.Top, bdr.Left, bdr.Bottom, bdr.Right,
                      bdr.Right.X - bdr.Left.X, bdr.Bottom.Y - bdr.Top.Y);

                    if (chkBDShowResults.Checked) {
                        ShowBorderDetectionResults(bds, bdr);

                    }

                } else {
                    lblBDResults.Text = "FAILED";
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBDBCMin_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBDBCMin.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBDBCMin.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBDBCMax_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBDBCMax.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBDBCMax.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBDRTop_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBDRTop.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBDRTop.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBDRLeft_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBDRLeft.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBDRLeft.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBDRBottom_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBDRBottom.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBDRBottom.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBDRRight_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBDRRight.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBDRRight.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblBDRRect_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblBDRRect.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblBDRRect.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }

        private void lblPunchSimStartCol_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblPunchSimStartCol.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblPunchSimStartCol.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblPunchSimEndCol_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblPunchSimEndCol.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblPunchSimEndCol.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblPunchSimStartColTxt_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblPunchSimStartColTxt.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblPunchSimStartColTxt.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void lblPunchSimEndColText_Click(object sender, EventArgs e) {

            try {

                ColorDialog dlg = new ColorDialog();
                dlg.Color = lblPunchSimEndColText.BackColor;

                if (dlg.ShowDialog() == DialogResult.OK) {
                    lblPunchSimEndColText.BackColor = dlg.Color;
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

        }
        private void cmdStartPunchSimulation_Click(object sender, EventArgs e) {

            cmdStartPunchSimulation.Enabled = false;

            try {

                if (m_PuncherCamera != null) {
                    IPunchPositions positions = m_PuncherCamera.PunchPositions;
                    if (positions != null) {

                        PunchPositions forSimulation = new PunchPositions(positions);
                        forSimulation.InPunchProgressMode = true;

                        int foundPositions = 0;
                        foreach (IPunchPosition pos in forSimulation) {
                            if (pos.InBlood) {
                                foundPositions++;
                            }

                            pos.InBlood = false;
                            pos.Locked = true;
                        }

                        if (foundPositions > 0) {

                            m_PuncherCamera.DisplayImage(m_PuncherCamera.TakePicture());

                            // Start simulation
                            int count = 0;
                            int duration = Convert.ToInt32(txtPunchSimDuration.Text);
                            while (count <= foundPositions) {
                                ProcessOnePunchSimulationStep(forSimulation, positions);
                                System.Threading.Thread.Sleep(duration);
                                count++;
                            }

                            m_PuncherCamera.PunchPositions = new PunchPositions();
                            m_PuncherCamera.DisplayImage(null);

                        } else {
                            MessageBox.Show("No positions detected");
                        }
                    }
                }

            } catch (Exception exc) {
                MessageBox.Show(exc.Message);
            }

            cmdStartPunchSimulation.Enabled = true;

        }


        #endregion


        #region Private methods

        private void ProcessOnePunchSimulationStep(IPunchPositions forSimulation, IPunchPositions orginalPositions) {

            int count = forSimulation.Count;
            if(count != orginalPositions.Count) {
                throw new ArgumentException();
            }


            IPunchPosition simP;
            IPunchPosition orgP;

            for (int i = 0; i < count; i++) {

                simP = (IPunchPosition)forSimulation[i];
                if (simP.InBlood) {
                    simP.TextColor = lblPunchSimEndColText.BackColor;
                    simP.FillColor = lblPunchSimEndCol.BackColor;
                } else {

                    orgP = (IPunchPosition)orginalPositions[i];
                    if (orgP.InBlood) {
                        simP = (IPunchPosition)forSimulation[i];
                        simP.InBlood = true;

                        simP.TextColor = lblPunchSimStartColTxt.BackColor;
                        simP.FillColor = lblPunchSimStartCol.BackColor;
                        break;
                    }
                }
            }

            m_PuncherCamera.PunchPositions = forSimulation;

        }

        private IPuncherCameraSettings GetCameraSettingsForComparer() {

            IPuncherCameraSettings cs = new CameraSettings();

            cs.DrawingSurface = lblLive.Handle;
            cs.BarcodeReaderLocation = txtDTKBarcodeReader.Text;
            cs.DriverImplementationFile = txtDSCameraDriver.Text;

            if (radCSA_1.Checked) {
                cs.SearchAreaCenter = new Point(
                  Convert.ToInt32(txtCSAX.Text),
                  Convert.ToInt32(txtCSAY.Text));

                cs.SearchAreaRadius = Convert.ToUInt16(txtCSARadius.Text);
            } else if (radCSA_2.Checked) {
                cs.SearchAreaCenter = new Point(
                  Convert.ToInt32(txtCSAX_2.Text),
                  Convert.ToInt32(txtCSAY_2.Text));

                cs.SearchAreaRadius = Convert.ToUInt16(txtCSARadius_2.Text);
            } else {
                cs.SearchAreaCenter = new Point(
                  Convert.ToInt32(txtCSAX_3.Text),
                  Convert.ToInt32(txtCSAY_3.Text));

                cs.SearchAreaRadius = Convert.ToUInt16(txtCSARadius_3.Text);
            }

            cs.SearchAreaBorderColor = lblCSAColor.BackColor;
            cs.SearchAreaBorderWidth = Convert.ToByte(txtCSABorder.Text);

            cs.BarcodeCount = 1;
            cs.DrawSearchAreaBorder = chkCDrawSA.Checked;

            return cs;

        }

        private IPuncherCameraSettings GetCameraSettings() {

            IPuncherCameraSettings camSettings = new CameraSettings();

            camSettings.SearchAreaAllowedInvalidPixelPercent = Convert.ToByte(txtSAPercent.Text);
            camSettings.HotspotAllowedInvalidPixelPercent = Convert.ToByte(txtHSPercent.Text);

            camSettings.InSimulationMode = chkSimulation.Checked;
            camSettings.SimulationFile = txtFilename.Text;
            camSettings.DrawingSurface = lblImage.Handle;

            camSettings.SearchAreaDetectionAccuracyX = Convert.ToByte(txtPDACX.Text);
            camSettings.SearchAreaDetectionAccuracyY = Convert.ToByte(txtPDACY.Text);

            camSettings.DrawHotspots = chkDrawHotspot.Checked;
            camSettings.AllHotspotsMustBeInvalid = chkHSAllInvalid.Checked;
            camSettings.HotspotColor = lblHotspotColor.BackColor;
            camSettings.HotspotCount = Convert.ToByte(txtHSCount.Text);
            camSettings.HotspotOffsetY = Convert.ToUInt16(txtHSOffsetY.Text);
            camSettings.HotspotSize = new Size(
              Convert.ToInt32(txtHSSizeX.Text),
              Convert.ToInt32(txtHSSizeY.Text));
            camSettings.HotspotDetectionColorMinOffset = Convert.ToInt32(txtHSEmptyMin.Text);
            camSettings.HotspotDetectionColorMaxOffset = Convert.ToInt32(txtHSEmptyMax.Text);
            camSettings.HotspotDetectionAccuracyX = Convert.ToByte(txtHSDAX.Text);
            camSettings.HotspotDetectionAccuracyY = Convert.ToByte(txtHSDAY.Text);

            camSettings.DrawBlood = chkDrawBlood.Checked;
            camSettings.BloodColor = lblBldColor.BackColor;

            camSettings.PunchDetectionAccuracyX = Convert.ToByte(txtPDAX.Text);
            camSettings.PunchDetectionAccuracyY = Convert.ToByte(txtPDAY.Text);
            camSettings.FrameRenderInterval = Convert.ToUInt16(txtFRI.Text);
            camSettings.FramesSkippedNotificationLimit = Convert.ToInt32(txtFSNL.Text);

            camSettings.DrawPunchPositions = chkDrawPunchHoles.Checked;
            camSettings.PunchBorder = Convert.ToByte(txtPunchBorder.Text);

            camSettings.DrawSearchAreaBorder = chkDrawSearchArea.Checked;
            camSettings.SearchAreaBorderColor = lblSAColor.BackColor;
            camSettings.SearchAreaBorderWidth = Convert.ToByte(txtSABorderWidth.Text);

            camSettings.SearchAreaCenter = new Point(
              Convert.ToInt32(txtSAX.Text),
              Convert.ToInt32(txtSAY.Text));

            camSettings.SearchAreaRadius = Convert.ToUInt16(txtSARadius.Text);

            camSettings.BloodColorMin = lblBloodMin.BackColor;
            camSettings.BloodColorMax = lblBloodMax.BackColor;

            camSettings.UseSecondBloodDetection = chkUseSecondPass.Checked;
            camSettings.BloodColorMin2 = lblBloodMin2.BackColor;
            camSettings.BloodColorMax2 = lblBloodMax2.BackColor;

            camSettings.SearchAreaDetectionColorMinOffset = Convert.ToInt32(txtECMin.Text);
            camSettings.SearchAreaDetectionColorMaxOffset = Convert.ToInt32(txtECMax.Text);

            camSettings.CameraConnection = PuncherCameraConnectionType.DirectShowCamera;
            camSettings.BarcodeReader = BarcodeReaderType.DTK;
            camSettings.BarcodeReaderLocation = txtDTKBarcodeReader.Text;

            camSettings.BloodDetection = BloodDetectionMethod.DetectBlood;
            camSettings.RequiredBloodPercent = Convert.ToByte(txtReqBloodPercent.Text);
            camSettings.DriverImplementationFile = txtDSCameraDriver.Text;
            camSettings.InputImage = InputImageType.Color;

            camSettings.BarcodeCount = Convert.ToByte(txtBarcodeCount.Text);
            camSettings.BarcodeReadingInterval1 = Convert.ToUInt16(txtBCReadingInterval1.Text);
            camSettings.BarcodeReadingInterval2 = Convert.ToUInt16(txtBCReadingInterval2.Text);


            byte bcReadDire = 0;
            if (chkBCOLeftToRight.Checked) {
                bcReadDire |= (byte)BarcodeOrientation.LeftToRight;
            }

            if (chkBCORightToLeft.Checked) {
                bcReadDire |= (byte)BarcodeOrientation.RightToLeft;
            }

            if (chkBCOTopToBottom.Checked) {
                bcReadDire |= (byte)BarcodeOrientation.TopToBottom;
            }

            if (chkBCOBottomToTop.Checked) {
                bcReadDire |= (byte)BarcodeOrientation.BottomToTop;
            }

            camSettings.BarcodeReadingDirection = bcReadDire;
            camSettings.CalibrationImage = GetCalibrationImage();

            camSettings.FlipHorizontally = chkFlipHorizontally.Checked;
            camSettings.FlipVertically = chkFlipVertically.Checked;

            return camSettings;

        }

        private ImageComparerSettings GetImageComparerSettings() {

            ImageComparerSettings ics = new ImageComparerSettings();

            ics.DetectionLimitMin = Convert.ToInt32(txtCDOMin.Text);
            ics.DetectionLimitMax = Convert.ToInt32(txtCDOMax.Text);

            ics.Image1 = (Bitmap)picStill1.Image;
            ics.Image2 = (Bitmap)picStill2.Image;


            if (radCSA_1.Checked) {
                ics.SearchAreaCenter = new Point(
                  Convert.ToInt32(txtCSAX.Text),
                  Convert.ToInt32(txtCSAY.Text));

                ics.SearchAreaRadius = Convert.ToUInt16(txtCSARadius.Text);
            } else if (radCSA_2.Checked) {
                ics.SearchAreaCenter = new Point(
                  Convert.ToInt32(txtCSAX_2.Text),
                  Convert.ToInt32(txtCSAY_2.Text));

                ics.SearchAreaRadius = Convert.ToUInt16(txtCSARadius_2.Text);
            } else {
                ics.SearchAreaCenter = new Point(
                  Convert.ToInt32(txtCSAX_3.Text),
                  Convert.ToInt32(txtCSAY_3.Text));

                ics.SearchAreaRadius = Convert.ToUInt16(txtCSARadius_3.Text);
            }

            ics.DrawCenter = chkCDrawCenter.Checked;
            ics.DrawInvalidPixels = chkCDrawInvalidPixels.Checked;
            ics.GenerateResultImage = chkCCreateResultImage.Checked;
            ics.InvalidPixelColor = lblCInvalidPixelColor.BackColor;
            ics.CenterColor = lblCCenterColor.BackColor;

            return ics;

        }
        private BorderDetectorSettings GetBorderDetectorSettings() {

            BorderDetectorSettings bds = new BorderDetectorSettings();

            bds.Image = m_PuncherCamera.TakePicture();

            bds.BorderColorMin = lblBDBCMin.BackColor;
            bds.BorderColorMax = lblBDBCMax.BackColor;

            bds.UseMaxDetectionPointTop = txtBDUMTop.Checked;
            bds.UseMaxDetectionPointLeft = txtBDUMLeft.Checked;
            bds.UseMaxDetectionPointBottom = txtBDUMBottom.Checked;
            bds.UseMaxDetectionPointRight = txtBDUMRight.Checked;

            bds.TopDetectionOffset = Convert.ToInt32(txtBDBOTop.Text);
            bds.LeftDetectionOffset = Convert.ToInt32(txtBDBOLeft.Text);
            bds.BottomDetectionOffset = Convert.ToInt32(txtBDBOBottom.Text);
            bds.RightDetectionOffset = Convert.ToInt32(txtBDBORight.Text);

            bds.HorizontalDetectionAccuracy = Convert.ToInt32(txtBDDAX.Text);
            bds.VerticalDetectionAccuracy = Convert.ToInt32(txtBDDAY.Text);

            return bds;
        }
        private Size GetImageSize() {
            return new Size(752, 480);
        }

        private Bitmap GetCalibrationImage() {

            if (txtCalibrationImage.Text.Length > 0) {
                return new Bitmap(txtCalibrationImage.Text);
            } else {
                return null;
            }

        }
        private void RefreshList(bool selectFist) {

            lbPunchPositions.Items.Clear();
            for (int i = 0; i < m_PunchPositions.Count; i++) {
                PunchPosition p = (PunchPosition)m_PunchPositions[i];
                lbPunchPositions.Items.Add(string.Format("{0}", p.ID));
            }

            if (lbPunchPositions.Items.Count == 0) {
                lbPunchPositions.SelectedIndex = -1;
            } else {

                if (selectFist) {
                    lbPunchPositions.SelectedIndex = 0;
                } else {
                    lbPunchPositions.SelectedIndex = lbPunchPositions.Items.Count - 1;
                }
            }

        }
        private void ClearSettings() {

            chkFlipHorizontally.Checked = false;
            chkFlipVertically.Checked = false;

            txtCalibrationImage.Text = "";

            chkBCOLeftToRight.Checked = false;
            chkBCORightToLeft.Checked = false;
            chkBCOTopToBottom.Checked = false;
            chkBCOBottomToTop.Checked = false;

            lblSAColor.BackColor = BackColor;

            txtPDACX.Text = string.Empty;
            txtPDACY.Text = string.Empty;

            txtSAPercent.Text = string.Empty;
            txtHSPercent.Text = string.Empty;

            chkDrawHotspot.Checked = false;
            chkHSAllInvalid.Checked = false;
            lblHotspotColor.BackColor = BackColor;
            txtHSCount.Text = string.Empty;
            txtHSOffsetY.Text = string.Empty;
            txtHSSizeX.Text = string.Empty;
            txtHSSizeY.Text = string.Empty;
            txtHSEmptyMin.Text = string.Empty;
            txtHSEmptyMax.Text = string.Empty;
            txtHSDAX.Text = string.Empty;
            txtHSDAY.Text = string.Empty;

            chkDrawBlood.Checked = false;
            lblBldColor.BackColor = BackColor;

            chkDrawPunchHoles.Checked = false;
            chkDrawSearchArea.Checked = false;

            txtPunchBorder.Text = string.Empty;
            txtSABorderWidth.Text = string.Empty;
            txtPDAX.Text = string.Empty;
            txtPDAY.Text = string.Empty;
            txtFRI.Text = string.Empty;
            txtFSNL.Text = string.Empty;

            txtIWidth.Text = string.Empty;
            txtIHeight.Text = string.Empty;

            txtSAX.Text = string.Empty;
            txtSAY.Text = string.Empty;
            txtSARadius.Text = string.Empty;

            lblBloodMin.BackColor = BackColor;
            lblBloodMax.BackColor = BackColor;

            chkUseSecondPass.Checked = false;
            lblBloodMin2.BackColor = BackColor;
            lblBloodMax2.BackColor = BackColor;

            txtECMin.Text = string.Empty;
            txtECMax.Text = string.Empty;

            txtReqBloodPercent.Text = string.Empty;

            txtBarcodeCount.Text = string.Empty;
            txtBCReadingInterval1.Text = string.Empty;
            txtBCReadingInterval2.Text = string.Empty;

            lblCardDetectedInfo.Text = "";
            lblCardDetectedInfo.ForeColor = Color.Red;

        }

        private void UpdateBloodcardStatusDelegateHandler(string status) {

            lblCardDetectedInfo.Text = status;
            if (status.Length == 0) {
                lbBarcodes.Items.Clear();
            }


        }
        private void UpdateBarcodesHandler() {

            lbBarcodes.Items.Clear();
            PlateExportImportBase.Barcode barcode = m_PuncherCamera.Barcode;
            if (barcode != null) {
                lbBarcodes.Items.Add(barcode.Code);
            }
        }
        private void AddErrorHandler(string desc) {
            lbErrors.Items.Add(desc);
        }
        private void LoadSettings(IPuncherCameraSettings settings) {

            chkFlipHorizontally.Checked = settings.FlipHorizontally;
            chkFlipVertically.Checked = settings.FlipVertically;

            txtCalibrationImage.Text = "";

            byte tmp = settings.BarcodeReadingDirection;
            chkBCOLeftToRight.Checked = (tmp & (byte)BarcodeOrientation.LeftToRight) == (byte)BarcodeOrientation.LeftToRight;
            chkBCORightToLeft.Checked = (tmp & (byte)BarcodeOrientation.RightToLeft) == (byte)BarcodeOrientation.RightToLeft;
            chkBCOTopToBottom.Checked = (tmp & (byte)BarcodeOrientation.TopToBottom) == (byte)BarcodeOrientation.TopToBottom;
            chkBCOBottomToTop.Checked = (tmp & (byte)BarcodeOrientation.BottomToTop) == (byte)BarcodeOrientation.BottomToTop;

            chkSimulation.Checked = settings.InSimulationMode;
            txtFilename.Text = settings.SimulationFile;

            txtSAPercent.Text = settings.SearchAreaAllowedInvalidPixelPercent.ToString();
            txtHSPercent.Text = settings.HotspotAllowedInvalidPixelPercent.ToString();

            txtPDACX.Text = settings.SearchAreaDetectionAccuracyX.ToString();
            txtPDACY.Text = settings.SearchAreaDetectionAccuracyY.ToString();

            chkDrawHotspot.Checked = settings.DrawHotspots;
            chkHSAllInvalid.Checked = settings.AllHotspotsMustBeInvalid;
            lblHotspotColor.BackColor = settings.HotspotColor;
            txtHSCount.Text = settings.HotspotCount.ToString();
            txtHSOffsetY.Text = settings.HotspotOffsetY.ToString();

            txtHSSizeX.Text = settings.HotspotSize.Width.ToString();
            txtHSSizeY.Text = settings.HotspotSize.Height.ToString();
            txtHSEmptyMin.Text = settings.HotspotDetectionColorMinOffset.ToString();
            txtHSEmptyMax.Text = settings.HotspotDetectionColorMaxOffset.ToString();

            txtHSDAX.Text = settings.HotspotDetectionAccuracyX.ToString();
            txtHSDAY.Text = settings.HotspotDetectionAccuracyY.ToString();

            //settings.DrawingSurface

            chkDrawBlood.Checked = settings.DrawBlood;
            lblBldColor.BackColor = settings.BloodColor;

            txtPDAX.Text = settings.PunchDetectionAccuracyX.ToString();
            txtPDAY.Text = settings.PunchDetectionAccuracyY.ToString();

            chkDrawPunchHoles.Checked = settings.DrawPunchPositions;
            txtPunchBorder.Text = settings.PunchBorder.ToString();

            chkDrawSearchArea.Checked = settings.DrawSearchAreaBorder;
            lblSAColor.BackColor = settings.SearchAreaBorderColor;
            txtSABorderWidth.Text = settings.SearchAreaBorderWidth.ToString();

            txtSAX.Text = settings.SearchAreaCenter.X.ToString();
            txtSAY.Text = settings.SearchAreaCenter.Y.ToString();
            txtFRI.Text = settings.FrameRenderInterval.ToString();
            txtFSNL.Text = settings.FramesSkippedNotificationLimit.ToString();

            txtSARadius.Text = settings.SearchAreaRadius.ToString();

            txtIWidth.Text = txtIWidth.Text;
            txtIHeight.Text = txtIHeight.Text;

            lblBloodMin.BackColor = settings.BloodColorMin;
            lblBloodMax.BackColor = settings.BloodColorMax;

            chkUseSecondPass.Checked = settings.UseSecondBloodDetection;
            lblBloodMin2.BackColor = settings.BloodColorMin2;
            lblBloodMax2.BackColor = settings.BloodColorMax2;

            txtECMin.Text = settings.SearchAreaDetectionColorMinOffset.ToString();
            txtECMax.Text = settings.SearchAreaDetectionColorMaxOffset.ToString();

            txtReqBloodPercent.Text = settings.RequiredBloodPercent.ToString();

            // settings.CameraConnection
            // settings.BarcodeReader
            // settings.BarcodeReaderLocation
            // settings.BloodDetection
            // 
            // settings.DriverImplementationFile
            // settings.InputImage

            txtBarcodeCount.Text = settings.BarcodeCount.ToString();
            txtBCReadingInterval1.Text = settings.BarcodeReadingInterval1.ToString();
            txtBCReadingInterval2.Text = settings.BarcodeReadingInterval2.ToString();

            // settings.PunchSearchPattern = psp;

        }
        private IPuncherCamera GetCamera() {

            bool useDynamicLoading = false;
            if (useDynamicLoading) {

                string cameraImplLocation = Application.StartupPath;
                if (cameraImplLocation[cameraImplLocation.Length - 1] != '\\') {
                    cameraImplLocation += '\\';
                }

                cameraImplLocation += "CameraWrapper.dll";
                Assembly cameraImpl = Assembly.LoadFrom(cameraImplLocation);
                Type[] types = cameraImpl.GetTypes();

                foreach (Type type in types) {
                    if (type.GetInterface("IPuncherCamera") != null) {
                        ConstructorInfo ctorInfo = type.GetConstructor(Type.EmptyTypes);
                        return (IPuncherCamera)ctorInfo.Invoke(null);
                    }
                }
            } else {
                return new Camera();
            }

            return null;
        }

        private void ShowBorderDetectionResults(BorderDetectorSettings settings, BorderDetectionResult bdr) {

            int y;
            int x;
            int i;
            int v;

            Bitmap bmp = settings.Image;

            // Top
            for (i = 0; i < bdr.TopBorder.Count; i++) {
                v = bdr.TopBorder[i].Y;
                x = bdr.TopBorder[i].X;
                for (y = settings.TopDetectionOffset; y <= v; y++) {
                    bmp.SetPixel(x, y, lblBDRTop.BackColor);
                }
            }

            // Left
            for (i = 0; i < bdr.LeftBorder.Count; i++) {
                v = bdr.LeftBorder[i].X;
                y = bdr.LeftBorder[i].Y;

                for (x = settings.LeftDetectionOffset; x <= v; x++) {
                    bmp.SetPixel(x, y, lblBDRLeft.BackColor);
                }
            }

            // Bottom
            for (i = 0; i < bdr.BottomBorder.Count; i++) {
                v = bdr.BottomBorder[i].Y;
                x = bdr.BottomBorder[i].X;

                for (y = settings.Image.Height - settings.BottomDetectionOffset - 1; y >= v; y--) {
                    bmp.SetPixel(x, y, lblBDRBottom.BackColor);
                }
            }

            // Right
            for (i = 0; i < bdr.RightBorder.Count; i++) {
                v = bdr.RightBorder[i].X;
                y = bdr.RightBorder[i].Y;

                for (x = settings.Image.Width - settings.RightDetectionOffset - 1; x >= v; x--) {
                    bmp.SetPixel(x, y, lblBDRRight.BackColor);
                }
            }


            for (x = bdr.Left.X; x < bdr.Right.X; x++) {
                bmp.SetPixel(x, bdr.Top.Y, lblBDRRect.BackColor);
                bmp.SetPixel(x, bdr.Bottom.Y, lblBDRRect.BackColor);
            }

            for (y = bdr.Top.Y; y < bdr.Bottom.Y; y++) {
                bmp.SetPixel(bdr.Left.X, y, lblBDRRect.BackColor);
                bmp.SetPixel(bdr.Right.X, y, lblBDRRect.BackColor);
            }

            ImageViewer.ShowImage(bmp);

        }

        #endregion

    }

}