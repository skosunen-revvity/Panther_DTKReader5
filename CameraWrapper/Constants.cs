// --------------------------------------------------------------------------
// Constants
// --------------------------------------------------------------------------
//
// Various constant definitions
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

namespace PKI.Puncher.CameraWrapper {

    internal class Constants {

        /// <summary>
        /// Messages which are use in exception descriptions
        /// </summary>
        internal class ExceptionMessages {

            public const string ArgumentNullException = "Argument \"{0}\" is null.";

        }

        /// <summary>
        /// Log messages
        /// </summary>
        internal class LogMessages {

            internal class Information {

                public const string CameraInitialized = "Camera initialized";
                public const string CameraIsRunInSimulationMode = "Camera is run in simulation mode. Used simulation file is: \"{0}\"";
                public const string CameraReleased = "Camera has been released";

            }
            internal class Error {

                public const string ExcCameraInitialized = "Exception was caught while initializing camera. Error description: {0} CS: {1}";
                public const string ExcReleasingCamera = "Exception was caught while releasing camera. Error description: {0} CS: {1}";
                public const string ErrCloseDSEventListener = "Exception was caught while releasing (DS camera) event listener. Error description: {0} CS: {1}";
                public const string ErrDSNotificationListener = "Exception was caught while listening (DS camera) notification. Error description: {0} CS: {1}";

                public const string ErrNullArgument = "Argument \"{0}\" that was passed to method {1}, is null";

            }
            internal class Debug {

                public const string InitializingCamera = "Initializing camera";
                public const string ReleasingCamera = "Releasing camera";
                public const string InitializeDSCamera = "Initializing DS camera from \"{0}\"";

            }


        }

        /// <summary>
        /// Error codes that are used when throwing PuncherCameraException
        /// </summary>
        internal class ErrorCodes {

            // NOTE: Dont use codes that are lower than 20000 - they are used in DSCameraDriver

            #region Camera

            public const int ERR_CAMERA_DSDRIVER_IMPL_NOT_FOUND = 20000;

            #endregion


            #region ImageComparer

            /// <summary>
            /// ImageComparer settings object is null
            /// </summary>
            public const int ERR_IMAGECOMPARER_SETTINGS_IS_NULL = 20100;

            /// <summary>
            /// Source image 1 is missing
            /// </summary>
            public const int ERR_IMAGECOMPARER_IMAGE_1_IS_NULL = 20101;

            /// <summary>
            /// Source image 2 is missing
            /// </summary>
            public const int ERR_IMAGECOMPARER_IMAGE_2_IS_NULL = 20102;

            /// <summary>
            /// Source image 1 size is invalid
            /// </summary>
            public const int ERR_IMAGECOMPARER_IMAGE_1_SIZE_IS_INVALID = 20103;

            /// <summary>
            /// Source image 2 size is invalid
            /// </summary>
            public const int ERR_IMAGECOMPARER_IMAGE_2_SIZE_IS_INVALID = 20104;

            /// <summary>
            /// Image 1 and 2 have different size
            /// </summary>
            public const int ERR_IMAGECOMPARER_SOURCEIMAGE_SIZE_IS_DIFFERENT = 20105;

            /// <summary>
            /// Search area position is invalid
            /// </summary>
            public const int ERR_IMAGECOMPARER_SEARCHAREA_POSITION_IS_INVALID = 20106;

            #endregion



        }

    }

}