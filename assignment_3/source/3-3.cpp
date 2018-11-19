/**
 * Use slide window + histogram similarity to track ROI
*/
#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>
using namespace std;

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        return -1;
    }
    cv::Mat image = cv::imread(argv[1], 1);
    if (image.empty()) {
        return -1;
    }
    int xo=97, yo=112;
	int width=25, height=1;
	cv::Mat roi(image,cv::Rect(xo,yo,width,height));

	// compute sum
	// returns a Scalar to work with multi-channel images
	cv::Scalar sum= cv::sum(roi);
    cout<<sum<<endl;
    cout<<image.at<cv::Vec3b>(112, 97)<<endl;
	std::cout << sum[0] << std::endl;
     
    // compute integral image
	cv::Mat integralImage;
	cv::integral(image,integralImage,CV_32F);
	// get sum over an area using three additions/subtractions
    int sumInt= integralImage.at<int>(yo+height,xo+width)
			      -integralImage.at<int>(yo+height,xo)
			      -integralImage.at<int>(yo,xo+width)
			      +integralImage.at<int>(yo,xo);
	std::cout << sumInt << std::endl;
    return 0;
}
