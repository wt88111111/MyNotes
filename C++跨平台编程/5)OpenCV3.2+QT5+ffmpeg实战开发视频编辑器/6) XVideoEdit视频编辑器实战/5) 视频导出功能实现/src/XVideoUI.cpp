#include "XVideoUI.h"
#include <QFileDialog>
#include <string>
#include <iostream>
#include <QMessageBox>
#include "XVideoThread.h"
#include "XFilter.h"
using namespace std;
static bool pressSlider = false;

XVideoUI::XVideoUI(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	// 隐藏窗口顶部标题栏
	setWindowFlags(Qt::FramelessWindowHint);
	// Qt原本是不支持cv::Mat作为信号和槽的传递参数, 所以这里需要注册一下才能使用
	qRegisterMetaType<cv::Mat>("cv::Mat");

	//原视频显示信号
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewImage1(cv::Mat)),
		ui.src1,
		SLOT(SetImage(cv::Mat))
	);

	//输出视频显示信号
	QObject::connect(XVideoThread::Get(),
		SIGNAL(ViewDes(cv::Mat)),
		ui.des,
		SLOT(SetImage(cv::Mat))
	);

	//导出视频结束
	QObject::connect(XVideoThread::Get(),
		SIGNAL(SaveEnd()),
		this,
		SLOT(ExportEnd())
	);

	startTimer(40);
}

void XVideoUI::Open()
{
	QString name = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("请选择视频文件"));
	if (name.isEmpty()) return;
	string file = name.toLocal8Bit().data();
	//QMessageBox::information(0, "", name);
	//cout << file << endl;
	if (!XVideoThread::Get()->Open(file))
	{
		cout << "open file failed!" << endl;
		return;
	}
}

void XVideoUI::timerEvent(QTimerEvent *e)
{
	if (pressSlider) return;
	double pos = XVideoThread::Get()->GetPos();
	ui.playSlider->setValue(pos*1000);
}

void XVideoUI::SliderPress()
{
	pressSlider = true;
}

void XVideoUI::SliderRelease()
{
	pressSlider = false;
}

// 滑动条拖动
void XVideoUI::SetPos(int pos)
{
	XVideoThread::Get()->Seek((double)pos / 1000.);
}

void XVideoUI::Set()
{
	XFilter::Get()->Clear();
	// 对比度和亮度
	if (ui.bright->value() > 0 || 
		ui.contrast->value() >1)
	{
		XFilter::Get()->Add(XTask{XTASK_GAIN,
		{(double)ui.bright->value(), ui.contrast->value()}
		});
	}
}

static bool isExport = false;

// 导出视频
void XVideoUI::Export()
{
	if (isExport)
	{
		//停止导出
		XVideoThread::Get()->StopSave();
		isExport = false;
		ui.exportButton->setText("Start Export");
		return;
	}
	//开始导出
	QString name = QFileDialog::getSaveFileName(this,
		"save", "out1.avi");
	if (name.isEmpty()) return;
	std::string filename = name.toLocal8Bit().data();
	if(XVideoThread::Get()->StartSave(filename))
	{ 
		isExport = true;
		ui.exportButton->setText("Stop Export");
	}
}

// 导出视频结束
void XVideoUI::ExportEnd()
{
	isExport = false;
	ui.exportButton->setText("Start Export");
}