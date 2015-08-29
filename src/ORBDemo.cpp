/*
 * ORBDemo.cpp
 *
 *  Created on: Aug 29, 2015
 *      Author: yanzhang
 */


#include <vector>
#include <iostream>
#include <stdlib.h>
#include "opencv2/features2d.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
//#include "opencv2/xfeature2d.hpp"

using namespace cv;

int main(int argc, char** argv){

    // read images
    Mat img_1;
    Mat img_2;
    Mat img_11;
    Mat img_22;
    img_1 = imread(argv[1],CV_LOAD_IMAGE_GRAYSCALE);
    img_2 = imread(argv[2],CV_LOAD_IMAGE_GRAYSCALE);

    // resize the images
//    Size size(512,512);
//    resize(img_1,img_1,size);
//    resize(img_2,img_2,size);

    // rotate the images
//    Point2f src_center(img_1.cols/2.0f, img_1.rows/2.0f);
//    Mat rotation_matrix = getRotationMatrix2D(src_center, 90, 1.0);
//    warpAffine(img_1,img_1,rotation_matrix,img_1.size());
//    warpAffine(img_2,img_2,rotation_matrix,img_1.size());


    // now, you can no more create an instance on the 'stack', like in the tutorial
    // (yea, noticed for a fix/pr).
    // you will have to use cv::Ptr all the way down:

    cv::Ptr<Feature2D> f2d = ORB::create();
    // you get the picture, i hope..

    //-- Step 1: Detect the keypoints:
    std::vector<KeyPoint> keypoints_1, keypoints_2;
    f2d->detect( img_1, keypoints_1 );
    f2d->detect( img_2, keypoints_2 );

    //-- Step 2: Calculate descriptors (feature vectors)
    Mat descriptors_1, descriptors_2;
    f2d->compute( img_1, keypoints_1, descriptors_1 );
    f2d->compute( img_2, keypoints_2, descriptors_2 );

    //-- Step 3: Matching descriptor vectors using BFMatcher :
    BFMatcher matcher(NORM_HAMMING,true);
    Mat match_visual;

    std::vector< DMatch > matches;
    matcher.match( descriptors_1, descriptors_2, matches );
    drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, match_visual,Scalar::all(-1), Scalar::all(-1),
            std::vector<char>(), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
    namedWindow("matching results", WINDOW_AUTOSIZE);
    imshow("matching results", match_visual);
    waitKey(0);
    return 0;
}
