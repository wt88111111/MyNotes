#pragma once
#include <opencv2/core.hpp>
class XImagePro
{
public:
	// 设置原图, 会清理处理结果
	void Set(cv::Mat mat1, cv::Mat mat2);

	// 获取处理后的结果
	cv::Mat Get() { return des; }

	// 设置亮度和对比度
	///@para bright   double  亮度   0~100
	///@para contrast double  对比度 1.0~3.0
	void Gain(double bright, double contrast);

	void Rotate90();
	void Rotate180();
	void Rotate270();

	void FlipX();
	void FlipY();
	void FilpXY();

	XImagePro();
	~XImagePro();
private:
	// 原图
	cv::Mat src1, src2;

	// 目标图
	cv::Mat des;
};

