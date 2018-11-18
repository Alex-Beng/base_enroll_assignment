/**
 * Use floodfill to get foreground
*/
#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
cv::Point seed;
bool clicked;

// 添加 alpha 通道
cv::Mat AddAlpha(cv::Mat image, cv::Mat alpha) {
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    channels.push_back(alpha);
    cv::merge(channels, image);
    return image;
}

// 调用 floodfill 鼠标回调函数
void MouseHandler(int event, int x, int y, int flags, void *) {
    if (event == CV_EVENT_LBUTTONDOWN) {
        seed = cv::Point(x, y);
        
        clicked = true;
    }
    else if (event == CV_EVENT_RBUTTONDOWN) {
        clicked = false;
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return -1;
    }
    cv::Mat image = cv::imread(argv[1]);
    cv::Mat image_for_flood = image.clone();            
    if (image.empty()) {
        return -1;
    }

    cv::namedWindow("2333");
    cv::setMouseCallback("2333", MouseHandler);

    
    while (1) {
        cv::imshow("2333", image);
        if (cv::waitKey(50) == 'q') {
            break;
        }
        if (clicked) {
            clicked = false;
            int flood_flag = 8 | (255<<8); // 临近四联通 & 填充mask为255
            cv::Mat mask(image.rows+2, image.cols+2, CV_8UC1, cv::Scalar(0));
            image_for_flood = image.clone();
            
            cv::floodFill(image_for_flood, mask, seed,
                      cv::Scalar(0,0,255), 0,
                      cv::Scalar(10,10,10), cv::Scalar(10,10,10),
                      flood_flag);
            cv::Mat foreground;            
            image.copyTo(foreground, mask(cv::Rect(1, 1, image.cols, image.rows)));
            cv::imshow("2333", image_for_flood);
            cv::imshow("result", foreground);
            cv::imwrite("result.png", AddAlpha(image, mask(cv::Rect(1, 1, image.cols, image.rows))));
            cv::waitKey();
        }
    }
    return 0;
}