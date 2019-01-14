#include "XVideoThread.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "XFilter.h"
using namespace cv;
using namespace std;
#pragma comment(lib, "opencv_world320d.lib")

// 一号视频源
static VideoCapture cap1;
static VideoCapture cap2;
static bool isExit = false;

// 打开一号视频源文件
bool XVideoThread::Open(const std::string file)
{
	Seek(0);
	mutex.lock();
	bool re = cap1.open(file);
	mutex.unlock();
	
	if (!re)
	{
		cout << "VideoCapture open failed!" << endl;
		return false;
	}
	cout << "VideoCapture open success!" << endl;
	fps = cap1.get(CAP_PROP_FPS);
	width = cap1.get(CAP_PROP_FRAME_WIDTH);
	height = cap1.get(CAP_PROP_FRAME_HEIGHT);
	if (fps <= 0) fps = 25;
	return true;
}

// 打开二号视频源文件
bool XVideoThread::Open2(const std::string file)
{
	Seek(0);
	mutex.lock();
	bool re = cap2.open(file);
	mutex.unlock();
	if (!re)
	{
		cout << "VideoCapture open2 failed!" << endl;
		return false;
	}
	cout << "VideoCapture open2 success!" << endl;
	/*fps = cap2.get(CAP_PROP_FPS);
	width = cap2.get(CAP_PROP_FRAME_WIDTH);
	height = cap2.get(CAP_PROP_FRAME_HEIGHT);
	if (fps <= 0) fps = 25;*/
	return true;
}

// 返回当前播放的位置
double XVideoThread::GetPos()
{
	double pos = 0;
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return pos;
	}
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	double cur = cap1.get(CAP_PROP_POS_FRAMES);
	if(count > 0.001) pos = cur / count;
	mutex.unlock();
	return pos;
}

//保存视频
static VideoWriter vw;

//开始保存视频
bool XVideoThread::StartSave(const std::string filename, int width, int height, bool isColor)
{
	cout << "开始导出" << endl;
	Seek(0);
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	if (width <= 0)
	{
		width = cap1.get(CAP_PROP_FRAME_WIDTH);
	}
	if (height <= 0)
	{
		height = cap1.get(CAP_PROP_FRAME_HEIGHT);
	}
	vw.open(filename,
		VideoWriter::fourcc('X','2','6','4'),
		this->fps,
		Size(width, height),
		isColor
		);
	if (!vw.isOpened())
	{
		cout << "start save failed!" << endl;
		mutex.unlock();
		return false;
	}

	this->isWrite = true;
	mutex.unlock();
	return true;
}

//停止保存视频, 写入视频帧的索引
void XVideoThread::StopSave()
{
	cout << "停止导出" << endl;
	mutex.lock();
	vw.release();
	isWrite = false;
	mutex.unlock();
}

// 跳转视频
///@para frame int 帧位置
bool XVideoThread::Seek(int frame)
{
	mutex.lock();
	if (!cap1.isOpened())
	{
		mutex.unlock();
		return false;
	}
	int re = cap1.set(CAP_PROP_POS_FRAMES, frame);
	if(cap2.isOpened())
		cap2.set(CAP_PROP_POS_FRAMES, frame);
	mutex.unlock();
	return re;
}

bool XVideoThread::Seek(double pos)
{
	double count = cap1.get(CAP_PROP_FRAME_COUNT);
	int frame = pos * count;
	return Seek(frame);
}

XVideoThread::XVideoThread()
{
	start();
}


XVideoThread::~XVideoThread()
{
	mutex.lock();
	isExit = true;
	mutex.unlock();
	wait();
}

void XVideoThread::run()
{
	Mat mat1;
	for (;;)
	{
		mutex.lock();
		if (isExit)
		{
			mutex.unlock();
			break;
		}
		// 判断视频是否打开
		if (!cap1.isOpened())
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
		if (!isPlay)
		{
			mutex.unlock();
			msleep(5);
			continue;
		}
		// 读取一帧视频,解码并颜色转换
		if (!cap1.read(mat1) || mat1.empty())
		{
			mutex.unlock();
			//导出到结尾位置, 停止导出
			if (isWrite)
			{
				StopSave();
				SaveEnd();
			}
			msleep(5);
			continue;
		}

		Mat mat2 = mark;
		//通过过滤器处理视频
		if (cap2.isOpened())
		{
			cap2.read(mat2);
		}
		// 显示图像1
		if (!isWrite)
		{
			ViewImage1(mat1);
			if(!mat2.empty())
				ViewImage2(mat2);
		}

		Mat des = XFilter::Get()->Filter(mat1, mat2);

		// 显示生成后图像
		if (!isWrite)
			ViewDes(des);

		int s = 0;
		s = 1000 / fps;
		if (isWrite)
		{
			s = 1;
			vw.write(des);
		}

		msleep(s);
		mutex.unlock();
	}
}
