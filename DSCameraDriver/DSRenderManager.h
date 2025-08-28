// --------------------------------------------------------------------------
// DSRenderManager.h
// --------------------------------------------------------------------------
//
// SpotLight DirectShow camera manager object, used to create and hold required
// DirectShow COM objects.
//
// When initializing DS objects and creating graph builder (m_pGraph), thread is
// started whose sole purpose currently is to listen device lost event from the
// DS runtime - this event means that the connection to camera was lost.
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 04.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

#pragma once

#include "DSCameraRenderer.h"
#include "BarcodeReader.h"


typedef struct _DSRENDERMANAGERSETTINGS {

  BYTE                    InSimulationMode;                   // When TRUE, camera is run in simulation mode
  PTCHAR                  pszSimulationFile;                  // Path to file which is used when in simulation mode

  HWND                    DrawingSurface;                     // Handle to window where we should draw the image

  BYTE                    DrawPunchPositions;                 // When TRUE, found punch positions are drawn
  BYTE                    PunchBorder;                        // Required blood (in pixels) that must be between
                                                              // each punch
  // --------------------------------------------------------------------------------------------
  // Barcode reader settings
  /*BYTE                    BarcodeCount;                       // Number of barcodes we try to read from the image

  USHORT                  BarcodeReadingInterval1;            // Barcode reading interval in milliseconds. This is used as
                                                              // a intervall between read attempts when we don´t have any 
                                                              // barcodes in cache

  USHORT                  BarcodeReadingInterval2;            // Barcode reading interval in milliseconds. This is used as
                                                              // a intervall between read attempts when barcode cache contains 
                                                              // barcodes

  PTCHAR                  pszBCReaderLocation;                // Barcode reader (DLL) location

  BYTE                    BcOrientation;                      // Specifies which way barcodes are detected from the image.
                                                              // This is a bit field, valid values are.
                                                              // 1 (BCR_RD_LEFT_TO_RIGHT) = Left to right
                                                              // 2 (BCR_RD_RIGHT_TO_LEFT) = Right to left
                                                              // 4 (BCR_RD_TOP_TO_BOTTOM) = Top to bottom
                                                              // 8 (BCR_RD_BOTTOM_TO_TOP) = Bottom to top*/
  BARCODEREADERSETTINGS   barcodeReaderSettings;

  // --------------------------------------------------------------------------------------------
  // etc

  COLORREF                BloodColorMin;                      // Min color value for blood. Image pixel is recoknised to blood 
                                                              // if its between BloodColorMin and BloodColorMax
  
  COLORREF                BloodColorMax;                      // Max color value for blood. Image pixel is recoknised to blood 
                                                              // if its between BloodColorMin and BloodColorMax


  BYTE                    UseSecondBloodDetection;            // When TRUE, second blood detection values (BloodColorMin2 and
                                                              // BloodColorMax2) are used for punch positions that was found
                                                              // earlier. If FALSE, the normal blood values are used (BloodColorMin
                                                              // and BloodColorMax)

  COLORREF                BloodColorMin2;                     // Min color value for second blood detection (UseSecondBloodDetection).
                                                              // Image pixel is recoknised to blood if its between BloodColorMin and 
                                                              // BloodColorMax

  COLORREF                BloodColorMax2;                     // Max color value for second blood detection (UseSecondBloodDetection).
                                                              // Image pixel is recoknised to blood if its between BloodColorMin and 
                                                              // BloodColorMax


  eBloodDetectionMethod   BloodDetectionMethod;               // Blood detection method

  BYTE                    DrawSearchAreaBorder;               // When TRUE, search area border is drawn
  COLORREF                SearchAreaBorderColor;              // Color for search area border
  BYTE                    SearchAreaBorderWidth;              // Search area border width

  INDEXVALUERANGE         SearchAreaColorOffsets;             // Min/max values which are used in search area hotspot activation
  
  BYTE                    SearchAreaDetectionAccuracyX;       // X Accuracy for search area hotspot creation. This value specifies 
                                                              // how many pixel is skipped when collecting search area hotspot data

  BYTE                    SearchAreaDetectionAccuracyY;       // Y Accuracy for search area hotspot creation. This value specifies 
                                                              // how many pixel is skipped when collecting search area hotspot data

  BYTE                  SearchAreaAllowedInvalidPixelPercent; // Number of pixel (percent) that needs to be invalid in
                                                              // search are hotspot before it is considered invalid. If this is
                                                              // 0, one invalid pixel will activate hotspot

  SIZE                    UiImageSize;                        // UI image size
  eInputImageType         InputImage;                         // Input image type

  BYTE                    RequiredBloodPercent;               // Percent that search area must contain blood before punch
                                                              // detection is started. If this is zero and detection area
                                                              // contains at least one pixel of blood, punch detection is
                                                              // started.

  POINT                   SearchAreaCenter;                   // Center point of search area
  USHORT                  SearchAreaRadius;                   // Search area radius

  POINT                   SearchAreaTopLeft;                  // Search area top left corner
  POINT                   SearchAreaBottomRight;              // Search area bottom right corner

  BYTE                    PunchDetectionAccuracyX;            // Punch detection increment, X-axis
  BYTE                    PunchDetectionAccuracyY;            // Punch detection increment, Y-axis

  USHORT                  FrameRenderInterval;                // Number of milliseconds between each drawn frame

  UINT                    FramesSkippedNotificationLimit;     // Time in milliseconds that must elapse before error is send 
                                                              // that frames processing is skipped because camere cant access 
                                                              // shared data

  BYTE                    DrawBlood;                          // When TRUE, found blood is also drawn
  COLORREF                BloodColor;                         // Color which is used to draw the blood

  BYTE                    DrawHotspots;                       // When TRUE, hotspots, which are used to detect whether
                                                              // card is present or not, are drawn

  COLORREF                HotspotColor;                       // Color for hotspot drawing
  BYTE                    HotspotCount;                       // Hotspot count

  USHORT                  HotspotOffsetY;                     // Hotspot offset from the bottom of the image (row 0).
                                                              // Used when placing the center of the hotspots

  SIZE                    HotspotSize;                        // Size of one hotspot

  INDEXVALUERANGE         HotspotColorOffsets;                // Min/max pixel values that is used in hotspot activation

  BYTE                    HotspotDetectionAccuracyX;          // Increment in X axis that is used when calculating hotspot
                                                              // index values
  BYTE                    HotspotDetectionAccuracyY;          // Increment in Y axis that is used when calculating hotspot
                                                              // index values

  BYTE                    HotspotAllowedInvalidPixelPercent;  // Number of pixel (percent) that needs to be invalid before
                                                              // hotspot is activated

  BYTE                    AllHotspotsMustBeInvalid;           // When TRUE, all hotspot must be invalid before thay are activated


  PBYTE                   CalibrationImage;                   // Image that can be used to calibrate the camera. If this is
                                                              // NULL, automatic calibration is used, which uses the next
                                                              // image that is received from the camera

  BYTE                    FlipHorizontally;                   // When TRUE, received image is flipped horizontally
  BYTE                    FlipVertically;                     // When TRUE, received image is flipped vertically

} DSRENDERMANAGERSETTINGS, *PDSRENDERMANAGERSETTINGS;



class DSRenderManager {

  public:

    DSRenderManager();
    ~DSRenderManager();

	// Get camera image width in pixels
	inline int            GetCameraImageWidth() { return m_cameraImageWidth; }

	// Get camera image height in pixels
	inline int            GetCameraImageHeight() { return m_cameraImageHeight; }

	// Get camera image width in mm
	inline double         GetCameraImageWidth_mm() { return m_cameraImageWidth_mm; }

	// Get camera image height in mm
	inline double         GetCameraImageHeight_mm() { return m_cameraImageHeight_mm; }

	// Get the camera type
	inline PTCHAR         GetCameraType() { return m_cameraType; }

	// Initialize the camera
	void InitializeCamera();

    // Create actual image renderer (m_pRenderer) and establish connection between it and the
    // DirectShow runtime. BarcodeReader, PunchDetector and BloodDetector is also created
    void Initialize(PDSRENDERMANAGERSETTINGS pSettings);

    // Release actual renderer. This will break the connection between DirectShow runtime and our renderer
    void Release();

    // Start playback. Tell DirectShow runtime that we are ready to receive images
    void Start();

    // Stop playback. Tell DirectShow runtime that we dont want to receive images
    void Stop();

    // Get current settings
    void GetCurrentSettings(PDSRENDERMANAGERSETTINGS pSettings);

    // Update settings
    void UpdateSettings(PDSRENDERMANAGERSETTINGS pSettings);

    // Get actual image renderer
    inline DSCameraRenderer* GetCameraRenderer() { return m_pRenderer; }

    // Punch Border, the area outside the actual punch area, that is reserved for punch position inaccuracies, the
    // border area needs to be in blood too (assuming that the punch area needs to be in blood), and no other punches
    // may be even partially positioned on this border area.
    void SetPunchBorder(BYTE border);
    BYTE GetPunchBorder();


  private:

                        // When TRUE, object is initialized
    BYTE                m_bInitialized;

                        // When TRUE, in simulation mode
    BYTE                m_bInSimulationMode;

                        // Name of the file that is used in simulation mode
    PTCHAR              m_pszSimulationFile;

                        // Actual image renderer. This is connected to DirectSHow runtime. THe DS runtime
                        // will call this class when its time to render image
    DSCameraRenderer*   m_pRenderer;

                        // DirectShow object that will hold the connection between camera and out code, the renderer
    IGraphBuilder*      m_pGraph;

                        // DirectShow object that is used to control the playback - the data flow in the filter graph
                        // (m_pGraph). Through this object the "playback" can be paused and resumed.
    IMediaControl*      m_pPlayer;

                        // Interface used to listen events from the DS graph builder (m_pGraph). A dedicated
                        // thread is created
    IMediaEvent*        m_pGraphEvents;

                        // HAndle to thread who will listen events that are received from the DS graph event
                        // listener (m_pGraphEvents). 
    HANDLE              m_hDSMediaEventListener;


	                    // The camera filter
	IBaseFilter*        m_pCameraFilter;

	                    // Camera image width/height in pixels
	int                 m_cameraImageWidth;
	int                 m_cameraImageHeight;

	                    // Camera image width/height in mm
	double              m_cameraImageWidth_mm;
	double              m_cameraImageHeight_mm;

	                    // Camera type
	PTCHAR              m_cameraType;

                        // Build the connection between DirectShow runtime and our code
    void                InitializeDSConnection();
    
                        // Throw exception if class isn´t initialized (m_bInitialized = FALSE)
    void                ThrowNotInitializedException(const PTCHAR pszMethod);
    
                        // Throw exception if class is already initialized (m_bInitialized = TRUE)
    void                ThrowAlreadyInitializedException(const PTCHAR pszMethod);
    
                        // Get settings that are specific to image renderer
    void                GetRendererSettings(const PDSRENDERMANAGERSETTINGS pSettings, PCAMERARENDERERSETTINGS pCRSettings);

                        // Helper method to check fi pin is already connected
    HRESULT             IsPinConnected(IPin *pPin, BYTE *pResult);
    
                        // Helper method to check if pins direction is the specified
    HRESULT             IsPinDirection(IPin *pPin, PIN_DIRECTION dir, BYTE *pResult);
    
                        // Helper method to find pin that hav the specified direction
    HRESULT             MatchPin(IPin *pPin, PIN_DIRECTION direction, BYTE bShouldBeConnected, BYTE *pResult);
    
                        // Helper method to find unconnected pin
    HRESULT             FindUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);

                        // Create and initialize barcode reader
    BarcodeReader*      CreateBarcodeReader(PDSRENDERMANAGERSETTINGS pSettings);
    
                        // Create and initialize punch detector
    PunchDetector*      CreatePunchDetector(PDSRENDERMANAGERSETTINGS pSettings);
    
                        // Create and initialize blood detector
    BloodDetector*      CreateBloodDetector(BYTE useSecondBloodColors, PDSRENDERMANAGERSETTINGS pSettings);

                        // Close the thread that is listening events from the DS graph builder
    void                CloseDSEventListenerThread();

                        // Create and start thread who will listen events from the DS graph builder
    void                CreateDSEventListenerThread();

                        // This method will enumerate all the cameras that are installed to this system, and
                        // return the first where the filter can be obtained - camera is connected and therefore
                        // can be initialized
    IBaseFilter*        FindCamera();

	                    // Get the camera settings
	void                GetCameraSettings(LPCSTR keyPath);

};
