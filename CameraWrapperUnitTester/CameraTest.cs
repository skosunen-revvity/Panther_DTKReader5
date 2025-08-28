using PKI.Puncher.CameraWrapper;
using PKI.Puncher.Library;
using Microsoft.VisualStudio.TestTools.UnitTesting;

using System;
using System.Windows.Forms;
using System.Drawing;
using System.Threading;

using System.IO;
using System.Diagnostics;
using System.Runtime.InteropServices;

namespace CameraWrapperUnitTester {

    /// <summary>
    ///This is a test class for CameraTest and is intended
    ///to contain all CameraTest Unit Tests
    ///</summary>
    [TestClass()]
    public class CameraTest {

        [DllImport("user32.dll", SetLastError = true)]
        static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        static extern IntPtr SendMessage(IntPtr hWnd, UInt32 Msg, IntPtr wParam, IntPtr lParam);

        private const UInt32 WM_CLOSE = 0x0010;


        private static IntPtr m_Handle = IntPtr.Zero;

        /// <summary>
        ///Gets or sets the test context which provides
        ///information about and functionality for the current test run.
        ///</summary>
        public TestContext TestContext {
            get {
                return testContextInstance;
            }
            set {
                testContextInstance = value;
            }
        }

        #region Additional test attributes
        // 
        //You can use the following additional attributes as you write your tests:
        //
        //Use ClassInitialize to run code before running the first test in the class
        //
        //Use TestInitialize to run code before running each test
        //[TestInitialize()]
        //public void MyTestInitialize()
        //{
        //}
        //
        //Use TestCleanup to run code after each test has run
        //[TestCleanup()]
        //public void MyTestCleanup()
        //{
        //}
        //
        #endregion


        /// <summary>
        /// Called by the test system before first test is excuted
        /// </summary>
        [ClassInitialize()]
        public static void UnitTestConstructor(TestContext testContext) {

        }


        /// <summary>
        /// Called by the test system after last test was executed
        /// </summary>
        [ClassCleanup()]
        public static void UnitTestDestructor() {

            try {

                if (m_camera != null) {
                    m_camera.Release();

                }

                m_camera = null;

                if (m_Handle != IntPtr.Zero) {
                    SendMessage(m_Handle, WM_CLOSE, IntPtr.Zero, IntPtr.Zero);
                }

                Thread.Sleep(2000);


            } catch (Exception) {
            }

        }


        #region Tests

        /// <summary>
        /// Test that will try to create the camera object
        /// </summary>
        [TestMethod()]
        public void CreateCameraTest() {

            Assert.IsNull(m_camera);

            try {
                m_camera = new Camera();
            } catch (Exception exc) {
                Assert.Fail("Failed to create Camera. Desc: {0} CS: {1}", exc.Message, exc.StackTrace);
            }

        }

        /// <summary>
        /// Test that will try to initializa the camera
        ///</summary>
        [TestMethod()]
        public void InitializeTest() {

            Assert.IsNotNull(m_camera);

            try {

                m_Handle = GetDrawingSurface();
                if (m_Handle == IntPtr.Zero) {
                    Assert.Fail("Failed to get drawing surface");
                }

                IPuncherCameraSettings settings = GetCameraSettings();
                m_camera.Initialize(settings.DrawingSurface, settings);
            } catch (Exception exc) {
                Assert.Fail("Failed to initialize Camera. Desc: {0} CS: {1}", exc.Message, exc.StackTrace);
            }

        }

        /// <summary>
        /// Test that will try to start the playback
        ///</summary>
        [TestMethod()]
        public void StartTest() {

            Assert.IsNotNull(m_camera);

            try {
                m_camera.Start();
            } catch (Exception exc) {
                Assert.Fail("Failed to start Camera. Desc: {0} CS: {1}", exc.Message, exc.StackTrace);
            }

        }

        /// <summary>
        /// Test that will try to stop the playback
        ///</summary>
        [TestMethod()]
        public void StopTest() {

            Assert.IsNotNull(m_camera);

            try {
                m_camera.Stop();
            } catch (Exception exc) {
                Assert.Fail("Failed to stop Camera. Desc: {0} CS: {1}", exc.Message, exc.StackTrace);
            }

        }

        /// <summary>
        /// Test that will try to release the camera
        ///</summary>
        [TestMethod()]
        public void ReleaseTest() {

            Assert.IsNotNull(m_camera);

            try {
                
                m_camera.Release();
                m_camera = null;

            } catch (Exception exc) {
                Assert.Fail("Failed to release Camera. Desc: {0} CS: {1}", exc.Message, exc.StackTrace);
            }

        }

        /// <summary>
        /// Test that will check that the requested and found punch positions are the same between
        /// the same image
        ///</summary>
        [TestMethod()]
        public void PunchPositions_MustBeSame() {

            int delayEmpty = 500;
            int delayCard = 500;
            int delayBlood = 500;

            string image1 = FILE_BLOOD_1;
            string image2 = FILE_BLOOD_1;

            Assert.IsNotNull(m_camera);

            try {

                ShowImage(FILE_EMPTY);
                SetPunchPositions();
                Thread.Sleep(delayEmpty);

                ShowImage(FILE_CARD);
                Thread.Sleep(delayCard);

                ShowImage(image1);
                Thread.Sleep(delayBlood);

                // Get first set on punch positions
                PunchPositions first = new PunchPositions(m_camera.PunchPositions);
                if(GetValidPositionCount(first) == 0) {
                    Assert.Fail("#1: No punch positions found from the \"{0}\"", image1);
                }

                ShowImage(FILE_CARD);
                Thread.Sleep(delayCard);

                ShowImage(image2);
                Thread.Sleep(delayBlood);

                // Get second set of punch positions
                PunchPositions second = new PunchPositions(m_camera.PunchPositions);

                // Positions are reset, in case we run other tests
                ShowImage(FILE_CARD);
                Thread.Sleep(10);

                if (GetValidPositionCount(second) == 0) {
                    Assert.Fail("#2: No punch positions found from the \"{0}\"", image2);
                }

                // Verify that positions are the same
                string errorDesc;
                if (!VerifyPositions(first, second, out errorDesc)) {
                    Assert.Fail(errorDesc);
                }


            } catch (Exception exc) {
                Assert.Fail("Failed to detect punch positions. Desc: {0} CS: {1}", exc.Message, exc.StackTrace);
            }

        }

        /// <summary>
        /// Test that will check that the requested and found punch positions are not the same between
        /// the different images
        ///</summary>
        [TestMethod()]
        public void PunchPositions_MustBeDifferent() {

            int delayEmpty = 500;
            int delayCard = 500;
            int delayBlood = 500;

            string image1 = FILE_BLOOD_1;
            string image2 = FILE_BLOOD_2;


            Assert.IsNotNull(m_camera);

            try {

                ShowImage(FILE_EMPTY);
                SetPunchPositions();
                Thread.Sleep(delayEmpty);

                ShowImage(FILE_CARD);
                Thread.Sleep(delayCard);

                ShowImage(image1);
                Thread.Sleep(delayBlood);

                // Get first set on punch positions
                PunchPositions first = new PunchPositions(m_camera.PunchPositions);
                if (GetValidPositionCount(first) == 0) {
                    Assert.Fail("#1: No punch positions found from the \"{0}\"", image1);
                }

                ShowImage(FILE_CARD);
                Thread.Sleep(delayCard);

                ShowImage(image2);
                Thread.Sleep(delayBlood);

                // Get second set of punch positions
                PunchPositions second = new PunchPositions(m_camera.PunchPositions);

                // Positions are reset, in case we run other tests
                ShowImage(FILE_CARD);
                Thread.Sleep(10);

                if (GetValidPositionCount(second) == 0) {
                    Assert.Fail("#2: No punch positions found from the \"{0}\"", image2);
                }

                // Verify that positions are different
                string errorDesc;
                if (VerifyPositions(first, second, out errorDesc)) {
                    Assert.Fail("Punch positions are the same");
                }

            } catch (Exception exc) {
                Assert.Fail("Failed to detect punch positions. Desc: {0} CS: {1}", exc.Message, exc.StackTrace);
            }

        }

        /// <summary>
        /// Test that checks that the settings we set to camera, are also the ones that we get from the camera
        /// </summary>
        [TestMethod()]
        public void Settings_MustBeSame() {

            Assert.IsNotNull(m_camera);

            try {

                IPuncherCameraSettings orginal = GetCameraSettings();
                m_camera.Settings = orginal;

                IPuncherCameraSettings current = m_camera.Settings;
                string errorDesc;
                if (!VerifySettings(orginal, current, out errorDesc)) {
                    Assert.Fail("Settings are different - " + errorDesc);
                }

            } catch (Exception exc) {
                Assert.Fail("Failed to if the settings are the same. Desc: {0} CS: {1}", exc.Message, exc.StackTrace);
            }

        }


        #endregion


        #region Private data

        private const string TEST_INPUT_ROOT = @"C:\SpotLight\Instr\Master\Imager\CameraWrapperUnitTester\TestInput\";

        private const string FILE_EMPTY = TEST_INPUT_ROOT + "empty_2.bmp";
        private const string FILE_CARD = TEST_INPUT_ROOT + "card.bmp";
        private const string FILE_BLOOD_1 = TEST_INPUT_ROOT + "blood_2.bmp";
        private const string FILE_BLOOD_2 = TEST_INPUT_ROOT + "blood_3.bmp";

        private static Camera m_camera = null;

        private TestContext testContextInstance;

        private static Process m_DrawingSurfaceExe = null;

        #endregion


        #region Private methods

        private bool VerifySettings(IPuncherCameraSettings s1, IPuncherCameraSettings s2, out string errorDesc) {

            errorDesc = "";

            if (s1.AllHotspotsMustBeInvalid != s2.AllHotspotsMustBeInvalid) {
                errorDesc = string.Format("AllHotspotsMustBeInvalid - #1 {0}, #2 {1}",
                    s1.AllHotspotsMustBeInvalid, s2.AllHotspotsMustBeInvalid);
                return false;
            }

            if (s1.BarcodeCount != s2.BarcodeCount) {
                errorDesc = string.Format("BarcodeCount - #1 {0}, #2 {1}",
                    s1.BarcodeCount, s2.BarcodeCount);
                return false;
            }

            if (s1.BarcodeReader != s1.BarcodeReader) {
                errorDesc = string.Format("BarcodeReader - #1 {0}, #2 {1}",
                    s1.BarcodeReader, s2.BarcodeReader);
                return false;
            }

            if (s1.BarcodeReaderLocation != s2.BarcodeReaderLocation) {
                errorDesc = string.Format("BarcodeReaderLocation - #1 {0}, #2 {1}",
                    s1.BarcodeReaderLocation, s2.BarcodeReaderLocation);
                return false;
            }

            if (s1.BarcodeReadingDirection != s2.BarcodeReadingDirection) {
                errorDesc = string.Format("BarcodeReadingDirection - #1 {0}, #2 {1}",
                    s1.BarcodeReadingDirection, s2.BarcodeReadingDirection);
                return false;
            }

            if (s1.BarcodeReadingInterval1 != s2.BarcodeReadingInterval1) {
                errorDesc = string.Format("BarcodeReadingInterval1 - #1 {0}, #2 {1}",
                    s1.BarcodeReadingInterval1, s2.BarcodeReadingInterval1);
                return false;
            }

            if (s1.BarcodeReadingInterval2 != s2.BarcodeReadingInterval2) {
                errorDesc = string.Format("BarcodeReadingInterval2 - #1 {0}, #2 {1}",
                    s1.BarcodeReadingInterval2, s2.BarcodeReadingInterval2);
                return false;
            }

            if (!IsColorSame(s1.BloodColor, s2.BloodColor)) {
                errorDesc = string.Format("BloodColor - #1 {0}, #2 {1}",
                    s1.BloodColor, s2.BloodColor);
                return false;
            }

            if (!IsColorSame(s1.BloodColorMax, s2.BloodColorMax)) {
                errorDesc = string.Format("BloodColorMax - #1 {0}, #2 {1}",
                    s1.BloodColorMax, s2.BloodColorMax);
                return false;
            }

            if (!IsColorSame(s1.BloodColorMax2, s2.BloodColorMax2)) {
                errorDesc = string.Format("BloodColorMax2 - #1 {0}, #2 {1}",
                    s1.BloodColorMax2, s2.BloodColorMax2);
                return false;
            }

            if (!IsColorSame(s1.BloodColorMin, s2.BloodColorMin)) {
                errorDesc = string.Format("BloodColorMin - #1 {0}, #2 {1}",
                    s1.BloodColorMin, s2.BloodColorMin);
                return false;
            }

            if (!IsColorSame(s1.BloodColorMin2, s2.BloodColorMin2)) {
                errorDesc = string.Format("BloodColorMin2 - #1 {0}, #2 {1}",
                    s1.BloodColorMin2, s2.BloodColorMin2);
                return false;
            }

            if (s1.BloodDetection != s2.BloodDetection) {
                errorDesc = string.Format("BloodDetection - #1 {0}, #2 {1}",
                    s1.BloodDetection, s2.BloodDetection);
                return false;
            }

            // CalibrationImage

            if (s1.CameraConnection != s2.CameraConnection) {
                errorDesc = string.Format("CameraConnection - #1 {0}, #2 {1}",
                    s1.CameraConnection, s2.CameraConnection);
                return false;
            }

            if (s1.DrawBlood != s2.DrawBlood) {
                errorDesc = string.Format("DrawBlood - #1 {0}, #2 {1}",
                    s1.DrawBlood, s2.DrawBlood);
                return false;
            }

            if (s1.DrawHotspots != s2.DrawHotspots) {
                errorDesc = string.Format("DrawHotspots - #1 {0}, #2 {1}",
                    s1.DrawHotspots, s2.DrawHotspots);
                return false;
            }

            if (s1.DrawingSurface != s2.DrawingSurface) {
                errorDesc = string.Format("DrawingSurface - #1 {0}, #2 {1}",
                    s1.DrawingSurface, s2.DrawingSurface);
                return false;
            }

            if (s1.DrawPunchPositions != s2.DrawPunchPositions) {
                errorDesc = string.Format("DrawPunchPositions - #1 {0}, #2 {1}",
                    s1.DrawPunchPositions, s2.DrawPunchPositions);
                return false;
            }

            if (s1.DrawSearchAreaBorder != s2.DrawSearchAreaBorder) {
                errorDesc = string.Format("DrawSearchAreaBorder - #1 {0}, #2 {1}",
                    s1.DrawSearchAreaBorder, s2.DrawSearchAreaBorder);
                return false;
            }

            if (s1.DriverImplementationFile != s2.DriverImplementationFile) {
                errorDesc = string.Format("DriverImplementationFile - #1 {0}, #2 {1}",
                    s1.DriverImplementationFile, s2.DriverImplementationFile);
                return false;
            }

            if (s1.FlipHorizontally != s2.FlipHorizontally) {
                errorDesc = string.Format("FlipHorizontally - #1 {0}, #2 {1}",
                    s1.FlipHorizontally, s2.FlipHorizontally);
                return false;
            }

            if (s1.FlipVertically != s2.FlipVertically) {
                errorDesc = string.Format("FlipVertically - #1 {0}, #2 {1}",
                    s1.FlipVertically, s2.FlipVertically);
                return false;
            }

            if (s1.FrameRenderInterval != s2.FrameRenderInterval) {
                errorDesc = string.Format("FrameRenderInterval - #1 {0}, #2 {1}",
                    s1.FrameRenderInterval, s2.FrameRenderInterval);
                return false;
            }

            if (s1.FramesSkippedNotificationLimit != s2.FramesSkippedNotificationLimit) {
                errorDesc = string.Format("FramesSkippedNotificationLimit - #1 {0}, #2 {1}",
                    s1.FramesSkippedNotificationLimit, s2.FramesSkippedNotificationLimit);
                return false;
            }

            if (s1.HotspotAllowedInvalidPixelPercent != s2.HotspotAllowedInvalidPixelPercent) {
                errorDesc = string.Format("HotspotAllowedInvalidPixelPercent - #1 {0}, #2 {1}",
                    s1.HotspotAllowedInvalidPixelPercent, s2.HotspotAllowedInvalidPixelPercent);
                return false;
            }

            if (!IsColorSame(s1.HotspotColor, s2.HotspotColor)) {
                errorDesc = string.Format("HotspotColor - #1 {0}, #2 {1}",
                    s1.HotspotColor, s2.HotspotColor);
                return false;
            }

            if (s1.HotspotCount != s2.HotspotCount) {
                errorDesc = string.Format("HotspotCount - #1 {0}, #2 {1}",
                    s1.HotspotCount, s2.HotspotCount);
                return false;
            }

            if (s1.HotspotDetectionAccuracyX != s2.HotspotDetectionAccuracyX) {
                errorDesc = string.Format("HotspotDetectionAccuracyX - #1 {0}, #2 {1}",
                    s1.HotspotDetectionAccuracyX, s2.HotspotDetectionAccuracyX);
                return false;
            }

            if (s1.HotspotDetectionAccuracyY != s2.HotspotDetectionAccuracyY) {
                errorDesc = string.Format("HotspotDetectionAccuracyY - #1 {0}, #2 {1}",
                    s1.HotspotDetectionAccuracyY, s2.HotspotDetectionAccuracyY);
                return false;
            }

            if (s1.HotspotDetectionColorMaxOffset != s2.HotspotDetectionColorMaxOffset) {
                errorDesc = string.Format("HotspotDetectionColorMaxOffset - #1 {0}, #2 {1}",
                    s1.HotspotDetectionColorMaxOffset, s2.HotspotDetectionColorMaxOffset);
                return false;
            }

            if (s1.HotspotDetectionColorMinOffset != s2.HotspotDetectionColorMinOffset) {
                errorDesc = string.Format("HotspotDetectionColorMinOffset - #1 {0}, #2 {1}",
                    s1.HotspotDetectionColorMinOffset, s2.HotspotDetectionColorMinOffset);
                return false;
            }

            if (s1.HotspotOffsetY != s2.HotspotOffsetY) {
                errorDesc = string.Format("HotspotOffsetY - #1 {0}, #2 {1}",
                    s1.HotspotOffsetY, s2.HotspotOffsetY);
                return false;
            }

            if (s1.HotspotSize != s2.HotspotSize) {
                errorDesc = string.Format("HotspotSize - #1 {0}, #2 {1}",
                    s1.HotspotSize, s2.HotspotSize);
                return false;
            }

            if (s1.InputImage != s2.InputImage) {
                errorDesc = string.Format("InputImage - #1 {0}, #2 {1}",
                    s1.InputImage, s2.InputImage);
                return false;
            }

            if (s1.InSimulationMode != s2.InSimulationMode) {
                errorDesc = string.Format("InSimulationMode - #1 {0}, #2 {1}",
                    s1.InSimulationMode, s2.InSimulationMode);
                return false;
            }

            if (s1.PunchBorder != s2.PunchBorder) {
                errorDesc = string.Format("PunchBorder - #1 {0}, #2 {1}",
                    s1.PunchBorder, s2.PunchBorder);
                return false;
            }

            if (s1.PunchDetectionAccuracyX != s2.PunchDetectionAccuracyX) {
                errorDesc = string.Format("PunchDetectionAccuracyX - #1 {0}, #2 {1}",
                    s1.PunchDetectionAccuracyX, s2.PunchDetectionAccuracyX);
                return false;
            }

            if (s1.PunchDetectionAccuracyY != s2.PunchDetectionAccuracyY) {
                errorDesc = string.Format("PunchDetectionAccuracyY - #1 {0}, #2 {1}",
                    s1.PunchDetectionAccuracyY, s2.PunchDetectionAccuracyY);
                return false;
            }

            if (s1.RequiredBloodPercent != s2.RequiredBloodPercent) {
                errorDesc = string.Format("RequiredBloodPercent - #1 {0}, #2 {1}",
                    s1.RequiredBloodPercent, s2.RequiredBloodPercent);
                return false;
            }

            if (s1.SearchAreaAllowedInvalidPixelPercent != s2.SearchAreaAllowedInvalidPixelPercent) {
                errorDesc = string.Format("SearchAreaAllowedInvalidPixelPercent - #1 {0}, #2 {1}",
                    s1.SearchAreaAllowedInvalidPixelPercent, s2.SearchAreaAllowedInvalidPixelPercent);
                return false;
            }

            if (!IsColorSame(s1.SearchAreaBorderColor, s2.SearchAreaBorderColor)) {
                errorDesc = string.Format("SearchAreaBorderColor - #1 {0}, #2 {1}",
                    s1.SearchAreaBorderColor, s2.SearchAreaBorderColor);
                return false;
            }

            if (s1.SearchAreaBorderWidth != s2.SearchAreaBorderWidth) {
                errorDesc = string.Format("SearchAreaBorderWidth - #1 {0}, #2 {1}",
                    s1.SearchAreaBorderWidth, s2.SearchAreaBorderWidth);
                return false;
            }

            if (s1.SearchAreaCenter != s2.SearchAreaCenter) {
                errorDesc = string.Format("SearchAreaCenter - #1 {0}, #2 {1}",
                    s1.SearchAreaCenter, s2.SearchAreaCenter);
                return false;
            }

            if (s1.SearchAreaDetectionAccuracyX != s2.SearchAreaDetectionAccuracyX) {
                errorDesc = string.Format("SearchAreaDetectionAccuracyX - #1 {0}, #2 {1}",
                    s1.SearchAreaDetectionAccuracyX, s2.SearchAreaDetectionAccuracyX);
                return false;
            }

            if (s1.SearchAreaDetectionAccuracyY != s2.SearchAreaDetectionAccuracyY) {
                errorDesc = string.Format("SearchAreaDetectionAccuracyY - #1 {0}, #2 {1}",
                    s1.SearchAreaDetectionAccuracyY, s2.SearchAreaDetectionAccuracyY);
                return false;
            }

            if (s1.SearchAreaDetectionColorMaxOffset != s2.SearchAreaDetectionColorMaxOffset) {
                errorDesc = string.Format("SearchAreaDetectionColorMaxOffset - #1 {0}, #2 {1}",
                    s1.SearchAreaDetectionColorMaxOffset, s2.SearchAreaDetectionColorMaxOffset);
                return false;
            }

            if (s1.SearchAreaDetectionColorMinOffset != s2.SearchAreaDetectionColorMinOffset) {
                errorDesc = string.Format("SearchAreaDetectionColorMinOffset - #1 {0}, #2 {1}",
                    s1.SearchAreaDetectionColorMinOffset, s2.SearchAreaDetectionColorMinOffset);
                return false;
            }

            if (s1.SearchAreaRadius != s2.SearchAreaRadius) {
                errorDesc = string.Format("SearchAreaRadius - #1 {0}, #2 {1}",
                    s1.SearchAreaRadius, s2.SearchAreaRadius);
                return false;
            }

            if (s1.SimulationFile != s2.SimulationFile) {
                errorDesc = string.Format("SimulationFile - #1 {0}, #2 {1}",
                    s1.SimulationFile, s2.SimulationFile);
                return false;
            }

            if (s1.UseSecondBloodDetection != s2.UseSecondBloodDetection) {
                errorDesc = string.Format("UseSecondBloodDetection - #1 {0}, #2 {1}",
                    s1.UseSecondBloodDetection, s2.UseSecondBloodDetection);
                return false;
            }

            return true;

        }

        /// <summary>
        /// Returns true if colors are same. Name property is ignored
        /// </summary>
        private bool IsColorSame(Color c1, Color c2) {

            if (c1.R == c2.R &&
                c1.G == c2.G &&
                c1.B == c2.B &&
                c1.A == c2.A) {
                return true;
            }
            
            return false;

        }

        /// <summary>
        /// Get camera settings
        /// </summary>
        private IPuncherCameraSettings GetCameraSettings() {

            IPuncherCameraSettings cs = new CameraSettings();

            cs.AllHotspotsMustBeInvalid = false;
            cs.BarcodeCount = 2;
            cs.BarcodeReader = BarcodeReaderType.DTK;
            cs.BarcodeReaderLocation = @"C:\SpotLight\Instr\Master\Imager\DTKBarcodeReader\DTKBarReader.dll";

            cs.BarcodeReadingDirection =
              (byte)BarcodeOrientation.LeftToRight |
              (byte)BarcodeOrientation.RightToLeft |
              (byte)BarcodeOrientation.TopToBottom |
              (byte)BarcodeOrientation.BottomToTop;

            cs.BarcodeReadingInterval1 = 500;
            cs.BarcodeReadingInterval2 = 1000;
            cs.BloodColor = Color.FromArgb(0, 128, 192);

            cs.BloodColorMin = Color.Black;
            cs.BloodColorMax = Color.FromArgb(100, 100, 100);

            cs.BloodColorMin2 = Color.Black;
            cs.BloodColorMax2 = Color.FromArgb(130, 130, 130);

            cs.BloodDetection = BloodDetectionMethod.DetectBlood;
            cs.CalibrationImage = new Bitmap(FILE_EMPTY);
            cs.CameraConnection = PuncherCameraConnectionType.DirectShowCamera;
            cs.DrawBlood = false;
            cs.DrawHotspots = false;
            cs.DrawingSurface = m_Handle;
            cs.DrawPunchPositions = false;
            cs.DrawSearchAreaBorder = false;
            cs.DriverImplementationFile = @"C:\SpotLight\Common\Bin\DSCameraDriver.dll";
            cs.FlipHorizontally = false;
            cs.FlipVertically = false;
            cs.FrameRenderInterval = 50;
            cs.FramesSkippedNotificationLimit = 2000;
            cs.HotspotAllowedInvalidPixelPercent = 10;
            cs.HotspotColor = Color.Red;
            cs.HotspotCount = 4;
            cs.HotspotDetectionAccuracyX = 0;
            cs.HotspotDetectionAccuracyY = 0;
            cs.HotspotDetectionColorMaxOffset = 50;
            cs.HotspotDetectionColorMinOffset = 50;
            cs.HotspotOffsetY = 50;
            cs.HotspotSize = new Size(50, 50);
            cs.InputImage = InputImageType.Color;
            cs.InSimulationMode = false;
            cs.PunchBorder = 2;
            cs.PunchDetectionAccuracyX = 0;
            cs.PunchDetectionAccuracyY = 0;
            cs.RequiredBloodPercent = 10;
            cs.SearchAreaAllowedInvalidPixelPercent = 10;
            cs.SearchAreaBorderColor = Color.Red;
            cs.SearchAreaBorderWidth = 1;
            cs.SearchAreaCenter = new Point(400, 200);
            cs.SearchAreaDetectionAccuracyX = 0;
            cs.SearchAreaDetectionAccuracyY = 0;
            cs.SearchAreaDetectionColorMaxOffset = 50;
            cs.SearchAreaDetectionColorMinOffset = 50;
            cs.SearchAreaRadius = 40;
            cs.SimulationFile = "";
            cs.UseSecondBloodDetection = true;

            return cs;

        }

        /// <summary>
        /// Show specified image in the camera
        /// </summary>
        private void ShowImage(string filename) {

            Bitmap bmp = new Bitmap(filename);
            m_camera.DisplayImage(bmp);
            bmp.Dispose();
            bmp = null;

        }

        private void SetPunchPositions() {

            IPunchPositions pp = new PunchPositions();

            pp.Add(GetPosition(1));
            pp.Add(GetPosition(2));
            pp.Add(GetPosition(3));
            pp.Add(GetPosition(4));
            pp.Add(GetPosition(5));
            pp.Add(GetPosition(6));

            m_camera.PunchPositions = pp;

        }
        private IPunchPosition GetPosition(int id) {

            IPunchPosition p = new PunchPosition();

            p.BorderColor = Color.Red;
            p.BorderWidth = 1;
            p.FillColor = Color.Red;
            p.ID = id;
            p.InBlood = false;
            p.Locked = false;
            p.Radius = 12;
            p.TextColor = Color.White;
            p.Used = true;


            return p;

        }

        private int GetValidPositionCount(PunchPositions pp) {

            int count = pp.Count;
            int c = 0;

            for (int i = 0; i < count; i++) {
                IPunchPosition p = (IPunchPosition)pp[i];
                if (p.InBlood) {
                    c++;
                }
            }

            return c;

        }

        private bool VerifyPositions(PunchPositions pp1, PunchPositions pp2, out string errorDes) {

            if (pp1.DrawBorder != pp2.DrawBorder) {
                errorDes = string.Format("DrawBorder doesn´t match - #1: {0} #2: {1}",
                    pp1.DrawBorder, pp2.DrawBorder);
                return false;
            }

            if (pp1.DrawBorderIfLocked != pp2.DrawBorderIfLocked) {
                errorDes = string.Format("DrawBorderIfLocked doesn´t match - #1: {0} #2: {1}",
                    pp1.DrawBorderIfLocked, pp2.DrawBorderIfLocked);
                return false;
            }

            if (pp1.DrawId != pp2.DrawId) {
                errorDes = string.Format("DrawId doesn´t match - #1: {0} #2: {1}",
                    pp1.DrawId, pp2.DrawId);
                return false;
            }


            if (pp1.DrawIdOnlyInBlood != pp2.DrawIdOnlyInBlood) {
                errorDes = string.Format("DrawIdOnlyInBlood doesn´t match - #1: {0} #2: {1}",
                    pp1.DrawIdOnlyInBlood, pp2.DrawIdOnlyInBlood);
                return false;
            }

            if (pp1.FillBackground != pp2.FillBackground) {
                errorDes = string.Format("FillBackground doesn´t match - #1: {0} #2: {1}",
                    pp1.FillBackground, pp2.FillBackground);
                return false;
            }

            if (pp1.InPunchProgressMode != pp2.InPunchProgressMode) {
                errorDes = string.Format("InPunchProgressMode doesn´t match - #1: {0} #2: {1}",
                    pp1.InPunchProgressMode, pp2.InPunchProgressMode);
                return false;
            }

            int c1 = pp1.Count;
            int c2 = pp2.Count;

            if (c1 == c2) {

                int fc1 = 0;
                int fc2 = 0;

                IPunchPosition p1;
                IPunchPosition p2;

                for (int i = 0; i < c1; i++) {

                    p1 = (IPunchPosition)pp1[i];
                    p2 = (IPunchPosition)pp2[i];

                    if (p1.InBlood) {
                        fc1++;
                    }

                    if (p2.InBlood) {
                        fc2++;
                    }

                    if (p1.Position != p2.Position) {
                        errorDes = string.Format("Found punch position doesn´t match. P: {0}, Pos 1: {1} Pos 2: {2}",
                            i, p1.Position, p2.Position);
                        return false;
                    }

                }

                if (fc1 != fc2) {
                    errorDes = string.Format("Found punch count doesn´t match. PC 1: {0} PC 2: {1}", fc1, fc2);
                    return false;
                } else {
                    errorDes = "";
                    return true;
                }


            }

            errorDes = string.Format("Punch count doesn´t match. PC 1: {0} PC 2: {1}", c1, c2);
            return false;

        }

        private IntPtr GetDrawingSurface() {

            if (m_DrawingSurfaceExe != null) {
                Assert.Fail("Drawing surface is already started");
            }


            string drawingSurfacePath;
#if DEBUG
            drawingSurfacePath = @"C:\SpotLight\Instr\Master\Imager\UnitTestDrawingSurface\bin\Debug\UnitTestDrawingSurface.exe";
#else
            drawingSurfacePath = @"C:\SpotLight\Instr\Master\Imager\UnitTestDrawingSurface\bin\Release\UnitTestDrawingSurface.exe";
#endif

            FileInfo fi = new FileInfo(drawingSurfacePath);
            if (!fi.Exists) {
                Assert.Fail("Failed to get drawing surface - \"{0}\" is missing", drawingSurfacePath);
            }


            // Start our surface app
            m_DrawingSurfaceExe = new Process();
            m_DrawingSurfaceExe.StartInfo.FileName = drawingSurfacePath;
            m_DrawingSurfaceExe.StartInfo.WindowStyle = ProcessWindowStyle.Normal;
            m_DrawingSurfaceExe.Start();

            Thread.Sleep(500);

            string wndClassNAme = "WindowsForms10.Window.8.app.0.378734a";
            string wndTitle = "CameraWrapper Unit test drawing surface";

            IntPtr tmpPtr = FindWindow(wndClassNAme, wndTitle);
            if (tmpPtr == IntPtr.Zero) {
                Assert.Fail("Failed to get drawing surface");
            }
            
            return tmpPtr;
        }

        #endregion

    }

}