//
//  calibrator.cpp
//  Stereo Matching
//
//  Created by 王宇晗 on 2018/9/2.
//  Copyright © 2018年 王宇晗. All rights reserved.
//

#include "calibrator.h"
#include <iostream>
#include <vector>

using namespace std;

void calibrateAndUndistort(CameraCalibrator &cc, string FILE_PATH, string basicName, int pictureNum, Size boardSize)
{
    puts("FUCK");
    Mat image;
    Size imageSize;
    vector<string> filelist;
    for(int i = 0; i <= pictureNum; i++)
    {
        string name = FILE_PATH + basicName;
        if (i < 10) name += "0" + to_string(i);
        else name += to_string(i);
        name += ".jpg";
        image = imread(name);
        if (image.empty()) continue;
        imageSize = image.size();
        filelist.push_back(name);
        imshow("Image", image);
        waitKey(100);
    }
    cc.addChessBoardPoints(filelist, boardSize);
    cc.calibrate(imageSize);
    for(int i = 0; i <= pictureNum; i++)
    {
        string name = FILE_PATH + basicName;
        if (i < 10) name += "0" + to_string(i);
        else name += to_string(i);
        name += ".jpg";
        image = imread(name);
        if (image.empty()) continue;
        imshow("Original Image", image);
        imshow("Undistorted Image", cc.rebuild(image));
        waitKey(500);
    }
    cout << "==CameraMatrix==" << endl;
    cout << cc.getCameraMatrix() << endl;
    cout << "==DistortionCoefficents==" << endl;
    cout << cc.getDistCoeffs() << endl;
    waitKey(3000);
    return;
}
