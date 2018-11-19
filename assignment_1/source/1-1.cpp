/**
 * OpenCV's hello world code
 * Author: Alex Beng
*/
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

int main(int argc, char const *argv[]) {
    cv::Mat image(200, 750, CV_8UC1, cv::Scalar(0));
    cv::putText(image, "Hello World !", cv::Point(75,150), 0, 3, cv::Scalar(255), 2);
    cv::imshow("hello", image);
    cv::waitKey(0);
    return 0;
}
