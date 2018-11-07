#include <iostream>
#include <vector>
#include<opencv2/opencv.hpp>
#include<chrono>
#include <math.h>
#define WINDOW_NAME1 "三视图"
#define WINDOW_NAME2 "主视图"
#define WINDOW_NAME3 "俯视图"
using namespace cv;
using namespace std;
#define PI 3.14159
//进行一次修改
//实验1

int main()
{
	Mat srcImage = imread("Sanshitu20181029.bmp");
	cout << "Loading the pictures!" << endl;
	if (!srcImage.data) 
	{ 
		cout << "初始化加载图片失败!" << endl;
		return 1;
	}
	namedWindow(WINDOW_NAME1);
	namedWindow(WINDOW_NAME2);
	namedWindow(WINDOW_NAME3);
	Mat grayImage;
	cvtColor(srcImage, grayImage, CV_BGR2GRAY);
	threshold(grayImage, grayImage, 250, 255, THRESH_BINARY);
	double qualityLevel = 0.01;
	double minDistance = 1;
	int blockSize = 3;
	double k = 0.04;
	int r = 4;
	cout << "Dealing with the pictures……" << endl;
	//对圆柱体和长方体的数据进行定义
	int rectX, rectY, rectZ;
	int circleX, circleR;

	//对主视图进行处理
	Mat imageROI1;
	imageROI1 = grayImage(Rect(0, 0, grayImage.cols / 2 + 50, grayImage.rows / 2 + 50));
	//主视图强角点检测
	vector<Point2f> corners1;
	goodFeaturesToTrack(imageROI1, corners1, 20, qualityLevel, minDistance,
		Mat(), blockSize, false, k);
	int lengthMin = grayImage.cols / 2, lengthMax = 0;
	int heightMin = grayImage.rows / 2, heightMax = 0;
	int lengthMid; int tempheight;
	for (unsigned int i = 0; i < corners1.size(); i++)
	{
		circle(imageROI1, corners1[i], r, Scalar(255, 0, 255), -1, 8, 0);
		//获取横纵坐标的最大最小值
		if (corners1[i].x > lengthMax)
		{
			lengthMax = corners1[i].x;
			tempheight = corners1[i].y;
		}
		if (corners1[i].x < lengthMin) lengthMin = corners1[i].x;
		if (corners1[i].y > heightMax) heightMax = corners1[i].y;
		if (corners1[i].y < heightMin) heightMin = corners1[i].y;
	}
	for (unsigned int i = 0; i < corners1.size(); i++)
	{
		if (corners1[i].x<lengthMax - 20 && corners1[i].x>lengthMin + 20) lengthMid = corners1[i].x;
	}
	//得到部分坐标
	rectX = lengthMid - lengthMin;
	rectZ = heightMax - heightMin;
	circleX = lengthMax - lengthMid;
	circleR = abs((heightMax + heightMin) / 2 - tempheight);
	//输出结果
	cout << "In the front view:" << endl;
	cout << "The size of lengthMax:   " << lengthMax << " ," << endl
		<< "The size of lengthMin:   " << lengthMin << " ," << endl
		<< "The size of lengthMid:   " << lengthMid << " ," << endl
		<< "The size of heightMax:   " << heightMax << " ," << endl
		<< "The size of heightMin:   " << heightMin << " ," << endl
		<< "The size of rectX:   " << rectX << " ," << endl
		<< "The size of rectZ:   " << rectZ << " ," << endl
		<< "The size of circleX: " << circleX << " ," << endl
		<< "The size of circleR: " << circleR << " ," << endl;


	//对俯视图进行处理
	Mat imageROI2;
	imageROI2 = grayImage(Rect(0, grayImage.rows / 2 + 50, grayImage.cols / 2 + 50, grayImage.rows / 2 - 50));
	//俯视图强角点检测
	vector<Point2f> corners2;
	goodFeaturesToTrack(imageROI2, corners2, 20, qualityLevel, minDistance,
		Mat(), blockSize, false, k);
	lengthMin = grayImage.cols / 2;  lengthMax = 0;
	heightMin = grayImage.rows / 2;  heightMax = 0;
	for (unsigned int i = 0; i < corners2.size(); i++)
	{
		circle(imageROI2, corners2[i], r, Scalar(255, 0, 255), -1, 8, 0);
		if (corners2[i].y > heightMax) heightMax = corners2[i].y;
		if (corners2[i].y < heightMin) heightMin = corners2[i].y;
	}
	rectY = heightMax - heightMin;
	cout << endl << "In the top view:" << endl;
	cout << "The size of heightMax:   " << heightMax << " ," << endl
		<< "The size of heightMin:   " << heightMin << " ," << endl
		<< "The size of rectY: " << rectY << " ," << endl;
	imshow(WINDOW_NAME3, imageROI2);

	//计算质心的位置
	float p1, p2;
	cout << "分别输入长方体和圆柱体的密度:";
	cin >> p1 >> p2;
	cout << "以长方体主视图左下角的直角边建立直角坐标系!" << endl;
	float centerX, centerY, centerZ;
	float volume1, volume2;
	cout << "可知，左侧均质长方体的质心位置在: A(" << rectX / 2 << ","
		<< rectY / 2 << "," << rectZ / 2 << ")" << endl;
	cout << "  而，右侧均质圆柱体的质心位置在: B(" << rectX + circleX / 2 << ","
		<< rectY / 2 << "," << rectZ / 2 << ")" << endl;
	cout << "Calculating the numbers……" << endl;
	volume1 = rectX * rectY*rectZ;
	volume2 = circleR * circleR*PI*circleX;
	centerX = ((rectX / 2)*volume1*p1 + (rectX + circleX / 2)*volume2*p2) /
		(volume1*p1 + volume2 * p2);
	centerY = ((rectY / 2)*volume1*p1 + (rectY / 2)*volume2*p2) /
		(volume1*p1 + volume2 * p2);
	centerZ = ((rectZ / 2)*volume1*p1 + (rectZ / 2)*volume2*p2) /
		(volume1*p1 + volume2 * p2);
	cout << "得到物体的质心为: C(" << centerX << "," << centerY
		<< "," << centerZ << ")" << endl;
	//在主视图中做出质心的位置
	int rr = 5;
	Mat dstImage = srcImage.clone();
	circle(imageROI1, Point(115 + (int)centerX, 342 - (int)centerY), rr, Scalar(255, 0, 255), -1, 8, 0);

	imshow(WINDOW_NAME1, dstImage);
	imshow(WINDOW_NAME2, imageROI1);

	waitKey(0);
	return 0;
}

