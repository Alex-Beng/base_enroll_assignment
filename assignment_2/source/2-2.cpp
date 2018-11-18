/**
 * Use Hue channel to thershold the image.
 * Author: Alex Beng
*/
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
cv::Rect ROI; // grabcut 是对 ROI 进行提取前景的
bool drawing_ROI, selected;
cv::Mat image_for_draw;

// 添加 alpha 通道
cv::Mat AddAlpha(cv::Mat image, cv::Mat alpha) {
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    channels.push_back(alpha);
    cv::merge(channels, image);
    return image;
}

// 用于获得ROI的鼠标回调事件
void MouseHandler(int event, int x, int y, int flags, void *) {
    switch(event) {
    case CV_EVENT_LBUTTONDOWN:
        drawing_ROI = true;
        ROI = cv::Rect(x, y, 0, 0);
        break;
    case CV_EVENT_MOUSEMOVE:
        if (drawing_ROI) {
            ROI.width = x - ROI.x;
            ROI.height = y - ROI.y;
            cv::Mat t_with_rect = image_for_draw.clone();
            cv::rectangle(t_with_rect, ROI, cv::Scalar(0, 0, 255), 2);
            cv::imshow("2333", t_with_rect);
        }
        break;
    case CV_EVENT_LBUTTONUP:
        drawing_ROI = false;
        selected = true;
        if (ROI.width < 0) {
            ROI.x += ROI.width;
            ROI.width *= -1;
        }
        if (ROI.height < 0) {
            ROI.y += ROI.height;
            ROI.height *= -1;
        }
    }
}

// // 返回前景
cv::Mat ThresholdWithGrabcut(cv::Mat image, cv::Rect roi) {
    cv::Mat background, foreground;
    cv::Mat result;
    cv::grabCut(image, result, roi, background, foreground, 5, cv::GC_INIT_WITH_RECT);
    cv::compare(result, cv::GC_PR_FGD, result, cv::CMP_EQ);
    return result;  
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return -1;
    }

    cv::Mat image = cv::imread(argv[1]);
    cv::resize(image, image, cv::Size(320, 240));
    image_for_draw = image.clone();
    if (image.empty()) {
        return -1;
    }
    cv::namedWindow("2333");
    cv::imshow("2333", image);
    cv::setMouseCallback("2333", MouseHandler);
    
    while (1) {
        if (cv::waitKey() == 'p') {
            cv::imshow("result", ThresholdWithGrabcut(image, ROI));
            cv::imwrite("result.png", AddAlpha(image, ThresholdWithGrabcut(image, ROI)));
            char t_char =  cv::waitKey(0);
            if (t_char == 'q') {
                cv::destroyAllWindows();
                return 0;
            }
        }
    }
    return 0;
}