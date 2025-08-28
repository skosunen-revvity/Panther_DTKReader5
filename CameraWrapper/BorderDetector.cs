// --------------------------------------------------------------------------
// BorderDetector
// --------------------------------------------------------------------------
//
// TODO: add desc
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C#.NET 2008
//
// $Header: $
// Created: 02.06.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

using System;
using System.Drawing;
using System.Collections.Generic;

namespace PKI.Puncher.CameraWrapper {

  public class BorderDetector {

    #region Type

    private enum Border : byte {
      Top,
      Left,
      Bottom,
      Right
    }

    #endregion


    #region Constructors

    protected BorderDetector() {
    }

    #endregion


    #region Public methods

    /// <summary>
    /// TODO: add desc
    /// </summary>
    /// <param name="settings"></param>
    /// <returns></returns>
    public static BorderDetectionResult Detect(BorderDetectorSettings settings) {

      if (settings == null) {
        throw new ArgumentNullException();
      }

      if (settings.Image == null || settings.HorizontalDetectionAccuracy < 1 || settings.VerticalDetectionAccuracy < 1) {
        throw new ArgumentException();
      }

      
      BorderDetectionResult bdr = new BorderDetectionResult();
      
      bdr.TopBorder = DetectBorder(Border.Top, settings);
      if (bdr.TopBorder.Count > 0) {

        bdr.LeftBorder = DetectBorder(Border.Left, settings);
        if (bdr.LeftBorder.Count > 0) {

          bdr.BottomBorder = DetectBorder(Border.Bottom, settings);
          if (bdr.BottomBorder.Count > 0) {

            bdr.RightBorder = DetectBorder(Border.Right, settings);
            if (bdr.RightBorder.Count > 0) {

              bdr.Top = FindPoint(Border.Top, settings.UseMaxDetectionPointTop, bdr.TopBorder);
              bdr.Left = FindPoint(Border.Left, settings.UseMaxDetectionPointLeft, bdr.LeftBorder);

              bdr.Bottom = FindPoint(Border.Bottom, settings.UseMaxDetectionPointBottom, bdr.BottomBorder);
              bdr.Right = FindPoint(Border.Right, settings.UseMaxDetectionPointRight, bdr.RightBorder);

            }
          }
        }
      }

      return bdr;
    }

    #endregion


    #region Private methods

    /// <summary>
    /// Detect border...TODO: add desc
    /// </summary>
    /// <param name="border"></param>
    /// <param name="settings"></param>
    /// <returns></returns>
    private static List<Point> DetectBorder(Border border, BorderDetectorSettings settings) {

      int xStart;
      int xEnd;
      int yStart;
      int yEnd;
      int step;

      int y;
      int x;
      Color pixel;

      List<Point> borderPoints = new List<Point>();

      int imageWidth = settings.Image.Width;
      int imageHeight = settings.Image.Height;
      

      if (border == Border.Left || border == Border.Right) {

        yEnd = imageHeight - settings.BottomDetectionOffset;
        step = (yEnd - settings.TopDetectionOffset) / settings.VerticalDetectionAccuracy;
        yStart = settings.TopDetectionOffset + (step / 2);

        if (border == Border.Left) {

          
          xStart = settings.LeftDetectionOffset;
          xEnd = imageWidth - settings.RightDetectionOffset;

          for (y = yStart; y < yEnd; y += step) {
            for (x = xStart; x < xEnd; x++) {
              pixel = settings.Image.GetPixel(x, y);
              if (IsColorInRange(settings.BorderColorMin, settings.BorderColorMax, pixel)) {
                borderPoints.Add(new Point(x, y));
                break;
              }
            }
          }


        } else {

          
          xStart = imageWidth - settings.RightDetectionOffset - 1;
          xEnd = settings.LeftDetectionOffset;

          for (y = yStart; y < yEnd; y += step) {
            for (x = xStart; x >= xEnd; x--) {
              pixel = settings.Image.GetPixel(x, y);
              if (IsColorInRange(settings.BorderColorMin, settings.BorderColorMax, pixel)) {
                borderPoints.Add(new Point(x, y));
                break;
              }
            }
          }


        }

      } else {

        
        xEnd = imageWidth - settings.RightDetectionOffset;
        step = (xEnd - settings.LeftDetectionOffset) / settings.HorizontalDetectionAccuracy;
        xStart = settings.LeftDetectionOffset + (step / 2);

        if (border == Border.Top) {


          yStart = settings.TopDetectionOffset;
          yEnd = imageHeight - settings.BottomDetectionOffset;

          for (x = xStart; x < xEnd; x += step) {
            for (y = yStart; y < yEnd; y++) {
              pixel = settings.Image.GetPixel(x, y);
              if (IsColorInRange(settings.BorderColorMin, settings.BorderColorMax, pixel)) {
                borderPoints.Add(new Point(x, y));
                break;
              }
            }
          }
          

        } else {


          yStart = imageHeight - settings.BottomDetectionOffset - 1;
          yEnd = settings.TopDetectionOffset;

          for (x = xStart; x < xEnd; x += step) {
            for (y = yStart; y >= yEnd; y--) {
              pixel = settings.Image.GetPixel(x, y);
              if (IsColorInRange(settings.BorderColorMin, settings.BorderColorMax, pixel)) {
                borderPoints.Add(new Point(x, y));
                break;
              }
            }
          }


        }
      }

      return borderPoints;

    }

    /// <summary>
    /// Return true if color, value, is between min and max colors
    /// </summary>
    /// <param name="min">Min color</param>
    /// <param name="max">Max color</param>
    /// <param name="value">Color to test</param>
    private static bool IsColorInRange(Color min, Color max, Color value) {

      if (value.R < min.R || value.R > max.R) {
        return false;
      }

      if (value.G < min.G || value.G > max.G) {
        return false;
      }

      if (value.B < min.B || value.B > max.B) {
        return false;
      }

      return true;

    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    /// <param name="border"></param>
    /// <param name="findMax"></param>
    /// <param name="points"></param>
    /// <returns></returns>
    private static Point FindPoint(Border border, bool findMax, List<Point> points) {

      int i;
      Point t;

      Point pt = points[0];

      switch (border) {

        case Border.Top:

          for (i = 1; i < points.Count; i++) {
            t = points[i];
            if (findMax) {
              if (t.Y > pt.Y) {
                pt = t;
              }
            } else {
              if (t.Y < pt.Y) {
                pt = t;
              }
            }
          }

          break;


        case Border.Left:

          for (i = 1; i < points.Count; i++) {
            t = points[i];
            if (findMax) {
              if (t.X > pt.X) {
                pt = t;
              }
            } else {
              if (t.X < pt.X) {
                pt = t;
              }
            }
          }

          break;


        case Border.Bottom:

          for (i = 1; i < points.Count; i++) {
            t = points[i];
            if (findMax) {
              if (t.Y < pt.Y) {
                pt = t;
              }
            } else {
              if (t.Y > pt.Y) {
                pt = t;
              }
            }
          }

          break;


        case Border.Right:

          for (i = 1; i < points.Count; i++) {
            t = points[i];
            if (findMax) {
              if (t.X < pt.X) {
                pt = t;
              }
            } else {
              if (t.X > pt.X) {
                pt = t;
              }
            }
          }

          break;

      }

      return pt;
    }

    #endregion
    
  }

}