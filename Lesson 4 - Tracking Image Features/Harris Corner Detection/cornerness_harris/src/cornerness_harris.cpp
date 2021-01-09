#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

using namespace std;

void cornernessHarris()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1.png");
    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY); // convert to grayscale

    // Detector parameters
    int blockSize = 2;     // for every pixel, a blockSize × blockSize neighborhood is considered
    int apertureSize = 3;  // aperture parameter for Sobel operator (must be odd)
    int minResponse = 100; // minimum value for a corner in the 8bit scaled response matrix
    double k = 0.04;       // Harris parameter (see equation for details)

    // Detect Harris corners and normalize output
    cv::Mat dst, dst_norm, dst_norm_scaled;
    dst = cv::Mat::zeros(img.size(), CV_32FC1);
    cv::cornerHarris(img, dst, blockSize, apertureSize, k, cv::BORDER_DEFAULT);
    cv::normalize(dst, dst_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());
    cv::convertScaleAbs(dst_norm, dst_norm_scaled);

    // visualize results
    string windowName = "Harris Corner Detector Response Matrix";
    cv::namedWindow(windowName, 4);
    cv::imshow(windowName, dst_norm_scaled);
    cv::waitKey(0);

    // TODO: Your task is to locate local maxima in the Harris response matrix 
    // and perform a non-maximum suppression (NMS) in a local neighborhood around 
    // each maximum. The resulting coordinates shall be stored in a list of keypoints 
    // of the type `vector<cv::KeyPoint>`.
    
    //https://cppsecrets.com/users/168511510411111711412197115105110104975764103109971051084699111109/C00-OpenCV-cvnormalize.php
    //https://cppsecrets.com/users/168511510411111711412197115105110104975764103109971051084699111109/C00-OpenCV-cvconvertScaleAbs.php
    
    vector<cv::KeyPoint> keypoints;
    
    //https://docs.opencv.org/3.4/d4/d7d/tutorial_harris_detector.html
    for( size_t j = 0; j < dst_norm.rows ; j++ )
    {
        for( size_t i = 0; i < dst_norm.cols; i++ )
        {            
            //take the keypoint if they are more than a threshold - minResponse
            int response = (int)dst_norm.at<float>(j, i);
            if( response > minResponse )
            {
                cv::KeyPoint tmpKeyPoint;
                
                //https://docs.opencv.org/3.4/d2/d29/classcv_1_1KeyPoint.html
                tmpKeyPoint.pt = cv::Point2f(i, j);
                tmpKeyPoint.size = 2 * apertureSize;
                tmpKeyPoint.response = response;
                
                if(keypoints.empty())
                {
                    keypoints.push_back(tmpKeyPoint);
                }
                else
                {
                    // Non-Maximum Suppression (NMS)
                    for(auto it = keypoints.begin(); it != keypoints.end(); ++it)
                    {
                        double overlap = cv::KeyPoint::overlap(tmpKeyPoint, *it);
                        
                        if(overlap > 0.0) // 0 means no overlap and 1 means overlap
                        {
                            //there is a overlap. so lets check which response is large
                            if(tmpKeyPoint.response > (*it).response)
                            {
                                *it = tmpKeyPoint; //replace
                                break;
                            }
                        }
                        else //0, there is no overlap
                        {
                            keypoints.push_back(tmpKeyPoint);
                        }
                    }
                }
            }
        }
    }
    
    // visualize keypoints
    windowName = "Harris Corner Detection Results";
    cv::namedWindow(windowName, 5);
    cv::Mat visImage = dst_norm_scaled.clone();
    cv::drawKeypoints(dst_norm_scaled, keypoints, visImage, cv::Scalar::all(-1), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    cv::imshow(windowName, visImage);
    cv::waitKey(0);
    
    

}

int main()
{
    cornernessHarris();
} 
