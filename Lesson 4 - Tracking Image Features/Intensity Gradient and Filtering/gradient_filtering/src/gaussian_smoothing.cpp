#include <iostream>
#include <numeric>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

void gaussianSmoothing1()
{
    // load image from file
    cv::Mat img;
    img = cv::imread("../images/img1gray.png");

    // create filter kernel
    float gauss_data[25] = {1, 4, 7, 4, 1,
                            4, 16, 26, 16, 4,
                            7, 26, 41, 26, 7,
                            4, 16, 26, 16, 4,
                            1, 4, 7, 4, 1};

    cv::Mat kernel = cv::Mat(5, 5, CV_32F, gauss_data);
    
    int arr_size = sizeof(gauss_data)/sizeof(gauss_data[0]);
    std::cout<< "array size is " << arr_size << std::endl;
    
    int normalizer = 0;
    for (int i=0; i<arr_size; i++)
    {
        normalizer += gauss_data[i];
    }
    
    std::cout<< "normalizer is " << normalizer << std::endl;
    
    for (int i=0; i<arr_size; i++)
    {
        gauss_data[i] /= normalizer;
    }

    // apply filter
    cv::Mat result;
    cv::filter2D(img, result, -1, kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

    // show result
    string windowName = "Gaussian Blurring";
    cv::namedWindow(windowName, 1); // create window
    cv::imshow(windowName, result);
    cv::waitKey(0); // wait for keyboard input before continuing
}

int main()
{
    gaussianSmoothing1();
}
