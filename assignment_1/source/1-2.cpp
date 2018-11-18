/**
 * At least 5 ways to reduce color
 * Author: Alex Beng
*/

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
using namespace std;

void ReduceColor(cv::Mat src_img, int flag = 0, int div = 64) {
    int row = src_img.rows;
    int col = src_img.cols;
    uchar div_half = div >> 1;

    switch (flag) {
    case 0: // 使用最友好的 at & 非位运算的方法
        for (int i=0; i<row; i++) {
            for (int j=0; j<col; j++) {
                if (src_img.channels() == 1) {
                    src_img.at<uchar>(i, j) = src_img.at<uchar>(i, j)/div*div + div_half;
                }
                else if (src_img.channels() == 3) {
                    for (int t=0; t<3; t++) {
                        src_img.at<cv::Vec3b>(i, j)[t] = src_img.at<cv::Vec3b>(j, i)[t]/div*div + div_half;  
                    }
                }
                else if (src_img.channels() == 4) {// 最后一个通道为 alpha 通道, 与颜色无关
                    for (int t=0; t<3; t++) {
                        src_img.at<cv::Vec4b>(i, j)[t] = src_img.at<cv::Vec4b>(j, i)[t]/div*div + div_half;  
                    }
                }
            }
        }
    case 1: // 使用 Mat_ 迭代 + 取模方法
        // uchar div_half = div >> 1;
        if (src_img.channels() == 1) {
            cv::Mat_<uchar>::iterator begin = src_img.begin<uchar>();
            cv::Mat_<uchar>::iterator end = src_img.end<uchar>();
            while (begin != end) {
                uchar t = *begin;
                *begin = t - t%div + div_half;
                begin++;
            }
        }
        else if (src_img.channels() == 3) {
            cv::Mat_<cv::Vec3b>::iterator begin = src_img.begin<cv::Vec3b>();
            cv::Mat_<cv::Vec3b>::iterator end = src_img.end<cv::Vec3b>();
            while (begin != end) {
                for (int k=0; k<3; k++) {
                    uchar t = (*begin)[k];
                    (*begin)[k] = t - t%div + div_half;
                }
                begin++;
            }
        }
    case 2: // 使用指针 + 显示查找表
        cv::Mat table(1, 256, CV_8U);
        
        uchar* data = table.ptr<uchar>(0);
        for (int i=0; i<256; i++) {
            data[i] = i/div*div + div/2;
        }

        cv::LUT(src_img, table, src_img);

    }
}

// 突然发现把多通道图也干了...2333
// 那就只写三种方法吧
int main(int argc, char const* argv[]) {
    if (argc != 2) {
        return -1;
    }

    cv::Mat image = cv::imread(argv[1], 0);
    if (image.empty()) {
        return -1;
    }
    ReduceColor(image, 2);
    cv::imwrite("./test.jpg", image);
    cv::imshow("2333", image);
    cv::waitKey(0);
    return 0;
}