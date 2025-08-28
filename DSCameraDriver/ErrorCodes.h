// --------------------------------------------------------------------------
// ErrorCodes.h
// --------------------------------------------------------------------------
//
// Error code declarations
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 03.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

#pragma once


// --------------------------------------------------------------------------
// General errors


#define ERR_EXCEPTIONINFO_IS_NULL                                   10    // Exception info is null
#define ERR_INVALID_X_COORD                                         11    // Invalid (too small/big) X coord
#define ERR_INVALID_Y_COORD                                         12    // Invalid (too small/big) Y coord



// --------------------------------------------------------------------------
// BloodDetector

#define ERR_BLOODDETECTOR_IMAGE_DATA_IS_NULL                        100   // Image data is null
#define ERR_BLOODDETECTOR_IMAGE_DATA_ACCESS_VIOLATION               101   // Invalid image data
#define ERR_BLOODDETECTOR_NOT_INITIALIZED                           102   // Blood detector isn´t initialized
#define ERR_BLOODDETECTOR_BD_ALREADY_INITIALIZED                    103   // Blood detector is already initialized
#define ERR_BLOODDETECTOR_BLOODMAP_IS_NULL                          104   // Bloodmap is null



// --------------------------------------------------------------------------
// DSCamera

#define ERR_DSCAMERA_INITIALIZE_FAILED                              200   // Unexpected error occured while initializing camera
#define ERR_DSCAMERA_RELEASE_FAILED                                 201   // Unexpected error occured while releasing camera
#define ERR_DSCAMERA_START_FAILED                                   202   // Unexpected error occured while starting player
#define ERR_DSCAMERA_STOP_FAILED                                    203   // Unexpected error occured while stopping player
#define ERR_DSCAMERA_PUNCHPOSITIONS_GET_FAILED                      205   // Unexpected error occured while returning punch positions info
#define ERR_DSCAMERA_PUNCHPOSITIONS_SET_NULL_ARG                    206   // Punch positions argument is null
#define ERR_DSCAMERA_PUNCHPOSITIONS_SET_FAILED                      207   // Unexpected error occured while updating punch positions info
#define ERR_DSCAMERA_SETTINGS_IS_NULL                               208   // Settings aregument is null
#define ERR_DSCAMERA_PPCOUNTCHANGEDEVENTHANDLE_GET_FAILED           209   // Unexpected error occured while returning event handle
#define ERR_DSCAMERA_BLOODCARDDETECTEDEVENTHANDLE_GET_FAILED        210   // Unexpected error occured while returning event handle
#define ERR_DSCAMERA_BLOODCARDREMOVEDEVENTHANDLE_GET_FAILED         211   // Unexpected error occured while returning event handle
#define ERR_DSCAMERA_BARCODESUPDATEDEVENTHANDLE_GET_FAILED          212   // Unexpected error occured while returning event handle
#define ERR_DSCAMERA_BARCODES_GET_FAILED                            213   // Unexpected error occured while returning barcodes
#define ERR_DSCAMERA_SIMULATIONFILE_MISSING                         214   // Simulation file is missing or null
#define ERR_DSCAMERA_BCREADER_LOCATION_MISSING                      215   // BC reader location is missing or null
#define ERR_DSCAMERA_BLOODCOLOR_MIN_GREATER_THEN_MAX                218   // Blood color min value is greater than max value
#define ERR_DSCAMERA_DRAWINGSURFACE_IS_INVALID                      219   // Drawing surface is invalid
#define ERR_DSCAMERA_EMPTYIMAGEINDEX_MIN_GREATER                    220   // Empty image min index value is greater than max value
#define ERR_DSCAMERA_IMAGESIZE_INVALID                              221   // Image size is invalid
#define ERR_DSCAMERA_TOO_SMALL_PUNCH_RADIUS                         222   // Too small punch radius
#define ERR_DSCAMERA_REQBLOOD_IS_TOO_BIG                            223   // Required blood percent is too big
#define ERR_DSCAMERA_SACENTER_IS_INVALID                            224   // Search area center is invalid
#define ERR_DSCAMERA_SARADIUS_IS_INVALID                            225   // Search area radius is invalid
#define ERR_DSCAMERA_SETTINGS_GET_FAILED                            227   // Unexpected error occured while returning camera settings
#define ERR_DSCAMERA_SETTINGS_SET_FAILED                            228   // Unexpected error occured while updating camera settings
#define ERR_DSCAMERA_NOT_INITIALIZED                                229   // Camera isn´t initialized
#define ERR_DSCAMERA_CANT_CHANGE_IMAGESIZE                          230   // Image size cant be changed without performing init
#define ERR_DSCAMERA_CAMERADRIVEREXCEPTIONEVENTHANDLE_GET_FAILED    231   // Unexpected error occured while returning event handle
#define ERR_DSCAMERA_CAMERADRIVEREXCEPTION_GET_FAILED               232   // Unexpected error occured while returning exception object
#define ERR_DSCAMERA_FSNL_MUST_BE_GREATER_TAHN_FRI                  233   // Frame skipped limit must be greater than render interval
#define ERR_DSCAMERA_FRAMESSKIPPEDNOTIFICATIONEVENT_GET_FAILED      234   // Unexpected error occured while returning event handle
#define ERR_DSCAMERA_HOTSPOTSIZE_IS_INVALID                         235   // Hotspot size is too small
#define ERR_DSCAMERA_HSINDEX_MIN_GREATER_THAN_MAX                   236   // Min empty hotspot index value is greater than the max value
#define ERR_DSCAMERA_CALIBRATE_FAILED                               237   // Unknown error occured while calibrating camera
#define ERR_DSCAMERA_READBARCODES_GET_FAILED                        238   // Unknown error occured while returning barcode read state
#define ERR_DSCAMERA_READBARCODES_SET_FAILED                        239   // Unknown error occured while updating barcode read state
#define ERR_DSCAMERA_CARDDETECTED_SET_FAILED                        240   // Unknown error occured while returning detected status
#define ERR_DSCAMERA_TAKEPICTURE_FAILED                             241   // Unknown error occured while taking picture
#define ERR_DSCAMERA_FAILED_TO_CREATE_CALIBRATION_IMAGE             242   // Failed to create calibration image
#define ERR_DSCAMERA_CALIBRATION_IMAGE_INVALID_SIZE                 243   // Used calibration image has invalid size
#define ERR_DSCAMERA_DISPLAY_IMAGE_FAILED                           244   // Unexpected error occured while displaying image
#define ERR_DSCAMERA_DISPLAY_IMAGE_INVALID_SIZE                     245   // Used display image has invalid size
#define ERR_DSCAMERA_SET_DRAWING_SURFACE_FAILED                     246   // Unexpected error occure while updating drawing surface



// --------------------------------------------------------------------------
// PunchDetector


#define ERR_PUNCHDETECTOR_NOT_INITIALIZED                           301   // Punch detector isn't initialized
#define ERR_PUNCHDETECTOR_ALREADY_INITIALIZED                       302   // Punch detector is already initialized
#define ERR_PUNCHDETECTOR_PUNCHMAP_IS_NULL                          303   // Punch map is null
#define ERR_PUNCHDETECTOR_SETTINGS_IS_NULL                          304   // Punch detector settings is null
#define ERR_PUNCHDETECTOR_BD_IS_NULL                                305   // Blood detector argument is null
#define ERR_PUNCHDETECTOR_CANNOT_ALLOCATE_MEMORY                    306   // Could not allocate memory
#define ERR_PUNCHDETECTOR_NO_ROOM_IN_ALLOCATED_PUNCHES              307   // Cannot add a punch to allocated punches array


// --------------------------------------------------------------------------
// DSCameraRenderer

#define ERR_DSCAMERARENDERER_CHECKMEDIATYPE_FAILED                  600   // Unknown error occured while detecting media
#define ERR_DSCAMERARENDERER_DORENDERSAMPLE_FAILED                  601   // Unknown error occured while drawing image
#define ERR_DSCAMERARENDERER_SHOULDDRAWSAMPLENOW_FAILED             602   // Unknown error occured while detecting weather image should be drawn
#define ERR_DSCAMERARENDERER_ALREADY_INITIALIZED                    603   // Camera renderer already initialized
#define ERR_DSCAMERARENDERER_NOT_INITIALIZED                        604   // Camera renderer is not initialized
#define ERR_DSCAMERARENDERER_FRAMES_SKIPPED_ERROR                   605   // Frames skipped notification limit exceeded


// --------------------------------------------------------------------------
// BarcodeReader

#define ERR_BARCODEREADER_SETTINGS_IS_NULL                          700   // BarcodeReader Settings is NULL
#define ERR_BARCODEREADER_READER_LOCATION_IS_MISSING                701   // Reader location is NULL or missing
#define ERR_BARCODEREADER_FAILED_TO_LOAD_READER                     702   // Failed to load reader DLL
#define ERR_BARCODEREADER_FAILED_TO_GET_METHODS                     703   // Failed to get reader init/release methods
#define ERR_BARCODEREADER_READER_LOADING_FAILED                     704   // Unknown error occured while loading DTK reader
#define ERR_BARCODEREADER_NOT_INITIALIZED                           705   // BarcodeReader isn´t initialized
#define ERR_BARCODEREADER_BARCODE_ARG_IS_MISSING                    706   // Barcode argument is NULL or missing
#define ERR_BARCODEREADER_GETSETTINGS_FAILED                        707   // Unknown error occured while returning current settings
#define ERR_BARCODEREADER_READ_FAILED                               708   // Unknown error occured while reading barcodes
#define ERR_BARCODEREADER_CLEARBARCODES_FAILED                      709   // Unknown error occured while clearing old barcodes


// --------------------------------------------------------------------------
// BarcodeList

#define ERR_BCLIST_INVALID_INDEX                                    1100  // Invalid index
#define ERR_BCLIST_STORAGE_IS_NULL                                  1101  // Barcode storage is null
#define ERR_BCLIST_TOO_BIG_CAPACITY                                 1102  // Barcode storage capacity is too big
#define ERR_BCLIST_TOO_SMALL_CAPACITY                               1103  // Barcode storage capacity is too small
#define ERR_BCLIST_LIST_IS_FULL                                     1104  // Barcode storage is full


// --------------------------------------------------------------------------
// DSRenderManager

#define ERR_DSRENDERMANAGER_INITIALIZE_FAILED                       1200  // Unknown error occured while initializing render manager
#define ERR_DSRENDERMANAGER_RELEASE_FAILED                          1201  // Unexpected error occured while releasing render manager
#define ERR_DSRENDERMANAGER_PLAYER_RUN                              1202  // Exception occured while starting player
#define ERR_DSRENDERMANAGER_PLAYER_RUN_ERROR                        1203  // Error occured while starting player
#define ERR_DSRENDERMANAGER_PLAYER_STOP                             1204  // Exception occured while pausing player
#define ERR_DSRENDERMANAGER_PLAYER_STOP_ERROR                       1205  // Error occured while pausing player
#define ERR_DSRENDERMANAGER_NOT_INITIALIZED                         1206  // RenderManager is not initialized
#define ERR_DSRENDERMANAGER_ALREADY_INITIALIZED                     1207  // RenderManager is already initialized
#define ERR_DSRENDERMANAGER_COM_INIT_FAILED                         1208  // COM init failed
#define ERR_DSRENDERMANAGER_GRAPHBUILDER_CREATION_ERROR             1209  // Error occured while creating DS graph builder
#define ERR_DSRENDERMANAGER_GRAPHBUILDER_IS_NULL                    1210  // DS graph builder is null
#define ERR_DSRENDERMANAGER_MEDIACONTROL_CREATION_ERROR             1211  // Error occured while creating DS media control
#define ERR_DSRENDERMANAGER_MEDIACONTROL_IS_NULL                    1212  // DS media control is null
#define ERR_DSRENDERMANAGER_CANT_FIND_INPUTPIN                      1213  // Failed to find input pin
#define ERR_DSRENDERMANAGER_INPUTPIN_IS_NULL                        1214  // Input pin is null
#define ERR_DSRENDERMANAGER_FAILED_TO_ADD_SPR_TO_CHAIN              1215  // Failked to add SpotLight filter/renderer in to DS graph
#define ERR_DSRENDERMANAGER_DEVENUMERATOR_CREATION_ERROR            1216  // Error occured while creating system device enumerator
#define ERR_DSRENDERMANAGER_DEVENUMERATOR_IS_NULL                   1217  // System device enumerator is null
#define ERR_DSRENDERMANAGER_CAMENUMERATOR_CREATION_ERROR            1218  // Error occured while creating camera enumerator
#define ERR_DSRENDERMANAGER_CAMENUMERATOR_IS_NULL                   1219  // Camera enumerator is null
#define ERR_DSRENDERMANAGER_CAMERA_CREATION_ERROR                   1220  // Error occured while creating camera object
#define ERR_DSRENDERMANAGER_CAMERA_IS_NULL                          1221  // Camera object is null
#define ERR_DSRENDERMANAGER_CAMERAFILTER_IS_NULL                    1223  // Camera filter object is null
#define ERR_DSRENDERMANAGER_CANT_FIND_OUTPUT_PIN                    1224  // Cant find (camera) output pin
#define ERR_DSRENDERMANAGER_CANTOUTPUTPIN_IS_NULL                   1225  // Ccamera output pin is null
#define ERR_DSRENDERMANAGER_FAILED_TO_ADD_CAMERA_FILTER             1226  // Failed to add camera source filter into DS graph
#define ERR_DSRENDERMANAGER_FAILED_TO_CONNECT_PINS                  1227  // Failed to connect pins
#define ERR_DSRENDERMANAGER_FAILED_TO_BUILD_GRAPH                   1228  // Failed to build graph
#define ERR_DSRENDERMANAGER_DSCONNECTION_INIT_FAILED                1229  // Unknown error occured while initializing DS connection
#define ERR_DSRENDERMANAGER_CREATEBLOODDETECTOR_FAILED              1230  // Unknown error occured while creating blood detector
#define ERR_DSRENDERMANAGER_CREATEPUNCHDETECTOR_FAILED              1231  // Unknown error occured while creating punch detector
#define ERR_DSRENDERMANAGER_CREATEBARCODEREADER_FAILED              1232  // Unknown error occured while creating barcode reader
#define ERR_DSRENDERMANAGER_GETSETTINGS_FAILED                      1233  // Unknown error occured while returning current settings
#define ERR_DSRENDERMANAGER_UPDATESETTINGS_FAILED                   1234  // Unknown error occured while updating current settings
#define ERR_DSRENDERMANAGER_MEDIAEVENTS_CREATION_ERROR              1235  // Failed to create graph event listener
#define ERR_DSRENDERMANAGER_MEDIAEVENTS_IS_NULL                     1236  // Graph event listener is null
#define ERR_DSRENDERMANAGER_DEVICE_LOST                             1237  // Camera was unplugged
#define ERR_DSRENDERMANAGER_DSELISTENER_EXCEPTION                   1238  // Exception was caught while listening DS events
#define ERR_DSRENDERMANAGER_DSELISTENER_FAILED_TO_CLOSE             1239  // Failed to close DS event listener thread
#define ERR_DSRENDERMANAGER_DSELISTENER_FAILED_TO_CREATE            1240  // Failed to create DS event listener thread
#define ERR_DSRENDERMANAGER_FINDCAMERA_FAILED                       1241  // Unknown error occured while finding camera
#define ERR_DSRENDERMANAGER_CAPTUREBUILDER_CREATION_ERROR           1242  // Error occured while creating DS capture builder
#define ERR_DSRENDERMANAGER_CAPTUREBUILDER_IS_NULL                  1243  // DS capture builder is null
#define ERR_DSRENDERMANAGER_CAPTUREBUILDER_SETFILTERGRAP            1244  // Error occured whil setting filter graph



// --------------------------------------------------------------------------
// PunchPositionManager

#define ERR_PPMANAGER_PPSTORAGE_IS_EMPTY                            2000  // Punch position storage is empty
#define ERR_PPMANAGER_INVALID_PPSTORAGE_INDEX                       2001  // Invalid punch position storage index


// --------------------------------------------------------------------------

// NOTE: Dont use error codes that are higher than 20000 - they are used in CameraWrapper.dll