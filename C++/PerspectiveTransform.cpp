#include "PerspectiveTransform.h"
#include "FieldPositionData.h"

PerspectiveTransform::PerspectiveTransform(int OutputWidth, int OutputHeight)
{
    persImgHorizontal = cv::Mat::zeros(OutputHeight, OutputWidth, CV_8UC3);
    persImgVertical = cv::Mat::zeros(OutputWidth, OutputHeight, CV_8UC3);

    dstHorizontal[0].x = 0;                 dstVertical[0].x = 0;
    dstHorizontal[0].y = 0;                 dstVertical[0].y = 0;
    dstHorizontal[1].x = OutputWidth-1;     dstVertical[1].x = OutputHeight-1;
    dstHorizontal[1].y = 0;                 dstVertical[1].y = 0;
    dstHorizontal[2].x = 0;                 dstVertical[2].x = 0;
    dstHorizontal[2].y = OutputHeight-1;    dstVertical[2].y = OutputWidth-1;
    dstHorizontal[3].x = OutputWidth-1;     dstVertical[3].x = OutputHeight-1;
    dstHorizontal[3].y = OutputHeight-1;    dstVertical[3].y = OutputWidth-1;
}

PerspectiveTransform::~PerspectiveTransform()
{
    persImgHorizontal.release();
    persImgVertical.release();
}

cv::Point2f *PerspectiveTransform::ChooseDstPoint(int arrayIndex)
{
    switch(fieldpositionPoint[arrayIndex])
    {
        case CutArea::DOWN:
            return dstHorizontal;
        break;
        case CutArea::LEFT:
            return dstVertical;
        break;
        case CutArea::RIGHT:
            return dstVertical;
        break;
        case CutArea::UP:
            return dstHorizontal;
        break;
    }
};


cv::Mat& PerspectiveTransform::ChoosePersImg(int arrayIndex)
{
    switch(fieldpositionPoint[arrayIndex])
    {
        case CutArea::DOWN:
            return persImgHorizontal;
        break;
        case CutArea::LEFT:
            return persImgVertical;
        break;
        case CutArea::RIGHT:
            return persImgVertical;
        break;
        case CutArea::UP:
            return persImgHorizontal;
        break;
    }
};

// MasterDuelHelper's special Run
cv::Mat PerspectiveTransform::Run(cv::Point2f before[], cv::Mat &originalImg, int arrayIndex)
{
	cv::Mat Trans = cv::getPerspectiveTransform(before, ChooseDstPoint(arrayIndex));
	cv::Mat persImg = ChoosePersImg(arrayIndex);
	cv::warpPerspective(originalImg, persImg, Trans, cv::Size(persImg.cols, persImg.rows), cv::INTER_LINEAR);
    RotateImg(persImg,arrayIndex);
    return persImg;
}

// Normal Run
void PerspectiveTransform::Run(cv::Point2f before[], cv::Point2f after[], cv::Mat &originalImg, cv::Mat &persImg)
{
	cv::Mat Trans = cv::getPerspectiveTransform(before, after);
	cv::warpPerspective(originalImg, persImg, Trans, cv::Size(persImg.cols, persImg.rows), cv::INTER_LINEAR);
}

// 翻轉模式，flipCode == 0垂直翻轉（沿X軸翻轉）
// flipCode>0水平翻轉（沿Y軸翻轉）
// flipCode<0水平垂直翻轉（先沿X軸翻轉，再沿Y軸翻轉，等價於旋轉180°）

void PerspectiveTransform::MatRotateClockWise90(cv::Mat &src)
{
	// 矩陣轉置
	transpose(src, src);
	//0: 沿X軸翻轉； >0: 沿Y軸翻轉； <0: 沿X軸和Y軸翻轉
	flip(src, src, 1);
};

void PerspectiveTransform::MatRotateClockWise180(cv::Mat &src)//順時針180
{
	//0: 沿X軸翻轉； >0: 沿Y軸翻轉； <0: 沿X軸和Y軸翻轉
	cv::flip(src, src, 0);
	cv::flip(src, src, 1);
};

void PerspectiveTransform::MatRotateClockWise270(cv::Mat &src)//順時針270
{
	transpose(src, src);
    //0: 沿X軸翻轉； >0: 沿Y軸翻轉； <0: 沿X軸和Y軸翻轉
	flip(src, src, 0);
};


cv::Mat& PerspectiveTransform::RotateImg(cv::Mat& NeedToRotateImg, int arrayIndex)
{
    switch(fieldpositionPoint[arrayIndex])
    {
        case CutArea::DOWN:
        break;
        case CutArea::LEFT:
            MatRotateClockWise270(NeedToRotateImg);
        break;
        case CutArea::RIGHT:
            MatRotateClockWise90(NeedToRotateImg);
        break;
        case CutArea::UP:
            MatRotateClockWise180(NeedToRotateImg);
        break;
    }

    return NeedToRotateImg;
};