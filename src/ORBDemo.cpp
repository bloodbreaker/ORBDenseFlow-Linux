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
#include "opencv2/xfeatures2d.hpp"

using namespace cv;

int main(int argc, char** argv){

    int i,j;
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

    cv::Ptr<xfeatures2d::BriefDescriptorExtractor> f2d = xfeatures2d::BriefDescriptorExtractor::create();
    // you get the picture, i hope..

    //-- Step 1: Detect the keypoints:

    // --- using detector
//    std::vector<KeyPoint> keypoints_1, keypoints_2;
//    f2d->detect( img_1, keypoints_1 );
//    f2d->detect( img_2, keypoints_2 );

    // --- using all the points
    std::vector<Point2f> points_1;
    std::vector<KeyPoint> keypoints_1;
    for (i = 15; i < img_1.cols-15; i++)
        for (j = 15; j < img_1.rows-15; j++)
            points_1.push_back(img_1.at<Point2f>(j,i));

    // --- convert vector<point> to keypoint vector
    KeyPoint::convert(points_1, keypoints_1);




    //-- Step 2: Calculate descriptors (feature vectors)
    Mat descriptors_1;
    f2d->compute( img_1, keypoints_1, descriptors_1 );

    // -- Step 3: create a multi-channel Mat to store the distribution field
    int nc;
    typedef Vec<uchar, 32> vec32d;
    std::vector<Mat> spectrum;
    Mat distribution_field(img_1.rows, img_1.cols, CV_8UC(32),Scalar::all(0));
    for (i = 15; i < img_1.cols-15; i++)
        for (j = 15; j < img_1.rows-15; j++)
            for (nc = 0; nc <32; nc++)
                distribution_field.at<vec32d>(j,i)=(descriptors_1.row(i*img_1.rows+j));

    split(distribution_field, spectrum);


    namedWindow("matching results", WINDOW_AUTOSIZE);
    imshow("matching results", spectrum[0]);
    waitKey(0);
    return 0;
}
