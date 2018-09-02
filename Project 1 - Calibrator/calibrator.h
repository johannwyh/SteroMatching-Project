//
//  calibrator.h
//  Stereo Matching
//
//  Created by 王宇晗 on 2018/8/31.
//  Copyright © 2018年 王宇晗. All rights reserved.
//

#ifndef calibrator_h
#define calibrator_h

#include <cstring>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

using namespace std;
using namespace cv;

class CameraCalibrator
{
private:
    vector< vector<Point3f> > objectPoints;
    vector< vector<Point2f> > imagePoints;
    Mat cameraMatrix;
    Mat distCoeffs;
    int flag;
    Mat mp1, mp2;
    bool isUndistorted = false;
    
    void addPointList(const vector<Point2f>& imageCorners, const vector<Point3f>& objectCorners)
    {
        imagePoints.push_back(imageCorners);
        objectPoints.push_back(objectCorners);
    }
public:
    CameraCalibrator(): flag(0), isUndistorted(false) {}
    
    int addChessBoardPoints(const vector<string> &filelist, Size boardSize)
    {
        vector<Point2f> imageCorners;
        vector<Point3f> objectCorners;
        
        for(int i = 0; i < boardSize.height; i++)
        {
            for(int j = 0; j < boardSize.width; j++)
            {
                objectCorners.push_back(Point3f(i, j, 0));
            }
        }
        
        Mat image;
        int acceptedFile = 0;
        for(int i = 0; i < filelist.size(); i++)
        {
            image = imread(filelist[i], 0);
            bool found = findChessboardCorners(image, boardSize, imageCorners);
            TermCriteria crit = TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 40, 0.001);
            cornerSubPix(image, imageCorners, Size(5, 5), Size(-1, -1), crit);
            if (imageCorners.size() == boardSize.area())
            {
                addPointList(imageCorners, objectCorners);
                acceptedFile++;
            }
            drawChessboardCorners(image, boardSize, imageCorners, found);
            imshow("Corners on Chessboard", image);
            waitKey(200);
        }
        return acceptedFile;
    }
    
    double calibrate(Size imageSize)
    {
        isUndistorted = false;
        vector<Mat> rvecs, tvecs;
        double ret = calibrateCamera(objectPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs, flag);
        return ret;
    }
    
    Mat rebuild(const Mat& image)
    {
        Mat undistorted;
        if (!isUndistorted)
        {
            initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(), Mat(), image.size(), CV_32FC1, mp1, mp2);
            isUndistorted = true;
        }
        remap(image, undistorted, mp1, mp2, INTER_LINEAR);
        return undistorted;
    }
    
    Mat getCameraMatrix() { return cameraMatrix; }
    Mat getDistCoeffs() { return distCoeffs; }
};

void calibrateAndUndistort(CameraCalibrator &cc, string FILE_PATH, string basicName, int pictureNum, Size boardSize);

#endif /* calibrator_h */
