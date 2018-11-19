/**
 * Use image histogram similarity to sort the image
 * Author: Alex Beng
*/
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
using namespace std;
// calc_similarity(1).jpg 的hist, 就是用于计算相似度 作为标准的hist
cv::Mat standard_hist;

// 返回单通道图的 Histogram
// 为了缩减计算规模 可以降低 hist_size
cv::Mat GetHistogram(cv::Mat image, int hist_sz = 256) {
    int hist_size[1];
    float hranges[2];
    const float* ranges[1];
    int channels[1];
    cv::Mat hist;

    hist_size[0] = hist_sz;
    hranges[0] = 0.0;
    hranges[1] = 256.0;
    ranges[0] = hranges;
    channels[0] = 0;
    cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, hist_size, ranges);
    return hist;
}

// 获得与 standard_hist 的相似度分数
double GetScore(cv::Mat image) {
    return cv::compareHist(standard_hist, GetHistogram(image), CV_COMP_INTERSECT);
}

// 读入文件
void ReadData(string root_path, std::vector<cv::Mat>& images) {
    cv::Mat image;
    int count = 1;
    do {
        stringstream t_ss;
        string t_path = root_path + "calc_similarity (";
        string t_s;

        t_ss << count++;
        t_ss >> t_s;

        t_path += t_s;
        t_path += ").jpg";

        image = cv::imread(t_path);
        
        
        if (image.empty()) {
            return;
        }
        else {

            if (count == 2) {
                cv::resize(image, image, cv::Size(255, 255));
                cv::imshow("standard image", image);
                cv::cvtColor(image, image, CV_BGR2HSV);
                standard_hist =  GetHistogram(image);
            }
            else {
                cv::cvtColor(image, image, CV_BGR2HSV);
                images.push_back(image);
            }
        }
    }while (!image.empty());
}

// sort 的自定义函数
bool for_compare(cv::Mat img1, cv::Mat img2) {
    return GetScore(img1) > GetScore(img2);
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return -1;
    }
    string root_path(argv[1]);
    std::vector<cv::Mat> images;
    ReadData(root_path, images);
    std::sort(images.begin(), images.end(), for_compare);

    for (size_t i=0; i<images.size(); i++) {
        stringstream t_ss;
        string t_s;
        t_ss << i+1;
        t_ss >> t_s;
        cv::cvtColor(images[i], images[i], CV_HSV2BGR);
        cv::resize(images[i], images[i], cv::Size(255, 255));
        cv::imshow(t_s, images[i]);
    }
    cv::waitKey(0);
    return 0;
}
