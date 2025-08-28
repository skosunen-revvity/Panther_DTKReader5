// --------------------------------------------------------------------------
// Camera
// --------------------------------------------------------------------------
//
// SpotLight camera object. Provides initialization methods to actual camera
// image renderer via dynamic loadin (Assembly.LoadFrom). This class can be
// also used to configute initialized camera via
// - Setting-property, set various settings. This is basically the same operation
//   that the Initialize-method performs
// - PunchPositions-property, request/search punch positions from the image
//
// When camera connection type is PuncherCameraConnectionType.DirectShowCamera,
// thread is started to listen the events which are raised from the actual image
// renderer. The image rendere is executed in different thread, because it is 
// called by the DirectShow runtime. Events incluse punch count changes, card
// detection changes, among others.
//
// Normally live image from the camera is displayed, put the DisplayImage can be 
// used to skip the live feed and display specified still image
//
// Basic usage:
// #1 Create camera and initialize it
// #2 Start it (via Start-method)
// #3 Try to find punch positions form the image (PunchPositions-property)
// #4 Listen PunchPositionCountChanged-event to get notification when found punch
//    position count have changed
// #5 Close camera (Release-method)
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C#.NET 2008
//
// $Header: $
// Created: 27.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

using System;
using System.Reflection;
using System.Threading;
using Microsoft.Win32.SafeHandles;
using System.Drawing;
using System.IO;

using PKI.GSP.Processor.Library;
using PKI.Puncher.Library;
using PKI.Wallac.Logging;
using PKI.Puncher.PlateExportImportBase;

namespace PKI.Puncher.CameraWrapper {

    /// <summary>
    /// SpotLight camera object. Provides initialization and configuration methods to
    /// actual camera image renderer (which is loaded during initialization via Assembly.LoadFrom)
    /// </summary>
    public class Camera : IPuncherCamera {

        // --------------------------------------------------------------------------
        // Supported camera definitions
        // 
        // CAMERA WIDTH and CAMERA_HEIGHT indicates the used resolution of the camera.
        // X_POSITION and Y_POSITION indicates the offset for the used image.
        // CAMERA_WIDTH_MM and CAMERA_HEIGHT_MM are measured with a ruler positioned below the camera.
        // The mm values are used only for the initial settings. 
        // The actual pixel size is set by the service program.
        // --------------------------------------------------------------------------

        // The original camera
        private readonly int ORIGINAL_CAMERA_WIDTH = 752;
        private readonly int ORIGINAL_CAMERA_HEIGHT = 480;
        private readonly int ORIGINAL_CAMERA_X_POSITION = 0;
        private readonly int ORIGINAL_CAMERA_Y_POSITION = 0;
        private readonly int ORIGINAL_CAMERA_WIDTH_MM = 85;
        private readonly int ORIGINAL_CAMERA_HEIGHT_MM = 54;
        private readonly int ORIGINAL_CAMERA_ORIENTATION = 8;
        private readonly int ORIGINAL_CAMERA_EXPOSURE = 800;
        private readonly int ORIGINAL_CAMERA_FRAMERATE = 1498;
        private readonly int ORIGINAL_CAMERA_MAX_EXPOSURE = 0;
        private readonly int ORIGINAL_CAMERA_COLOR_TEMPERATURE = 3250;
        private readonly int ORIGINAL_CAMERA_GAIN_BLUE = 17;
        private readonly int ORIGINAL_CAMERA_GAIN_RED = 4;
        private readonly int ORIGINAL_CAMERA_GAIN_GREEN = 0;
        private readonly int ORIGINAL_CAMERA_GAIN_MASTER = 10;
        private readonly int ORIGINAL_CAMERA_BL_ENABLE = 0;
        private readonly int ORIGINAL_CAMERA_EDGE_ENHANCEMENT = 1;

        // The second camera
        private readonly int SECOND_CAMERA_VERSION_WIDTH = 1440;
        private readonly int SECOND_CAMERA_VERSION_HEIGHT = 920;
        private readonly int SECOND_CAMERA_VERSION_X_POSITION = 90;
        private readonly int SECOND_CAMERA_VERSION_Y_POSITION = 130;
        private readonly int SECOND_CAMERA_VERSION_WIDTH_MM = 86;
        private readonly int SECOND_CAMERA_VERSION_HEIGHT_MM = 55;
        private readonly int SECOND_CAMERA_ORIENTATION = 8;
        private readonly int SECOND_CAMERA_BINNING = 0;
        private readonly int SECOND_CAMERA_CLOCK = 25;
        private readonly int SECOND_CAMERA_EXPOSURE = 1000;
        private readonly int SECOND_CAMERA_FRAMERATE = 1498;
        private readonly int SECOND_CAMERA_MAX_EXPOSURE = 0;
        private readonly int SECOND_CAMERA_COLOR_TEMPERATURE = 3250;
        private readonly int SECOND_CAMERA_COLOR_CORRECTION = 0;
        private readonly int SECOND_CAMERA_COLOR_MODE = 1;
        private readonly int SECOND_CAMERA_GAIN_BLUE = 23;
        private readonly int SECOND_CAMERA_GAIN_RED = 13;
        private readonly int SECOND_CAMERA_GAIN_GREEN = 0;
        private readonly int SECOND_CAMERA_GAIN_MASTER = 10;
        private readonly int SECOND_CAMERA_GAMMA = 100;
        private readonly int SECOND_CAMERA_BL_ENABLE = 0;
        private readonly int SECOND_CAMERA_BLACK_LEVEL = 0;
        private readonly int SECOND_CAMERA_EDGE_ENHANCEMENT = 1;
        // Bad pixel correction off
        private readonly int SECOND_CAMERA_BPC = 0;  

        /// <summary>
        /// Logger interface
        /// </summary>
        private Logger m_logger;

        /// <summary>
        /// Storage for configurables, initialized by LoadSettings and SaveSettings.
        /// </summary>
        private PersistorGroup m_storage = null;

        #region Constructors

        /// <summary>
        /// Default constructor
        /// </summary>
        public Camera() {
            m_logger = Logger.GetLogger("Camera");
        }

        #endregion


        #region IPuncherCamera Members

        /// <summary>
        /// Initialize actual camera renderer (IPuncherCameraSettings.DriverImplementationFile).
        /// If the cameraSettings-argument is null, LoadSettings is used. Can be called only once.
        /// 
        /// NOTE: After camera is succesfully initialized, Start must be called to get image from it
        /// 
        /// NOTE: The method starts a thread that listens to events from the camera image renderer. This thread runs 
        /// until calling the Release method. When these methods are called in the correct order from a single thread 
        /// it is safe to initialize and release the camera repeatedly. When Initialize and Release are called from
        /// different threads care need to be taken to ensure that the Release method has finished before calling
        /// Initialize.
        /// 
        /// </summary>
        /// <param name="drawingSurface">Native window handle (in Win32 HWND) where the actual image is drawn. This will override the value specified in cameraSettings</param>
        /// <param name="cameraSettings">Settings. If null, LoadSettings-method is used</param>
        public void Initialize(IntPtr drawingSurface, IPuncherCameraSettings cameraSettings) {

            // Only once
            if (!m_Initialized) {

                // Initialize logger
                Log.Initialize("PKI.Puncher.CameraWrapper");

                const string PROCNAME = CLASSNAME + SEP + "Initialize";

                try {

                    Log.Debug(PROCNAME, LogMessages.Debug.InitializingCamera);
                    if (cameraSettings == null) {
                        cameraSettings = LoadSettings();
                    }

                    cameraSettings.DrawingSurface = drawingSurface;


                    if (cameraSettings.InSimulationMode) {
                        Log.Information(PROCNAME, LogMessages.Information.CameraIsRunInSimulationMode, cameraSettings.SimulationFile);
                    }

                    switch (cameraSettings.CameraConnection) {

                        case PuncherCameraConnectionType.DirectShowCamera:
                            InitializeDSCamera(cameraSettings);

                            // Cache PunchPositionCountChanged-event sending interval
                            m_PunchCountChangedEventSendingInterval = cameraSettings.PunchCountChangedEventSendingInterval;

                            // Create and start listener thread. This will listen events that are raised from
                            // the actual camera image renderer.
                            m_DSCameraNotificationListener  = new Thread(CameraDriverNotificationListener);
                            m_DSCameraNotificationListener.Start(this);
                            break;

                        default:
                            // Invalid camera
                            // TODO: Add description
                            throw new ArgumentException();

                    }


                    Log.Information(PROCNAME, LogMessages.Information.CameraInitialized);
                    m_Initialized = true;

                } catch (Exception exc) {
                    Log.Error(PROCNAME, exc, LogMessages.Error.ExcCameraInitialized, exc.Message, exc.StackTrace);
                    throw exc;
                }
            } else {
                throw new InvalidOperationException();
            }

        }

        /// <summary>
        /// Unload camera.
        /// 
        /// NOTE: This method stops the thread that listens to image renderer events and waits until the thread has stopped.
        /// In the worst case the thread needs about 200 ms to stop. In an environment where the camera need to be initialized
        /// again the caller must ensure that Initialize is only called after this method has returned.
        /// 
        /// </summary>
        public void Release() {

            if (m_Initialized) {

                const string PROCNAME = CLASSNAME + SEP + "Release";
                Log.Debug(PROCNAME, LogMessages.Debug.ReleasingCamera);

                try {

                    // Close event listener
                    if (m_DSCameraNotificationListener != null) {
                        m_Closing = true;
                        m_DSCameraNotificationListener.Join();
                        m_DSCameraNotificationListener = null;
                    }

                } catch (Exception exc) {
                    Log.Error(PROCNAME, exc, LogMessages.Error.ErrCloseDSEventListener, exc.Message, exc.StackTrace);
                }

                try {

                    // Unload actual image renderer
                    if (m_DSCameraDriver != null) {
                        m_DSCameraDriver.Release();
                        m_DSCameraDriver = null;

                    }

                } catch (Exception exc) {
                    Log.Error(PROCNAME, exc, LogMessages.Error.ExcReleasingCamera, exc.Message, exc.StackTrace);
                    throw exc;
                }

                m_Initialized = false;
                Log.Information(PROCNAME, LogMessages.Information.CameraReleased);

            } else {
                throw new InvalidOperationException();
            }

        }

        /// <summary>
        /// Start camera. After camera is succesfully initialized, Start must be called in order 
        /// to get image from it, and start image processing. When image processing is started
        /// also events are received from the camera.
        /// </summary>
        public void Start() {

            if (m_Initialized) {
                switch (m_CameraConnection) {

                    case PuncherCameraConnectionType.DirectShowCamera:
                        m_DSCameraDriver.Start();
                        break;
                }
            } else {
                throw new InvalidOperationException();
            }

        }

        /// <summary>
        /// Stop camera. This won´t release camera, but it will just stop the image rendering/processing.
        /// </summary>
        public void Stop() {

            if (m_Initialized) {
                switch (m_CameraConnection) {

                    case PuncherCameraConnectionType.DirectShowCamera:
                        m_DSCameraDriver.Stop();
                        break;
                }
            } else {
                throw new InvalidOperationException();
            }

        }

        /// <summary>
        /// Returns measured frame intervals, and resets the maximum detected value.
        /// </summary>
        /// <param name="filteredInterval">Filtered frame interval from instrument startup in milliseconds</param>
        /// <param name="maxInterval">Maximum detected frame interval since instrument startup or since previous call of this 
        /// function in millisecons. </param>
        public void GetFrameInterval(out uint filteredInterval, out uint maxInterval) {
            if (m_Initialized) {
                switch (m_CameraConnection) {
                    case PuncherCameraConnectionType.DirectShowCamera:
                        filteredInterval = m_DSCameraDriver.FilteredFrameInterval;
                        maxInterval = m_DSCameraDriver.MaxFrameInterval;
                        m_DSCameraDriver.ResetMaxFrameInterval();
                        return;
                }
            }
            throw new InvalidOperationException();
        }

        /// <summary>
        /// Parform camera calibration, using next image, or, if specified, using the specified image.
        /// When camera is calibrated, so called hotspots are calculated from the image. These hotspots
        /// are later used to detect whether image contains card or not. The size of the image, must be
        /// the same that was used when camera was initialized
        /// </summary>
        public void Calibrate(Bitmap calibrationImage) {

            if (m_Initialized) {
                switch (m_CameraConnection) {

                    case PuncherCameraConnectionType.DirectShowCamera:
                        m_DSCameraDriver.Calibrate(calibrationImage);
                        break;
                }
            } else {
                throw new InvalidOperationException();
            }

        }

        /// <summary>
        /// Get/set the barcode reading status. When true, barcodes are read from the image
        /// </summary>
        public bool ReadBarcodes {
            get {

                if (m_Initialized) {
                    switch (m_CameraConnection) {

                        case PuncherCameraConnectionType.DirectShowCamera:
                            return m_DSCameraDriver.ReadBarcodes;
                    }
                }

                throw new InvalidOperationException();

            }
            set {

                if (m_Initialized) {
                    switch (m_CameraConnection) {

                        case PuncherCameraConnectionType.DirectShowCamera:
                            m_DSCameraDriver.ReadBarcodes = value;
                            break;
                    }
                } else {
                    throw new InvalidOperationException();
                }

            }
        }

        /// <summary>
        /// Load settings from the configuration file. All values, except the DrawingSurface, are loaded.
        /// Also, the following values are not loaded because, they are the default and the only type of 
        /// operation this calss currently supports:
        /// - CameraConnection
        /// - BarcodeReader
        /// - InputImage
        /// - BloodDetection
        /// - CalibrationImage
        /// </summary>
        public IPuncherCameraSettings LoadSettings() {

            // TODO: Use constants as default values
            m_storage = Config.Instance.GetGroup(CameraUtils.CAMERA_PERSISTOR_TYPE, CameraUtils.CAMERA_PERSISTOR_NAME);
            IPuncherCameraSettings cameraSettings = new CameraSettings();

            cameraSettings.PunchCountChangedEventSendingInterval = (ushort)m_storage.GetInt("PunchCountChangedEventSendingInterval", 500);

            cameraSettings.FlipHorizontally = m_storage.GetBool("FlipHorizontally", false);
            cameraSettings.FlipVertically = m_storage.GetBool("FlipVertically", false);

            // Specifies the directions how barcodes are detected from the image
            cameraSettings.BarcodeReadingDirection = (byte)m_storage.GetInt("BarcodeReadingDirection",
              (byte)PKI.Puncher.Library.BarcodeOrientation.LeftToRight |
              (byte)PKI.Puncher.Library.BarcodeOrientation.RightToLeft |
              (byte)PKI.Puncher.Library.BarcodeOrientation.TopToBottom |
              (byte)PKI.Puncher.Library.BarcodeOrientation.BottomToTop);

            cameraSettings.EnableScanRect1 = m_storage.GetBool("EnableScanRect1", true);
            cameraSettings.ScanRect1Left = (ushort)m_storage.GetInt("ScanRect1Left", 0);
            cameraSettings.ScanRect1Top = (ushort)m_storage.GetInt("ScanRect1Top", 350);
            cameraSettings.ScanRect1Right = (ushort)m_storage.GetInt("ScanRect1Right", 752);
            cameraSettings.ScanRect1Bottom = (ushort)m_storage.GetInt("ScanRect1Bottom", 480);

            cameraSettings.EnableScanRect2 = m_storage.GetBool("EnableScanRect2", true);
            cameraSettings.ScanRect2Left = (ushort)m_storage.GetInt("ScanRect2Left", 0);
            cameraSettings.ScanRect2Top = (ushort)m_storage.GetInt("ScanRect2Top", 0);
            cameraSettings.ScanRect2Right = (ushort)m_storage.GetInt("ScanRect2Right", 120);
            cameraSettings.ScanRect2Bottom = (ushort)m_storage.GetInt("ScanRect2Bottom", 480);

            cameraSettings.EnableScanRect3 = m_storage.GetBool("EnableScanRect3", true);
            cameraSettings.ScanRect3Left = (ushort)m_storage.GetInt("ScanRect3Left", 632);
            cameraSettings.ScanRect3Top = (ushort)m_storage.GetInt("ScanRect3Top", 0);
            cameraSettings.ScanRect3Right = (ushort)m_storage.GetInt("ScanRect3Right", 752);
            cameraSettings.ScanRect3Bottom = (ushort)m_storage.GetInt("ScanRect3Bottom", 480);

            cameraSettings.BarcodeRedundancy = (ushort)m_storage.GetInt("BarcodeRedundancy", 6);
            cameraSettings.MinimumBarcodeHeight = (ushort)m_storage.GetInt("MinimumBarcodeHeight", 20);

            cameraSettings.SearchAreaAllowedInvalidPixelPercent = (byte)m_storage.GetInt("SearchAreaAllowedInvalidPixelPercent", 20);
            cameraSettings.HotspotAllowedInvalidPixelPercent = (byte)m_storage.GetInt("HotspotAllowedChangedPixelPercent", 10);

            cameraSettings.InSimulationMode = m_storage.GetBool("InSimulationMode", false);
            cameraSettings.SimulationFile = System.Environment.ExpandEnvironmentVariables(
                m_storage.GetString("SimulationFile", string.Empty));

            // We cant load this from settings. This is a value specifed by the OS
            //cameraSettings.DrawingSurface

            cameraSettings.SearchAreaDetectionAccuracyX = (byte)m_storage.GetInt("SearchAreaDetectionAccuracyX", 1);
            cameraSettings.SearchAreaDetectionAccuracyY = (byte)m_storage.GetInt("SearchAreaDetectionAccuracyY", 1);

            cameraSettings.DrawHotspots = m_storage.GetBool("DrawHotspots", false);
            cameraSettings.AllHotspotsMustBeInvalid = m_storage.GetBool("AllHotspotsMustHaveChanged", false);

            cameraSettings.HotspotColor = Color.FromArgb(
              m_storage.GetInt("HotspotColorR", 255),
              m_storage.GetInt("HotspotColorG", 165),
              m_storage.GetInt("HotspotColorB", 0));

            cameraSettings.HotspotCount = (byte)m_storage.GetInt("HotspotCount", 4);
            cameraSettings.HotspotOffsetY = (ushort)m_storage.GetInt("HotspotOffsetY", 400);

            cameraSettings.HotspotSize = new Size(
              m_storage.GetInt("HotspotSizeW", 20),
              m_storage.GetInt("HotspotSizeH", 140));

            // Defaults defined from somewhat yellowish-orange image by IT 2011/09/16. Background image color in hotspot position
            // is approximatelly 25 for three RGB coefficients. For blood card the RGB value is approx. 255, 235, 120, giving sum
            // 610 for blood card. Choosing half the difference between 75, 610, gives ColorOffset 267. Note that going to darker images,
            // offset 267 from background color 75 means that RGB coeffients should be negative, so black blood cards cannot be detected.
            cameraSettings.HotspotDetectionColorMinOffset = m_storage.GetInt("HotspotDetectionColorMinOffset", 267);
            cameraSettings.HotspotDetectionColorMaxOffset = m_storage.GetInt("HotspotDetectionColorMaxOffset", 267);

            cameraSettings.HotspotDetectionAccuracyX = (byte)m_storage.GetInt("HotspotDetectionAccuracyX", 0);
            cameraSettings.HotspotDetectionAccuracyY = (byte)m_storage.GetInt("HotspotDetectionAccuracyY", 0);

            cameraSettings.DrawBlood = m_storage.GetBool("DrawBlood", false);
            cameraSettings.BloodColor = Color.FromArgb(
              m_storage.GetInt("BloodColorR", 0),
              m_storage.GetInt("BloodColorG", 128),
              m_storage.GetInt("BloodColorB", 192));

            cameraSettings.PunchDetectionAccuracyX = (byte)m_storage.GetInt("PunchDetectionAccuracyX", 1);
            cameraSettings.PunchDetectionAccuracyY = (byte)m_storage.GetInt("PunchDetectionAccuracyY", 1);

            cameraSettings.FrameRenderInterval = (ushort)m_storage.GetInt("FrameRenderInterval", 50);
            cameraSettings.FramesSkippedNotificationLimit = m_storage.GetInt("FramesSkippedNotificationLimit", 2000);

            cameraSettings.DrawPunchPositions = m_storage.GetBool("DrawPunchPositions", true);
            cameraSettings.PunchBorder = (byte)m_storage.GetInt("PunchBorder", 2);

            cameraSettings.DrawSearchAreaBorder = m_storage.GetBool("DrawSearchAreaBorder", true);
            cameraSettings.SearchAreaBorderColor = Color.FromArgb(
              m_storage.GetInt("SearchAreaBorderColorR", 255),
              m_storage.GetInt("SearchAreaBorderColorG", 128),
              m_storage.GetInt("SearchAreaBorderColorB", 64));

            cameraSettings.SearchAreaBorderWidth = (byte)m_storage.GetInt("SearchAreaBorderWidth", 1);

            cameraSettings.SearchAreaCenter = new Point(
              m_storage.GetInt("SearchAreaCenterX", 378),
              m_storage.GetInt("SearchAreaCenterY", 168));

            cameraSettings.SearchAreaRadius = (byte)m_storage.GetInt("SearchAreaRadius", 69);

            cameraSettings.BloodColorMin = Color.FromArgb(
              m_storage.GetInt("BloodColorMinR", 0),
              m_storage.GetInt("BloodColorMinG", 0),
              m_storage.GetInt("BloodColorMinB", 0));
            cameraSettings.BloodColorMax = Color.FromArgb(
              m_storage.GetInt("BloodColorMaxR", 160),
              m_storage.GetInt("BloodColorMaxG", 160),
              m_storage.GetInt("BloodColorMaxB", 160));

            cameraSettings.BloodColorMin2 = Color.FromArgb(
              m_storage.GetInt("BloodColorMin2R", 0),
              m_storage.GetInt("BloodColorMin2G", 0),
              m_storage.GetInt("BloodColorMin2B", 0));
            cameraSettings.BloodColorMax2 = Color.FromArgb(
              m_storage.GetInt("BloodColorMax2R", 180),
              m_storage.GetInt("BloodColorMax2G", 180),
              m_storage.GetInt("BloodColorMax2B", 180));

            cameraSettings.SearchAreaDetectionColorMinOffset = m_storage.GetInt("SearchAreaDetectionColorMinOffset", 50);
            cameraSettings.SearchAreaDetectionColorMaxOffset = m_storage.GetInt("SearchAreaDetectionColorMaxOffset", 50);

            cameraSettings.BarcodeReaderLocation = System.Environment.ExpandEnvironmentVariables(
                m_storage.GetString("BarcodeReaderLocation", ""));
            cameraSettings.RequiredBloodPercent = (byte)m_storage.GetInt("RequiredBloodPercent", 10);
            cameraSettings.DriverImplementationFile = System.Environment.ExpandEnvironmentVariables(
                m_storage.GetString("DriverImplementationFile", ""));

            cameraSettings.BarcodeCount = (byte)m_storage.GetInt("BarcodeCount", 1);
            cameraSettings.BarcodeReadingInterval1 = (ushort)m_storage.GetInt("BarcodeReadingInterval1", 100);
            cameraSettings.BarcodeReadingInterval2 = (ushort)m_storage.GetInt("BarcodeReadingInterval2", 250);


            // TODO: Load these settings when we support more
            cameraSettings.CameraConnection = PuncherCameraConnectionType.DirectShowCamera;
            cameraSettings.BarcodeReader = BarcodeReaderType.DTK;
            cameraSettings.InputImage = InputImageType.Color;
            cameraSettings.BloodDetection = BloodDetectionMethod.DetectBlood;
            cameraSettings.CalibrationImage = CameraUtils.LoadCalibrationImage();

            #region Original camera registry settings

            // These settings need to be copied to the windows registry for the camera to function as desired.
            // The registry keys are Orientation, Timing, White-Balance, Gain, Processing and 
            // they are located below HKEY_CURRENT_USER\Software\IDS\uEye\DirectShow\Device1\Settings\UI122xLE-C

            cameraSettings.RegSettingImageSizeWidth = (ushort)m_storage.GetInt("RegSettingImageSizeWidth", ORIGINAL_CAMERA_WIDTH);
            cameraSettings.RegSettingImageSizeHeight = (ushort)m_storage.GetInt("RegSettingImageSizeHeight", ORIGINAL_CAMERA_HEIGHT);
            cameraSettings.RegSettingImageSizeXPosition = (ushort)m_storage.GetInt("RegSettingImageSizeXPosition", ORIGINAL_CAMERA_X_POSITION);
            cameraSettings.RegSettingImageSizeYPosition = (ushort)m_storage.GetInt("RegSettingImageSizeYPosition", ORIGINAL_CAMERA_Y_POSITION);
            cameraSettings.RegSettingImageSizeWidth_mm = (ushort)m_storage.GetInt("RegSettingImageSizeWidth_mm", ORIGINAL_CAMERA_WIDTH_MM);
            cameraSettings.RegSettingImageSizeHeight_mm = (ushort)m_storage.GetInt("RegSettingImageSizeHeight_mm", ORIGINAL_CAMERA_HEIGHT_MM);
            cameraSettings.RegSettingImageSizeOrientation = (ushort)m_storage.GetInt("RegSettingImageSizeOrientation", ORIGINAL_CAMERA_ORIENTATION);
            cameraSettings.RegSettingTimingExposure = (ushort)m_storage.GetInt("RegSettingTimingExposure", ORIGINAL_CAMERA_EXPOSURE);
            cameraSettings.RegSettingTimingFramerate = (ushort)m_storage.GetInt("RegSettingTimingFramerate", ORIGINAL_CAMERA_FRAMERATE);
            cameraSettings.RegSettingTimingMaxExposure = (ushort)m_storage.GetInt("RegSettingTimingMaxExposure", ORIGINAL_CAMERA_MAX_EXPOSURE);
            cameraSettings.RegSettingWhiteBalanceColortemperature = (ushort)m_storage.GetInt("RegSettingBalanceColortemperature", ORIGINAL_CAMERA_COLOR_TEMPERATURE);
            cameraSettings.RegSettingGainBlue = (ushort)m_storage.GetInt("RegSettingGainBlue", ORIGINAL_CAMERA_GAIN_BLUE);
            cameraSettings.RegSettingGainRed = (ushort)m_storage.GetInt("RegSettingGainRed", ORIGINAL_CAMERA_GAIN_RED);
            cameraSettings.RegSettingGainGreen = (ushort)m_storage.GetInt("RegSettingGainGreen", ORIGINAL_CAMERA_GAIN_GREEN);
            cameraSettings.RegSettingGainMaster = (ushort)m_storage.GetInt("RegSettingGainMaster", ORIGINAL_CAMERA_GAIN_MASTER);
            cameraSettings.RegSettingProcessingBLEnable = (ushort)m_storage.GetInt("RegSettingProcessingBLEnable", ORIGINAL_CAMERA_BL_ENABLE);
            cameraSettings.RegSettingProcessingEdgeEnhancement = (ushort)m_storage.GetInt("RegSettingProcessingEdgeEnhancement", ORIGINAL_CAMERA_EDGE_ENHANCEMENT);

            #endregion

            #region Camera 2 registry settings

            // Camera 2 settings are located below HKEY_CURRENT_USER\Software\IDS\uEye\DirectShow\Device1\Settings\UI125xLE-C

            cameraSettings.Cam2RegSettingImageSizeWidth = (ushort)m_storage.GetInt("Cam2RegSettingImageSizeWidth", SECOND_CAMERA_VERSION_WIDTH);
            cameraSettings.Cam2RegSettingImageSizeHeight = (ushort)m_storage.GetInt("Cam2RegSettingImageSizeHeight", SECOND_CAMERA_VERSION_HEIGHT);
            cameraSettings.Cam2RegSettingImageSizeXPosition = (ushort)m_storage.GetInt("Cam2RegSettingImageSizeXPosition", SECOND_CAMERA_VERSION_X_POSITION);
            cameraSettings.Cam2RegSettingImageSizeYPosition = (ushort)m_storage.GetInt("Cam2RegSettingImageSizeYPosition", SECOND_CAMERA_VERSION_Y_POSITION);
            cameraSettings.Cam2RegSettingImageSizeWidth_mm = (ushort)m_storage.GetInt("Cam2RegSettingImageSizeWidth_mm", SECOND_CAMERA_VERSION_WIDTH_MM);
            cameraSettings.Cam2RegSettingImageSizeHeight_mm = (ushort)m_storage.GetInt("Cam2RegSettingImageSizeHeight_mm", SECOND_CAMERA_VERSION_HEIGHT_MM);
            cameraSettings.Cam2RegSettingImageSizeOrientation = (ushort)m_storage.GetInt("Cam2RegSettingImageSizeOrientation", SECOND_CAMERA_ORIENTATION);
            cameraSettings.Cam2RegSettingImageSizeBinning = (ushort)m_storage.GetInt("Cam2RegSettingImageSizeBinning", SECOND_CAMERA_BINNING);
            cameraSettings.Cam2RegSettingResamplingBinningMode = (ushort)m_storage.GetInt("Cam2RegSettingImageSizeBinning", SECOND_CAMERA_BINNING);
            cameraSettings.Cam2RegSettingTimingClock = (ushort)m_storage.GetInt("Cam2RegSettingTimingClock", SECOND_CAMERA_CLOCK);
            cameraSettings.Cam2RegSettingTimingExposure = (ushort)m_storage.GetInt("Cam2RegSettingTimingExposure", SECOND_CAMERA_EXPOSURE);
            cameraSettings.Cam2RegSettingTimingFramerate = (ushort)m_storage.GetInt("Cam2RegSettingTimingFramerate", SECOND_CAMERA_FRAMERATE);
            cameraSettings.Cam2RegSettingTimingMaxExposure = (ushort)m_storage.GetInt("Cam2RegSettingTimingMaxExposure", SECOND_CAMERA_MAX_EXPOSURE);
            cameraSettings.Cam2RegSettingWhiteBalanceColortemperature = (ushort)m_storage.GetInt("Cam2RegSettingBalanceColortemperature", SECOND_CAMERA_COLOR_TEMPERATURE);
            cameraSettings.Cam2RegSettingColorColorCorrection = (ushort)m_storage.GetInt("Cam2RegSettingColorColorCorrection", SECOND_CAMERA_COLOR_CORRECTION);
            cameraSettings.Cam2RegSettingColorColorMode = (ushort)m_storage.GetInt("Cam2RegSettingColorColorMode", SECOND_CAMERA_COLOR_MODE);
            cameraSettings.Cam2RegSettingGainBlue = (ushort)m_storage.GetInt("Cam2RegSettingGainBlue", SECOND_CAMERA_GAIN_BLUE);
            cameraSettings.Cam2RegSettingGainRed = (ushort)m_storage.GetInt("Cam2RegSettingGainRed", SECOND_CAMERA_GAIN_RED);
            cameraSettings.Cam2RegSettingGainGreen = (ushort)m_storage.GetInt("Cam2RegSettingGainGreen", SECOND_CAMERA_GAIN_GREEN);
            cameraSettings.Cam2RegSettingGainMaster = (ushort)m_storage.GetInt("Cam2RegSettingGainMaster", SECOND_CAMERA_GAIN_MASTER);
            cameraSettings.Cam2RegSettingGainGamma = (ushort)m_storage.GetInt("Cam2RegSettingGainGamma", SECOND_CAMERA_GAMMA);
            cameraSettings.Cam2RegSettingProcessingBLEnable = (ushort)m_storage.GetInt("Cam2RegSettingProcessingBLEnable", SECOND_CAMERA_BL_ENABLE);
            cameraSettings.Cam2RegSettingProcessingBlackLevel = (ushort)m_storage.GetInt("Cam2RegSettingProcessingBlackLevel", SECOND_CAMERA_BLACK_LEVEL);
            cameraSettings.Cam2RegSettingProcessingEdgeEnhancement = (ushort)m_storage.GetInt("Cam2RegSettingProcessingEdgeEnhancement", SECOND_CAMERA_EDGE_ENHANCEMENT);
            cameraSettings.Cam2RegSettingProcessingBPC = (ushort)m_storage.GetInt("Cam2RegSettingProcessingBPC", SECOND_CAMERA_BPC);

            #endregion

            #region Sample barcode settings

            cameraSettings.EnableCode128 = m_storage.GetBool("EnableCode128", true);
            cameraSettings.EnableInterleaved2of5 = m_storage.GetBool("EnableInterleaved2of5", true);
            cameraSettings.EnableCode39 = m_storage.GetBool("EnableCode39", true);
            cameraSettings.EnableDataMatrix = m_storage.GetBool("EnableDataMatrix", true);

            #endregion

            return cameraSettings;

        }

        /// <summary>
        /// Save settings to configuration file. All values, except the DrawingSurface, are saved.
        /// Also, the following values are not saved because, they are the default and the only type of 
        /// operation this calss currently supports:
        /// - CameraConnection
        /// - BarcodeReader
        /// - InputImage
        /// - BloodDetection
        /// - CalibrationImage
        /// </summary>
        public void SaveSettings(IPuncherCameraSettings cameraSettings) {
            m_storage = Config.Instance.GetGroup(CameraUtils.CAMERA_PERSISTOR_TYPE, CameraUtils.CAMERA_PERSISTOR_NAME);

            m_storage.SetInt("PunchCountChangedEventSendingInterval", cameraSettings.PunchCountChangedEventSendingInterval);
            m_storage.SetBool("FlipHorizontally", cameraSettings.FlipHorizontally);
            m_storage.SetBool("FlipVertically", cameraSettings.FlipVertically);
            m_storage.SetInt("BarcodeReadingDirection", (int)cameraSettings.BarcodeReadingDirection);
            m_storage.SetInt("SearchAreaAllowedInvalidPixelPercent", cameraSettings.SearchAreaAllowedInvalidPixelPercent);
            m_storage.SetInt("HotspotAllowedChangedPixelPercent", cameraSettings.HotspotAllowedInvalidPixelPercent);
            m_storage.SetBool("InSimulationMode", cameraSettings.InSimulationMode);
            m_storage.SetString("SimulationFile", cameraSettings.SimulationFile);

            // We cant save this to settings. This value is specified by the OS
            //cameraSettings.DrawingSurface

            m_storage.SetInt("SearchAreaDetectionAccuracyX", cameraSettings.SearchAreaDetectionAccuracyX);
            m_storage.SetInt("SearchAreaDetectionAccuracyY", cameraSettings.SearchAreaDetectionAccuracyY);

            m_storage.SetBool("DrawHotspots", cameraSettings.DrawHotspots);
            m_storage.SetBool("AllHotspotsMustHaveChanged", cameraSettings.AllHotspotsMustBeInvalid);

            m_storage.SetInt("HotspotColorR", cameraSettings.HotspotColor.R);
            m_storage.SetInt("HotspotColorG", cameraSettings.HotspotColor.G);
            m_storage.SetInt("HotspotColorB", cameraSettings.HotspotColor.B);

            m_storage.SetInt("HotspotCount", cameraSettings.HotspotCount);
            m_storage.SetInt("HotspotOffsetY", cameraSettings.HotspotOffsetY);

            m_storage.SetInt("HotspotSizeW", cameraSettings.HotspotSize.Width);
            m_storage.SetInt("HotspotSizeH", cameraSettings.HotspotSize.Height);

            m_storage.SetInt("HotspotDetectionColorMinOffset", cameraSettings.HotspotDetectionColorMinOffset);
            m_storage.SetInt("HotspotDetectionColorMaxOffset", cameraSettings.HotspotDetectionColorMaxOffset);

            m_storage.SetInt("HotspotDetectionAccuracyX", cameraSettings.HotspotDetectionAccuracyX);
            m_storage.SetInt("HotspotDetectionAccuracyY", cameraSettings.HotspotDetectionAccuracyY);

            m_storage.SetBool("DrawBlood", cameraSettings.DrawBlood);
            m_storage.SetInt("BloodColorR", cameraSettings.BloodColor.R);
            m_storage.SetInt("BloodColorG", cameraSettings.BloodColor.G);
            m_storage.SetInt("BloodColorB", cameraSettings.BloodColor.B);

            m_storage.SetInt("PunchDetectionAccuracyX", cameraSettings.PunchDetectionAccuracyX);
            m_storage.SetInt("PunchDetectionAccuracyY", cameraSettings.PunchDetectionAccuracyY);

            m_storage.SetInt("FrameRenderInterval", cameraSettings.FrameRenderInterval);
            m_storage.SetInt("FramesSkippedNotificationLimit", cameraSettings.FramesSkippedNotificationLimit);

            m_storage.SetBool("DrawPunchPositions", cameraSettings.DrawPunchPositions);
            m_storage.SetInt("PunchBorder", cameraSettings.PunchBorder);

            m_storage.SetBool("DrawSearchAreaBorder", cameraSettings.DrawSearchAreaBorder);
            m_storage.SetInt("SearchAreaBorderColorR", cameraSettings.SearchAreaBorderColor.R);
            m_storage.SetInt("SearchAreaBorderColorG", cameraSettings.SearchAreaBorderColor.G);
            m_storage.SetInt("SearchAreaBorderColorB", cameraSettings.SearchAreaBorderColor.B);

            m_storage.SetInt("SearchAreaBorderWidth", cameraSettings.SearchAreaBorderWidth);

            m_storage.SetInt("SearchAreaCenterX", cameraSettings.SearchAreaCenter.X);
            m_storage.SetInt("SearchAreaCenterY", cameraSettings.SearchAreaCenter.Y);

            m_storage.SetInt("SearchAreaRadius", cameraSettings.SearchAreaRadius);

            m_storage.SetInt("BloodColorMinR", cameraSettings.BloodColorMin.R);
            m_storage.SetInt("BloodColorMinG", cameraSettings.BloodColorMin.G);
            m_storage.SetInt("BloodColorMinB", cameraSettings.BloodColorMin.B);

            m_storage.SetInt("BloodColorMaxR", cameraSettings.BloodColorMax.R);
            m_storage.SetInt("BloodColorMaxG", cameraSettings.BloodColorMax.G);
            m_storage.SetInt("BloodColorMaxB", cameraSettings.BloodColorMax.B);

            m_storage.SetBool("UseSecondBloodDetection", cameraSettings.UseSecondBloodDetection);

            m_storage.SetInt("BloodColorMin2R", cameraSettings.BloodColorMin2.R);
            m_storage.SetInt("BloodColorMin2G", cameraSettings.BloodColorMin2.G);
            m_storage.SetInt("BloodColorMin2B", cameraSettings.BloodColorMin2.B);

            m_storage.SetInt("BloodColorMax2R", cameraSettings.BloodColorMax2.R);
            m_storage.SetInt("BloodColorMax2G", cameraSettings.BloodColorMax2.G);
            m_storage.SetInt("BloodColorMax2B", cameraSettings.BloodColorMax2.B);

            m_storage.SetInt("SearchAreaDetectionColorMinOffset", cameraSettings.SearchAreaDetectionColorMinOffset);
            m_storage.SetInt("SearchAreaDetectionColorMaxOffset", cameraSettings.SearchAreaDetectionColorMaxOffset);

            m_storage.SetString("BarcodeReaderLocation", cameraSettings.BarcodeReaderLocation);

            m_storage.SetInt("RequiredBloodPercent", cameraSettings.RequiredBloodPercent);
            m_storage.SetString("DriverImplementationFile", cameraSettings.DriverImplementationFile);

            m_storage.SetInt("BarcodeCount", cameraSettings.BarcodeCount);
            m_storage.SetInt("BarcodeReadingInterval1", cameraSettings.BarcodeReadingInterval1);
            m_storage.SetInt("BarcodeReadingInterval2", cameraSettings.BarcodeReadingInterval2);

            if (cameraSettings.CalibrationImage != null) {
                string imagePath = CameraUtils.GetCalibrationImageName(true);
                CameraUtils.SaveCalibrationImage(cameraSettings.CalibrationImage, imagePath);
            } else {
                m_storage.SetString("CalibrationImageName", "");
            }


            #region Camera registry settings
            m_storage.SetInt("RegSettingTimingExposure", cameraSettings.RegSettingTimingExposure);
            m_storage.SetInt("Cam2RegSettingTimingExposure", cameraSettings.Cam2RegSettingTimingExposure);

            // TODO: Other camera registry settings omitted for time being
            #endregion

            // TODO: save these settings when we support more
            //cameraSettings.CameraConnection = PuncherCameraConnectionType.DirectShowCamera;
            //cameraSettings.BarcodeReader = BarcodeReaderType.DTK;
            //cameraSettings.InputImage = InputImageType.Color;
            //cameraSettings.BloodDetection = BloodDetectionMethod.DetectBlood;

            #region Sample barcode settings

            m_storage.SetBool("EnableCode128", cameraSettings.EnableCode128);
            m_storage.SetBool("EnableInterleaved2of5", cameraSettings.EnableInterleaved2of5);
            m_storage.SetBool("EnableCode39", cameraSettings.EnableCode39);
            m_storage.SetBool("EnableDataMatrix", cameraSettings.EnableDataMatrix);

            #endregion

            m_storage.SetBool("EnableScanRect1", cameraSettings.EnableScanRect1);
            m_storage.SetInt("ScanRect1Left", cameraSettings.ScanRect1Left);
            m_storage.SetInt("ScanRect1Top", cameraSettings.ScanRect1Top);
            m_storage.SetInt("ScanRect1Right", cameraSettings.ScanRect1Right);
            m_storage.SetInt("ScanRect1Bottom", cameraSettings.ScanRect1Bottom);

            m_storage.SetBool("EnableScanRect2", cameraSettings.EnableScanRect2);
            m_storage.SetInt("ScanRect2Left", cameraSettings.ScanRect2Left);
            m_storage.SetInt("ScanRect2Top", cameraSettings.ScanRect2Top);
            m_storage.SetInt("ScanRect2Right", cameraSettings.ScanRect2Right);
            m_storage.SetInt("ScanRect2Bottom", cameraSettings.ScanRect2Bottom);

            m_storage.SetBool("EnableScanRect3", cameraSettings.EnableScanRect3);
            m_storage.SetInt("ScanRect3Left", cameraSettings.ScanRect3Left);
            m_storage.SetInt("ScanRect3Top", cameraSettings.ScanRect3Top);
            m_storage.SetInt("ScanRect3Right", cameraSettings.ScanRect3Right);
            m_storage.SetInt("ScanRect3Bottom", cameraSettings.ScanRect3Bottom);

            m_storage.SetInt("BarcodeRedundancy", cameraSettings.BarcodeRedundancy);
            m_storage.SetInt("MinimumBarcodeHeight", cameraSettings.MinimumBarcodeHeight);

            Config.Instance.Save();

        }

        /// <summary>
        /// Take picture. Next image that is received from the camera, is captured and returned.
        /// Returns null, if capturing failed
        /// </summary>
        public Bitmap TakePicture() {
            if (m_Initialized) {
                switch (m_CameraConnection) {
                    case PuncherCameraConnectionType.DirectShowCamera:
                        return m_DSCameraDriver.TakePicture();
                }
            }
            throw new InvalidOperationException();
        }
          
        /// <summary>
        /// Grabs currently visible picture to be returned later by TakePicture
        /// </summary>
        public void GrabPicture() {
            if (m_Initialized) {
                switch (m_CameraConnection) {
                    case PuncherCameraConnectionType.DirectShowCamera:
                        m_DSCameraDriver.GrabPicture();
                        return;
                }
            }
            throw new InvalidOperationException();
        }

        /// <summary>
        /// Event that is raised with a little delay when exception has occured in camera driver
        /// </summary>
        public event CameraDriverExceptionOccuredDelegate CameraDriverExceptionOccured;

        /// <summary>
        /// Raised when the count of found punch positions have changed
        /// </summary>
        public event PunchPositionCountChangedDelegate PunchPositionCountChanged;

        /// <summary>
        /// Raised when the count of found barcodes have changed
        /// </summary>
        public event BarcodesUpdatedDelegate BarcodesUpdated;

        /// <summary>
        /// Raised when calculated hotsopts have changed (= blood card is detected from the image)
        /// </summary>
        public event BloodCardDetectedDelegate BloodCardDetected;

        /// <summary>
        /// Raised when hotspot info has returned the same as it was when the info was calculated
        /// (= blood card was removed from the image)
        /// </summary>
        public event BloodCardRemovedDelegate BloodCardRemoved;

        /// <summary>
        /// Raised when the amount specifed in IPuncherCameraSettings.FramesSkippedNotificationLimit
        /// is exceeded. When for e.g. applying new settings, the actual iimage renderer is skipped
        /// and the image isn´t processed because the data and settings that are required, is locked
        /// by the update process. Basically this means that the lock isn´t released.
        /// </summary>
        public event FramesSkippedNotificationDelegate FramesSkippedNotification;

        /// <summary>
        /// Set/get the punch positions that are searched/found from image. The object (IPunchPositions)
        /// is a local copy, and therefore it can´t be stored and used to modify the positions.
        /// </summary>
        public IPunchPositions PunchPositions {
            get {

                if (m_Initialized) {
                    switch (m_CameraConnection) {
                        case PuncherCameraConnectionType.DirectShowCamera:

                            if (m_DSCameraDriver != null) {
                                return m_DSCameraDriver.PunchPositions;
                            }

                            break;
                    }
                }

                throw new InvalidOperationException();

            }
            set {

                if (m_Initialized) {
                    switch (m_CameraConnection) {
                        case PuncherCameraConnectionType.DirectShowCamera:

                            if (m_DSCameraDriver != null) {
                                m_DSCameraDriver.PunchPositions = value;
                            }

                            break;
                    }
                }

            }
        }

        /// <summary>
        /// Get/set current settings.
        /// </summary>
        public IPuncherCameraSettings Settings {
            get {

                // Must be initialized
                if (m_Initialized) {
                    switch (m_CameraConnection) {

                        case PuncherCameraConnectionType.DirectShowCamera:
                            IPuncherCameraSettings s = m_DSCameraDriver.Settings;
                            if (s != null) {
                                s.DriverImplementationFile = m_DriverLocation;
                            }

                            return s;
                    }
                }

                throw new InvalidOperationException();

            }
            set {

                // Must be initialized
                if (m_Initialized) {
                    switch (m_CameraConnection) {

                        case PuncherCameraConnectionType.DirectShowCamera:
                            m_DSCameraDriver.Settings = value;

                            // Cache PunchPositionCountChanged-event sending interval
                            m_PunchCountChangedEventSendingInterval = value.PunchCountChangedEventSendingInterval;
                            break;
                    }
                } else {
                    throw new InvalidOperationException();
                }
            }
        }
        
        /// <summary>
        /// The barcode read, or null if nothing has been read.
        /// </summary>
        public Barcode Barcode {
            get {
                if (m_Initialized) {
                    switch (m_CameraConnection) {
                        case PuncherCameraConnectionType.DirectShowCamera:

                            if (m_DSCameraDriver != null) {
                                string[] readCode = m_DSCameraDriver.Barcode;
                                if (readCode != null) {
                                    if (readCode.Length >= 2) {
                                        // By specs, DSCameraDriver.Barocde returns two element string vector, where first element
                                        // is the actual barcode and the second element the barcode type as string
                                        Barcode retVal = new Barcode(BarcodeTypeConverter.FromString(readCode[1]), readCode[0]);
                                        return retVal;
                                    }
                                }
                                return null;
                            }

                            break;
                    }
                }

                throw new InvalidOperationException();

            }
        }

        /// <summary>
        /// Returns true when card has been detected
        /// </summary>
        public bool CardDetected {
            get {

                if (m_Initialized) {
                    switch (m_CameraConnection) {
                        case PuncherCameraConnectionType.DirectShowCamera:

                            if (m_DSCameraDriver != null) {
                                return m_DSCameraDriver.CardDetected;
                            }

                            break;
                    }
                }

                throw new InvalidOperationException();

            }
        }

        /// <summary>
        /// Force camera to display specified image. In order to switch back to live image,
        /// this method must be called with null argument. The size of the image must be the
        /// same that was used when camera was initialized
        /// </summary>
        /// <param name="imageToShow">Image to display, or null to switch back live image</param>
        public void DisplayImage(Bitmap imageToShow) {

            if (m_Initialized) {
                switch (m_CameraConnection) {

                    case PuncherCameraConnectionType.DirectShowCamera:
                        m_DSCameraDriver.DisplayImage(imageToShow);
                        break;
                }
            } else {
                throw new InvalidOperationException();
            }

        }

        /// <summary>
        /// Update camera drawing surface. Camera must be initialized in order to update the drawing 
        /// surface. Drawing surface must be a native window handle (Win32 HWND)
        /// </summary>
        /// <param name="newDrawingSurface">New drawing surface (Win32 HWND)</param>
        public void SetDrawingSurface(IntPtr newDrawingSurface) {

            if (m_Initialized) {
                switch (m_CameraConnection) {

                    case PuncherCameraConnectionType.DirectShowCamera:
                        m_DSCameraDriver.SetDrawingSurface(newDrawingSurface);
                        break;
                }
            } else {
                throw new InvalidOperationException();
            }

        }

        /// <summary>
        /// Punch Border, the area outside the actual punch area, that is reserved for punch position inaccuracies, the
        /// border area needs to be in blood too (assuming that the punch area needs to be in blood), and no other punches
        /// may be even partially positioned on this border area.
        /// </summary>
        public byte PunchBorder {
            get {
                if (m_Initialized) {
                    switch (m_CameraConnection) {
                        case PuncherCameraConnectionType.DirectShowCamera:
                            return m_DSCameraDriver.PunchBorder;
                    }
                }
                throw new InvalidOperationException();
            } 
            set {
                string me = System.Reflection.MethodInfo.GetCurrentMethod().Name;
                m_logger.Debug(me, "Setting {0}", value);
                if (m_storage != null) {
                    m_storage.SetInt("PunchBorder", value);
                }
                if (m_Initialized) {
                    switch (m_CameraConnection) {
                        case PuncherCameraConnectionType.DirectShowCamera:
                            m_DSCameraDriver.PunchBorder = value;
                            break;
                    }
                } else {
                    // Camera may not be initialized, as the intention is to set this border only to settings, no reason to throw the
                    // exception
                }
            }
        }

        /// <summary>
        /// Blood Border, the area in the outskirts of detected blood, that is reserved for punchhead-to-camera position inaccuracies.
        /// </summary>
        public byte BloodBorder {
            get {
                if (m_Initialized) {
                    switch (m_CameraConnection) {
                        case PuncherCameraConnectionType.DirectShowCamera:
                            return m_DSCameraDriver.PunchBorder;
                    }
                }
                throw new InvalidOperationException();
            }
            set {
                string me = System.Reflection.MethodInfo.GetCurrentMethod().Name;
                m_logger.Debug(me, "Setting {0}", value);
                if (m_Initialized) {
                    switch (m_CameraConnection) {
                        case PuncherCameraConnectionType.DirectShowCamera:
                            m_DSCameraDriver.BloodBorder = value;
                            break;
                    }
                } else {
                    // Camera may not be initialized, as the intention is to set this border only to settings, no reason to throw the
                    // exception
                }
            }
        }

        /// <summary>
        /// Whether the blood detection is on or off. 
        /// </summary>
        public bool BloodDetectionOn {
            get {
                if (m_Initialized) {
                    switch (m_CameraConnection) {
                        case PuncherCameraConnectionType.DirectShowCamera:
                            return m_DSCameraDriver.BloodDetectionOn;
                    }
                }
                return false;
            }
            set {
                if (m_Initialized) {
                    switch (m_CameraConnection) {
                        case PuncherCameraConnectionType.DirectShowCamera:
                            m_DSCameraDriver.BloodDetectionOn = value;
                            return;
                    }
                }
            }
        }

        public void EnterStandby() {
            string me = System.Reflection.MethodInfo.GetCurrentMethod().Name;
            m_logger.Debug(me, "Started");
            if (m_Initialized) {
                switch (m_CameraConnection) {

                    case PuncherCameraConnectionType.DirectShowCamera:
                        m_DSCameraDriver.EnterStandby();
                        break;
                }
            }
        }

        public void LeaveStandby() {
            string me = System.Reflection.MethodInfo.GetCurrentMethod().Name;
            m_logger.Debug(me, "Started");
            if (m_Initialized) {
                switch (m_CameraConnection) {

                    case PuncherCameraConnectionType.DirectShowCamera:
                        m_DSCameraDriver.LeaveStandby();
                        break;
                }
            }
        }

        /// <summary>
        /// Maximum intensity for blood
        /// </summary>
        public Color BloodDetectionColorMax {
            get {
                switch (m_CameraConnection) {

                    case PuncherCameraConnectionType.DirectShowCamera:
                        return Color.FromArgb(m_DSCameraDriver.BloodDetectionColorMax, m_DSCameraDriver.BloodDetectionColorMax, m_DSCameraDriver.BloodDetectionColorMax);
                }
                throw new NotImplementedException();
            }
            set {
                switch (m_CameraConnection) {

                    case PuncherCameraConnectionType.DirectShowCamera:
                        int sum = value.R + value.G + value.R;
                        m_DSCameraDriver.BloodDetectionColorMax = (byte)(sum / 3);
                        return;
                }
                throw new NotImplementedException();
            }
        }

        #endregion


        #region Private data

        /// <summary>
        /// Helper used in logging
        /// </summary>
        private const string CLASSNAME = "Camera";

        /// <summary>
        /// Helper used in logging
        /// </summary>
        private const string SEP = ".";


        /// <summary>
        /// True when camera has been initialized succesfully
        /// </summary>
        private bool m_Initialized = false;

        /// <summary>
        /// Thread which is listening native windows event (Win32 HEVENT) that gets signaled when
        /// updated information is available in actual image renderer.
        /// </summary>
        private Thread m_DSCameraNotificationListener = null;

        /// <summary>
        /// Flag which is used to notify notification listener thread (m_DSCameraNotificationListener) that it should close
        /// </summary>
        private bool m_Closing = false;

        /// <summary>
        /// Type of current camera connection
        /// </summary>
        private PuncherCameraConnectionType m_CameraConnection = PuncherCameraConnectionType.DirectShowCamera;

        /// <summary>
        /// DirectShow camera driver
        /// </summary>
        private IPuncherDSCameraDriver m_DSCameraDriver = null;

        /// <summary>
        /// Camera driver location
        /// </summary>
        private string m_DriverLocation = string.Empty;

        /// <summary>
        /// Interval, in milliseconds, that is used when sending PunchCountChanged-event. This is cached
        /// when camera is initialized or when settings are updated. This is used in the event listener
        /// thread (m_DSCameraNotificationListener), when sending PunchPositionCountChanged-event.
        /// </summary>
        private int m_PunchCountChangedEventSendingInterval = 500;

        #endregion


        #region Private methods

        /// <summary>
        /// Initialize direct show camera. Camera is loaded dynamically from IPuncherCameraSettings.DriverImplementationFile
        /// and the first class that implements the IPuncherDSCameraDriver-interface is used.
        /// </summary>
        private void InitializeDSCamera(IPuncherCameraSettings settings) {

            const string METHODNAME = "InitializeDSCamera";
            const string PROCNAME = CLASSNAME + SEP + METHODNAME;
            Log.Debug(PROCNAME, LogMessages.Debug.InitializeDSCamera, settings.DriverImplementationFile);
            
            // Cache, used later if settings are fetched
            m_DriverLocation = settings.DriverImplementationFile;
            

            string interfaceName = typeof(IPuncherDSCameraDriver).ToString();

            if (!File.Exists(settings.DriverImplementationFile)) {
                m_logger.Debug(METHODNAME, "Driver implementation file does not exist");
            } else {
                m_logger.Debug(METHODNAME, "Driver implementation file exists");
            }

            // Load
            Assembly driverImpl = Assembly.LoadFrom(settings.DriverImplementationFile);
            Type[] types = driverImpl.GetTypes();

            m_logger.Debug(METHODNAME, "Found {0} types", types.Length);
            

            // Find the first class that implements the IPuncherDSCameraDriver interface
            foreach (Type type in types) {
                if (type.GetInterface(interfaceName) != null) {

                    // Invoke default constructor
                    ConstructorInfo ctorInfo = type.GetConstructor(Type.EmptyTypes);
                    m_DSCameraDriver = (IPuncherDSCameraDriver)ctorInfo.Invoke(null);
                    break;
                }
            }
            m_Closing = false;

            if (m_DSCameraDriver != null) {
                // Initialize
                m_DSCameraDriver.Initialize(settings);
            } else {
                throw new PuncherCameraException(CLASSNAME, METHODNAME, Constants.ErrorCodes.ERR_CAMERA_DSDRIVER_IMPL_NOT_FOUND, 0);
            }

        }

        /// <summary>
        /// Raise PunchPositionCountChanged-event
        /// </summary>
        private void ThrowPunchPositionCountChanged() {

            PunchPositionCountChangedDelegate e = PunchPositionCountChanged;
            if (e != null) {
                e();
            }

        }

        /// <summary>
        /// Raise BarcodesUpdated-event
        /// </summary>
        private void ThrowBarcodesUpdated() {

            BarcodesUpdatedDelegate e = BarcodesUpdated;
            if (e != null) {
                e();
            }

        }

        /// <summary>
        /// Raise ThrowBloodCardDetected-event
        /// </summary>
        private void ThrowBloodCardDetected() {

            BloodCardDetectedDelegate e = BloodCardDetected;
            if (e != null) {
                e();
            }

        }

        /// <summary>
        /// Raise BloodCardRemoved-event
        /// </summary>
        private void ThrowBloodCardRemoved() {

            BloodCardRemovedDelegate e = BloodCardRemoved;
            if (e != null) {
                e();
            }

        }

        /// <summary>
        /// Raise ThrowCameraDriverException-event
        /// </summary>
        private void ThrowCameraDriverException() {

            CameraDriverExceptionOccuredDelegate a = CameraDriverExceptionOccured;
            if (a != null) {

                switch (m_CameraConnection) {
                    case PuncherCameraConnectionType.DirectShowCamera:

                        if (m_DSCameraDriver != null) {

                            PuncherCameraException pce = m_DSCameraDriver.CameraDriverException;
                            if (pce != null) {
                                a(pce);
                            }

                        }

                        break;
                }
            }

        }

        /// <summary>
        /// Raise FramesSkippedNotification-event
        /// </summary>
        private void ThrowFramesSkippedNotification() {

            FramesSkippedNotificationDelegate a = FramesSkippedNotification;
            if (a != null) {
                a();
            }

        }

        /// <summary>
        /// Thread entry point fro the (DS camera) notification listener. This thread will listen native 
        /// windows events (Win32 HEVENT) that are specified in the (IPuncherDSCameraDriver) m_DSCameraDriver.
        /// When event get signaled, possible parameters ar collected and and correct activation method is
        /// called in the camera object (passed as argument).
        /// 
        /// Thread will excute as long as the m_Closing member in camera is false.
        /// </summary>
        /// <param name="o">Camera object</param>
        private static void CameraDriverNotificationListener(object o) {

            const string PROCNAME = CLASSNAME + SEP + "CameraDriverNotificationListener";

            Camera cam = null;
            if (o != null) {
                try {

                    cam = (Camera)o;

                    // Get handles that we can listen
                    ManualResetEvent punchPositionCountChangedEvent = new ManualResetEvent(false);
                    punchPositionCountChangedEvent.SafeWaitHandle = new SafeWaitHandle(
                      cam.m_DSCameraDriver.PunchPositionCountChangedEventHandle, false);

                    ManualResetEvent bloodCardDetectedEvent = new ManualResetEvent(false);
                    bloodCardDetectedEvent.SafeWaitHandle = new SafeWaitHandle(
                      cam.m_DSCameraDriver.BloodCardDetectedEventHandle, false);

                    ManualResetEvent bloodCardRemovedEvent = new ManualResetEvent(false);
                    bloodCardRemovedEvent.SafeWaitHandle = new SafeWaitHandle(
                      cam.m_DSCameraDriver.BloodCardRemovedEventHandle, false);

                    ManualResetEvent barcodesUpdatedEvent = new ManualResetEvent(false);
                    barcodesUpdatedEvent.SafeWaitHandle = new SafeWaitHandle(
                      cam.m_DSCameraDriver.BarcodesUpdatedEventHandle, false);

                    ManualResetEvent cameraDriverExceptionEvent = new ManualResetEvent(false);
                    cameraDriverExceptionEvent.SafeWaitHandle = new SafeWaitHandle(
                      cam.m_DSCameraDriver.CameraDriverExceptionEventHandle, false);

                    ManualResetEvent framesSkippedNotification = new ManualResetEvent(false);
                    framesSkippedNotification.SafeWaitHandle = new SafeWaitHandle(
                      cam.m_DSCameraDriver.FramesSkippedNotificationEventHandle, false);



                    // NOTE: If you change the event order, remember to update the switch clause also
                    WaitHandle[] events = new WaitHandle[] {
                        punchPositionCountChangedEvent,
                        bloodCardDetectedEvent,
                        bloodCardRemovedEvent,
                        barcodesUpdatedEvent,
                        cameraDriverExceptionEvent,
                        framesSkippedNotification };


                    // Loop while executing
                    while (!cam.m_Closing) {

                        switch (WaitHandle.WaitAny(events, 200)) {

                            case 0:
                                // Punch position count changed
                                cam.ThrowPunchPositionCountChanged();
                                punchPositionCountChangedEvent.Reset();
                                Thread.Sleep(cam.m_PunchCountChangedEventSendingInterval);
                                break;

                            case 1:
                                // Bloodcard detected
                                cam.ThrowBloodCardDetected();
                                bloodCardDetectedEvent.Reset();
                                break;

                            case 2:
                                // Bloodcard removed
                                cam.BloodCardRemoved?.Invoke();
                                bloodCardRemovedEvent.Reset();
                                break;

                            case 3:
                                // Barcodes updated
                                cam.ThrowBarcodesUpdated();
                                barcodesUpdatedEvent.Reset();
                                break;

                            case 4:
                                // Camera driver exception occured
                                cam.ThrowCameraDriverException();
                                cameraDriverExceptionEvent.Reset();
                                break;

                            case 5:
                                // Framse skipped
                                cam.ThrowFramesSkippedNotification();
                                framesSkippedNotification.Reset();
                                break;


                            default:
                                break;

                        }

                    }

                    // Release handles
                    punchPositionCountChangedEvent.Reset();
                    bloodCardDetectedEvent.Reset();
                    bloodCardRemovedEvent.Reset();
                    barcodesUpdatedEvent.Reset();
                    cameraDriverExceptionEvent.Reset();
                    framesSkippedNotification.Reset();

                } catch (Exception exc) {
                    Log.Error(PROCNAME, exc, LogMessages.Error.ErrDSNotificationListener, exc.Message, exc.StackTrace);
                }
            }

        }

        #endregion

    }

}
