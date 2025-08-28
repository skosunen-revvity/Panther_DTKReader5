// --------------------------------------------------------------------------
// Log
// --------------------------------------------------------------------------
//
// Logger component that uses PKI.Wallac.Logging.Logger for actual logging
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

using PKI.Wallac.Logging;

namespace PKI.Puncher.CameraWrapper {

    internal class LogMessages {

        internal enum Information {

            /// <summary>
            /// Camera component initialized
            /// </summary>
            CameraInitialized = 0,

            /// <summary>
            /// Camera is run in simulation mode
            /// 
            /// Params:
            /// #! - Simulation file
            /// </summary>
            CameraIsRunInSimulationMode,

            /// <summary>
            /// Camera has been released
            /// </summary>
            CameraReleased,


        }
        internal enum Error {

            /// <summary>
            /// Exception was caught while initializing camera
            /// 
            /// Params:
            /// #1 - Error message
            /// #2 - Stack trace
            /// </summary>
            ExcCameraInitialized = 0,

            /// <summary>
            /// Exception was caught while releasing camera
            /// 
            /// Params:
            /// #1 - Error message
            /// #2 - Stack trace
            ExcReleasingCamera,

            /// <summary>
            /// Passed argument is null
            /// 
            /// Params:
            /// #1 - Argument name
            /// #2 - Method name
            /// </summary>
            ErrNullArgument,

            /// <summary>
            /// Error occured whil closing (DS camera) event listener
            /// 
            /// Params:
            /// #1 - Error message
            /// #2 - Stack trace
            /// </summary>
            ErrCloseDSEventListener,

            /// <summary>
            /// Error occure in the (DS camera) notification listener thread
            /// 
            /// Params:
            /// #1 - Error message
            /// #2 - Stack trace
            /// </summary>
            ErrDSNotificationListener

        }
        internal enum Debug {

            /// <summary>
            /// Initializing camera
            /// </summary>
            InitializingCamera = 0,

            /// <summary>
            /// Releasing camera
            /// </summary>
            ReleasingCamera,

            /// <summary>
            /// Initializing DS camera
            /// 
            /// Params:
            /// #1 - Driver implementation file
            /// </summary>
            InitializeDSCamera



        }

    }

    internal class Log {

        #region Constructors

        protected Log() {
        }
        static Log() {
        }

        #endregion


        #region Public methods

        /// <summary>
        /// TODO: Add desc
        /// </summary>
        /// <param name="sourceName"></param>
        public static void Initialize(string sourceName) {

            if (m_Logger == null) {
                m_Logger = Logger.GetLogger(sourceName);
            }

        }

        /// <summary>
        /// TODO Add desc
        /// </summary>
        public static void Release() {

            // TODO: Cand we close logger here
            Logger.Shutdown();

            m_Logger = null;

        }

        /// <summary>
        /// Add information message
        /// </summary>
        /// <param name="methodName">Name of the method who is writing this message</param>
        /// <param name="messageID">Message ID</param>
        /// <param name="messageArguments">Optional message arguments</param>
        public static void Information(string methodName, LogMessages.Information messageID, params object[] messageArguments) {

            if (m_Logger != null) {
                m_Logger.Info(methodName, StringBag.GetInformationLogMessage(messageID), messageArguments);
            }

        }

        /// <summary>
        /// Add error message
        /// </summary>
        /// <param name="methodName">Name of the method who is writing this message</param>
        /// <param name="exc">Optional exception information</param>
        /// <param name="messageID">Message ID</param>
        /// <param name="messageArguments"Optional message arguments></param>
        public static void Error(string methodName, Exception exc, LogMessages.Error messageID, params object[] messageArguments) {

            if (m_Logger != null) {
                m_Logger.Error(methodName, exc, StringBag.GetErrorLogMessage(messageID), messageArguments);
            }

        }

        /// <summary>
        /// Add debug message
        /// </summary>
        /// <param name="methodName">Name of the method who is writing this message</param>
        /// <param name="messageID">Message ID</param>
        /// <param name="messageArguments">Optional message arguments</param>
        public static void Debug(string methodName, LogMessages.Debug messageID, params object[] messageArguments) {

            if (m_Logger != null) {
                m_Logger.Debug(methodName, StringBag.GetDebugLogMessage(messageID), messageArguments);
            }

        }

        #endregion



        #region Private data


        /// <summary>
        /// Actual logging provider
        /// </summary>
        private static Logger m_Logger = null;

        #endregion

    }

}