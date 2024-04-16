#ifndef __PERSPECTIVETRANSFORM_H__
#define __PERSPECTIVETRANSFORM_H__

//使用OpenCV C語言
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/highgui/highgui_c.h> 

//使用OpenCV C++語言
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

class PerspectiveTransform
{
public:
	PerspectiveTransform(int OutputWidth, int OutputHeight);
    ~PerspectiveTransform();
	cv::Mat Run(cv::Point2f before[], cv::Mat &originalImg, int arrayIndex);
	void Run(cv::Point2f before[], cv::Point2f after[], cv::Mat &originalImg, cv::Mat &persImg);

private:
	cv::Point2f* ChooseDstPoint(int arrayIndex);
	cv::Mat& ChoosePersImg(int arrayIndex);
	void MatRotateClockWise90(cv::Mat &src);
	void MatRotateClockWise180(cv::Mat &src);
	void MatRotateClockWise270(cv::Mat &src);
	cv::Mat& RotateImg(cv::Mat& NeedToRotateImg, int arrayIndex);
    cv::Mat persImgHorizontal;
    cv::Mat persImgVertical;
    cv::Point2f dstHorizontal[4];
    cv::Point2f dstVertical[4];

protected:
};

#endif // __PERSPECTIVETRANSFORM_H__

