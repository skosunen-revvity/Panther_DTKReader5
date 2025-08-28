// --------------------------------------------------------------------------
// StringBag
// --------------------------------------------------------------------------
//
// Helper object to manage various strings
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

    internal enum ExceptionMessages {

        /// <summary>
        /// Required argument is null
        /// 
        /// Params:
        /// 1# - argument name
        /// </summary>
        ArgumentNullException = 0

    }

    internal class StringBag {

        #region Constructors

        protected StringBag() {
        }
        static StringBag() {
            Load();
        }

        #endregion


        #region Public methods

        public static void Load() {
        }

        public static string GetExceptionDescription(ExceptionMessages messageID, params object[] messageArguments) {

            string msg = "";
            switch (messageID) {

                case ExceptionMessages.ArgumentNullException:
                    msg = Constants.ExceptionMessages.ArgumentNullException;
                    break;


            }

            return string.Format(msg, messageArguments);

        }
        public static string GetInformationLogMessage(LogMessages.Information messageID) {

            switch (messageID) {

                case LogMessages.Information.CameraInitialized:
                    return Constants.LogMessages.Information.CameraInitialized;

                case LogMessages.Information.CameraIsRunInSimulationMode:
                    return Constants.LogMessages.Information.CameraIsRunInSimulationMode;

                case LogMessages.Information.CameraReleased:
                    return Constants.LogMessages.Information.CameraReleased;

                default:
                    return string.Format("UNKNOWN INFORMATION MESSAGE ID: {0}", (int)messageID);
            }

        }
        public static string GetErrorLogMessage(LogMessages.Error messageID) {

            switch (messageID) {

                case LogMessages.Error.ExcCameraInitialized:
                    return Constants.LogMessages.Error.ExcCameraInitialized;

                case LogMessages.Error.ExcReleasingCamera:
                    return Constants.LogMessages.Error.ExcReleasingCamera;

                case LogMessages.Error.ErrNullArgument:
                    return Constants.LogMessages.Error.ErrNullArgument;

                case LogMessages.Error.ErrCloseDSEventListener:
                    return Constants.LogMessages.Error.ErrCloseDSEventListener;

                case LogMessages.Error.ErrDSNotificationListener:
                    return Constants.LogMessages.Error.ErrDSNotificationListener;


                default:
                    return string.Format("UNKNOWN ERROR MESSAGE ID: {0}", (int)messageID);
            }

        }
        public static string GetDebugLogMessage(LogMessages.Debug messageID) {

            switch (messageID) {

                case LogMessages.Debug.InitializingCamera:
                    return Constants.LogMessages.Debug.InitializingCamera;

                case LogMessages.Debug.ReleasingCamera:
                    return Constants.LogMessages.Debug.ReleasingCamera;

                case LogMessages.Debug.InitializeDSCamera:
                    return Constants.LogMessages.Debug.InitializeDSCamera;


                default:
                    return string.Format("UNKNOWN DEBUG MESSAGE ID: {0}", (int)messageID);

            }

        }

        #endregion

    }

}