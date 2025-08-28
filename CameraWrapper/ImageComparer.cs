// --------------------------------------------------------------------------
// ImageComparer
// --------------------------------------------------------------------------
//
// TODO: add desc
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C#.NET 2008
//
// $Header: $
// Created: 21.06.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

using System;
using System.Drawing;

namespace PKI.Puncher.CameraWrapper {

  public class ImageComparer {

    #region Constructors

    public ImageComparer() {
    }

    #endregion


    #region Public methods

    public ImageComparerResults Compare(ImageComparerSettings settings) {

      ImageComparerResults result = new ImageComparerResults();
      ValidateSettings(settings);

      int differenceMapHeight;
      int differenceMapWidth;

      bool[] differenceMap;

      // Build difference map - true means that pixel is different
      if (!BuildDifferenceMap(settings, out differenceMapWidth, out differenceMapHeight, out differenceMap)) {

        Point center;
        int radius;

        // Find the largest circle inside the changed area
        if (FindLargestCircle(differenceMapWidth, differenceMapHeight, differenceMap, out center, out radius)) {
          
          result.Center = new Point(
            (settings.SearchAreaCenter.X - settings.SearchAreaRadius) + center.X,
            (settings.SearchAreaCenter.Y - settings.SearchAreaRadius) + center.Y);

          result.Radius = radius;

          if (settings.GenerateResultImage) {

            Point tl;
            Point br;

            Bitmap resultImage = new Bitmap(differenceMapWidth, differenceMapHeight);

            bool cropped;
            CreateBounds(settings.SearchAreaCenter, settings.SearchAreaRadius, Point.Empty,
              new Point(settings.Image1.Width, settings.Image1.Height), out cropped, out tl, out br);

            Color col;

            int _y = 0;
            int _x = 0;

            for (int y = tl.Y; y < br.Y; y++) {
              for (int x = tl.X; x < br.X; x++) {
                if (ContainsPoint(settings.SearchAreaCenter.X, settings.SearchAreaCenter.Y, settings.SearchAreaRadius, x, y)) {

                  if (settings.DrawInvalidPixels) {
                    col = settings.InvalidPixelColor;
                    if (!differenceMap[(_y * differenceMapWidth) + _x]) {
                      col = settings.Image2.GetPixel(x, y);
                    }
                  } else {
                    col = settings.Image2.GetPixel(x, y);
                  }

                  resultImage.SetPixel(_x, _y, col);

                }

                _x++;
              }

              _x = 0;
              _y++;
            }

            if (settings.DrawCenter) {
              DrawCross(center, 5, settings.CenterColor, ref resultImage);
            }

            result.ResultImage = resultImage;
          }
        }
      }

      return result;

    }

    #endregion


    #region Private data

    private const string CLASSNAME = "ImageComparer";
    private const string SEP = ".";

    #endregion


    #region Private methods

    private void ValidateSettings(ImageComparerSettings settings) {

      const string METHODNAME = "ValidateSettings";
      if (settings == null) {
        // Settings are missing
        throw new ImageComparerException(CLASSNAME, METHODNAME, Constants.ErrorCodes.ERR_IMAGECOMPARER_SETTINGS_IS_NULL);
      }

      if (settings.Image1 == null) {
        // Image 1 is missing
        throw new ImageComparerException(CLASSNAME, METHODNAME, Constants.ErrorCodes.ERR_IMAGECOMPARER_IMAGE_1_IS_NULL);
      }

      if (settings.Image2 == null) {
        // Image 2 is missing
        throw new ImageComparerException(CLASSNAME, METHODNAME, Constants.ErrorCodes.ERR_IMAGECOMPARER_IMAGE_2_IS_NULL);
      }

      if (settings.Image1.Width == 0 || settings.Image1.Height == 0) {
        // Image 1 size is invalid
        throw new ImageComparerException(CLASSNAME, METHODNAME, Constants.ErrorCodes.ERR_IMAGECOMPARER_IMAGE_1_SIZE_IS_INVALID);
      }

      if (settings.Image2.Width == 0 || settings.Image2.Height == 0) {
        // Image 2 size is invalid
        throw new ImageComparerException(CLASSNAME, METHODNAME, Constants.ErrorCodes.ERR_IMAGECOMPARER_IMAGE_2_SIZE_IS_INVALID);
      }

      if (settings.Image1.Width != settings.Image2.Width ||
        settings.Image1.Height != settings.Image2.Height) {
        // Image 1 and 2 have different size
        throw new ImageComparerException(CLASSNAME, METHODNAME, Constants.ErrorCodes.ERR_IMAGECOMPARER_SOURCEIMAGE_SIZE_IS_DIFFERENT);
      }

      if (settings.SearchAreaCenter.X < 0 || settings.SearchAreaCenter.X >= settings.Image1.Width ||
        settings.SearchAreaCenter.Y < 0 || settings.SearchAreaCenter.Y >= settings.Image1.Height) {
        // Search area center is invalid
        throw new ImageComparerException(CLASSNAME, METHODNAME,
          Constants.ErrorCodes.ERR_IMAGECOMPARER_SEARCHAREA_POSITION_IS_INVALID);       
      }

    }
    private bool ContainsPoint(int centerX, int centerY, int radius, int x, int y) {

      int dX = x - centerX;
      int dY = y - centerY;

      int sumOfSquares = dX * dX + dY * dY;

      return radius * radius >= sumOfSquares;

    }
    private void DrawCross(Point center, int len, Color col, ref Bitmap source) {

      int xStart = center.X - len;
      int xEnd = center.X + len + 1;

      int yStart = center.Y - len;
      int yEnd = center.Y + len + 1;

      if (xStart < 0) {
        xStart = 0;
      }

      if (xEnd > source.Width) {
        xEnd = source.Width;
      }

      if (yStart < 0) {
        yStart = 0;
      }

      if (yEnd > source.Height) {
        yEnd = source.Height;
      }

      int t;
      for (t = xStart; t < xEnd; t++) {
        source.SetPixel(t, center.Y, col);
      }

      for (t = yStart; t < yEnd; t++) {
        source.SetPixel(center.X, t, col);
      }

    }

    private bool BuildDifferenceMap(ImageComparerSettings settings, out int diffMapWidth, out int diffMapHeight, out bool[] differenceMap) {

      Point tl;
      Point br;

      bool cropped;
      CreateBounds(settings.SearchAreaCenter, settings.SearchAreaRadius, Point.Empty,
        new Point(settings.Image1.Width, settings.Image1.Height), out cropped, out tl, out br);

      diffMapHeight = br.Y - tl.Y;
      diffMapWidth = br.X - tl.X;
      differenceMap = new bool[diffMapHeight * diffMapWidth];

      bool identical = true;

      Color tmp;
      int value1;
      int value2;

      bool mapValue;

      for (int x = tl.X; x < br.X; x++) {
        for (int y = tl.Y; y < br.Y; y++) {

          mapValue = false;
          if (ContainsPoint(settings.SearchAreaCenter.X, settings.SearchAreaCenter.Y, settings.SearchAreaRadius, x, y)) {

            tmp = settings.Image1.GetPixel(x, y);
            value1 = tmp.R + tmp.G + tmp.B;

            tmp = settings.Image2.GetPixel(x, y);
            value2 = tmp.R + tmp.G + tmp.B;

            if (value2 < (value1 - settings.DetectionLimitMin) || value2 > (value1 + settings.DetectionLimitMax)) {
              mapValue = true;
              identical = false;
            }
          }

          differenceMap[((y - tl.Y) * diffMapWidth) + (x - tl.X)] = mapValue;
        }
      }

      return identical;

    }
    private bool FindLargestCircle(int diffMapWidth, int diffMapHeight, bool[] differenceMap, out Point center, out int radius) {

      center = new Point(ImageComparerResults.UNKNOWN_X, ImageComparerResults.UNKNOWN_Y);
      radius = 0;

      int maxRadius = (diffMapWidth / 2) + 1;
      Point ptTmp = Point.Empty;

      for (int y = 0; y < diffMapHeight; y++) {
        for (int x = 0; x < diffMapWidth; x++) {
          for (int r = 1; r < maxRadius; r++) {
            
            ptTmp.X = x;
            ptTmp.Y = y;

            if (IsAll(ptTmp, r, true, diffMapWidth, diffMapHeight, differenceMap)) {
              if (r > radius) {
                center.X = x;
                center.Y = y;
                radius = r;
              }
            } else {
              break;
            }

          }
        }
      }

      return (center.X != ImageComparerResults.UNKNOWN_X);

    }

    private bool IsAll(Point center, int radius, bool value, int mapWidth, int mapHeight, bool[] map) {

      Point tl;
      Point br;

      bool oneFound = false;

      bool cropped;
      CreateBounds(center, radius, new Point(0, 0), new Point(mapWidth, mapHeight), out cropped, out tl, out br);
      if (!cropped) {
        for (int y = tl.Y; y < br.Y; y++) {
          for (int x = tl.X; x < br.X; x++) {
            if (ContainsPoint(center.X, center.Y, radius, x, y)) {

              if (map[(y * mapWidth) + x] != value) {
                return false;
              }

              oneFound = true;

            }
          }
        }
      }

      return oneFound;

    }

    private void CreateBounds(Point center, int radius, Point limitTopLeft, Point limitBottomRight, out bool cropped, out Point topLeft, out Point bottomRight) {

      topLeft = new Point(center.X - radius, center.Y - radius);
      bottomRight = new Point(center.X + radius, center.Y + radius);
      cropped = false;

      if (topLeft.X < limitTopLeft.X) {
        topLeft.X = limitTopLeft.X;
        cropped = true;
      }

      if (topLeft.Y < limitTopLeft.Y) {
        topLeft.Y = limitTopLeft.Y;
        cropped = true;
      }

      if (bottomRight.X > limitBottomRight.X) {
        bottomRight.X = limitBottomRight.X;
        cropped = true;
      }

      if (bottomRight.Y > limitBottomRight.Y) {
        bottomRight.Y = limitBottomRight.Y;
        cropped = true;
      }

    }

    #endregion

  }

}