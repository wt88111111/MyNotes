#include "XImagePro.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
using namespace cv;
// 设置原图, 会清理处理结果
void XImagePro::Set(cv::Mat mat1, cv::Mat mat2)
{
	if (mat1.empty()) return;
	this->src1 = mat1;
	this->src2 = mat2;
	this->src1.copyTo(des);
}

// 设置亮度和对比度
///@para bright   double  亮度   0~100
///@para contrast double  对比度 1.0~3.0
void XImagePro::Gain(double bright, double contrast)
{
	if (des.empty()) return;
	des.convertTo(des, -1, contrast, bright);
}

void XImagePro::Rotate90()
{
	if (des.empty()) return;
	rotate(des, des, ROTATE_90_CLOCKWISE);
}
void XImagePro::Rotate180()
{
	if (des.empty()) return;
	rotate(des, des, ROTATE_180);
}
void XImagePro::Rotate270()
{
	if (des.empty()) return;
	rotate(des, des, ROTATE_90_COUNTERCLOCKWISE);
}

// 左右镜像
void XImagePro::FlipX()
{
	if (des.empty()) return;
	flip(des, des, 0);
}

// 上下镜像
void XImagePro::FlipY()
{
	if (des.empty()) return;
	flip(des, des, 1);
}

void XImagePro::FilpXY()
{
	if (des.empty()) return;
	flip(des, des, -1);
}

// 视频画面裁剪
void XImagePro::Clip(int x, int y, int w, int h)
{
	if (des.empty()) return;
	if (x < 0 || y < 0 || w <= 0 || h <= 0) return;
	if (x > des.cols || y > des.rows) return;
	des = des(Rect(x, y, w, h));
}

//图像金字塔
void XImagePro::PyDown(int count)
{
	if (des.empty()) return;
	for (int i = 0; i < count; i++)
	{
		pyrDown(des, des);
	}
}
void XImagePro::PyUp(int count)
{
	if (des.empty()) return;
	for (int i = 0; i < count; i++)
	{
		pyrUp(des, des);
	}
}

// 图像尺寸
void XImagePro::Resize(int width, int height)
{
	if (des.empty()) return;
	resize(des, des, Size(width, height));
}

XImagePro::XImagePro()
{
}


XImagePro::~XImagePro()
{
}
