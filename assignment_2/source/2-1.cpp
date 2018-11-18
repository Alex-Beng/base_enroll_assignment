/**
 * Use Hue channel to thershold the image.
 * Author: Alex Beng
*/
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

// 添加 alpha 通道
cv::Mat AddAlpha(cv::Mat image, cv::Mat alpha) {
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    channels.push_back(alpha);
    cv::merge(channels, image);
    return image;
}

// Hue 通道是一个圆周, 所以需要给定方向
cv::Mat ThersholdHue(cv::Mat image, int t1, int t2, int direction=0) {
    cv::Mat hsv_image;
    cv::Mat channels[3];
    cv::cvtColor(image, hsv_image, CV_BGR2HSV);
    cv::split(hsv_image, channels);

    cv::Mat result_1, result_2;
    
    if (direction == 0) { // t1 < pix < t2
        result_1 = channels[0] > t1;
        result_2 = channels[0] < t2;
        return result_1 & result_2;
    }
    else { // another direction threshold
        result_1 = channels[0] < t1;
        result_2 = channels[0] > t2;
        return result_1 | result_2;
    }
}

// 默认使用 L1 距离
cv::Mat ThersholdWithDistance(cv::Mat image, cv::Vec3b color, double distance, int flag=1) {
    int row = image.rows;
    int col = image.cols;
    cv::Mat result(row, col, CV_8UC1);

    if (flag == 1){
        for (int i=0; i<row; i++) {
            for (int j=0; j<col; j++) {
                if ( cv::norm(image.at<cv::Vec3b>(i, j), color, cv::NORM_L1) < distance ) {
                    result.at<uchar>(i, j) = 255;
                }
                else {
                    result.at<uchar>(i, j) = 0;
                }
                    
            }
        }
    }
    else if (flag == 2) {
        for (int i=0; i<row; i++) {
            for (int j=0; j<col; j++) {
                if ( cv::norm(image.at<cv::Vec3b>(i, j), color, cv::NORM_L2) < distance ) {
                    result.at<uchar>(i, j) = 255;
                }
                else {
                    result.at<uchar>(i, j) = 0;
                }
                    
            }
        }
    }
    return result;
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return -1;
    }

    cv::Mat image = cv::imread(argv[1]);
    if (image.empty()) {
        return -1;
    }

    cv::imwrite("proc_1.png", AddAlpha(image, ThersholdHue(image, 100, 200, 0)));
    cv::imwrite("proc_2.png", AddAlpha(image, ThersholdHue(image, 100, 200, 1)));
    
    cv::imwrite("proc_3.png", AddAlpha(image, ThersholdWithDistance(image, cv::Vec3b(255,255,255), 100, 1)));
    cv::imwrite("proc_4.png", AddAlpha(image, ThersholdWithDistance(image, cv::Vec3b(255,255,255), 100, 2)));

    return 0;
}
