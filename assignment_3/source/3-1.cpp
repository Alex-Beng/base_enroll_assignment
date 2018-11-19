/**
 * Compute 1/3 channels image's histogram and show it
 * Author: Alex Beng
*/
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
using namespace std;
std::vector<cv::Mat> result;

// 获得单通道的可视化直方图
cv::Mat ChannelHistogram(cv::Mat image) {
    int hist_size[1];
    float hranges[2];
    const float* ranges[1];
    int channels[1];
    cv::Mat hist;

    hist_size[0] = 256;
    hranges[0] = 0.0;
    hranges[1] = 256.0;
    ranges[0] = hranges;
    channels[0] = 0;
    cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, hist_size, ranges);

    double t_max, t_min;
    cv::minMaxLoc(hist, &t_min, &t_max);
    int row = hist.rows;
    int col = hist.rows;
    int summit = cvCeil(0.9*hist.rows);

    cv::Mat hist_image(row, col, CV_8UC1, cv::Scalar::all(0));
    float* data = hist.ptr<float>(0);
    
    for (int i=0; i<256; i++) {
        int t_height = cvCeil(summit/t_max*(*(data+i)));
        cv::line(hist_image, cv::Point(i, row-t_height), cv::Point(i, 0),
                    cv::Scalar(255));
    }
    return hist_image;
}

// 获得 1/3 通道的直方图 
void GetHistogram(cv::Mat image, std::vector<cv::Mat> &result) {
    result.clear();
    if (image.channels() == 1) {
        result.push_back(ChannelHistogram(image).clone());
    }
    else if (image.channels() == 3) {

        cv::Mat t_channels[3];
        cv::split(image, t_channels);
        for (int i=0; i<3; i++) {
            result.push_back(ChannelHistogram(t_channels[i]).clone());
        }
    }
}


int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return -1;
    }

    cv::Mat image = cv::imread(argv[1], 1);
    if (image.empty()) {
        return -1;
    }

    GetHistogram(image, result);
    
    for (size_t i=0; i<result.size(); i++) {
        stringstream t_ss;
        string t_s;
        t_ss << i;
        t_ss >> t_s;
        cv::imshow(t_s, result[i]);
    }
    cv::waitKey(0);
    return 0;
}
