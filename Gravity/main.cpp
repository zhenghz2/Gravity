#include <iostream>
#include <vector>
#include<opencv2/opencv.hpp>
#include<chrono>
#include <math.h>
#define WINDOW_NAME1 "����ͼ"
#define WINDOW_NAME2 "����ͼ"
#define WINDOW_NAME3 "����ͼ"
using namespace cv;
using namespace std;
#define PI 3.14159
//����һ���޸�
//ʵ��1

int main()
{
	Mat srcImage = imread("Sanshitu20181029.bmp");
	cout << "Loading the pictures!" << endl;
	if (!srcImage.data) 
	{ 
		cout << "��ʼ������ͼƬʧ��!" << endl;
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
	cout << "Dealing with the pictures����" << endl;
	//��Բ����ͳ���������ݽ��ж���
	int rectX, rectY, rectZ;
	int circleX, circleR;

	//������ͼ���д���
	Mat imageROI1;
	imageROI1 = grayImage(Rect(0, 0, grayImage.cols / 2 + 50, grayImage.rows / 2 + 50));
	//����ͼǿ�ǵ���
	vector<Point2f> corners1;
	goodFeaturesToTrack(imageROI1, corners1, 20, qualityLevel, minDistance,
		Mat(), blockSize, false, k);
	int lengthMin = grayImage.cols / 2, lengthMax = 0;
	int heightMin = grayImage.rows / 2, heightMax = 0;
	int lengthMid; int tempheight;
	for (unsigned int i = 0; i < corners1.size(); i++)
	{
		circle(imageROI1, corners1[i], r, Scalar(255, 0, 255), -1, 8, 0);
		//��ȡ��������������Сֵ
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
	//�õ���������
	rectX = lengthMid - lengthMin;
	rectZ = heightMax - heightMin;
	circleX = lengthMax - lengthMid;
	circleR = abs((heightMax + heightMin) / 2 - tempheight);
	//������
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


	//�Ը���ͼ���д���
	Mat imageROI2;
	imageROI2 = grayImage(Rect(0, grayImage.rows / 2 + 50, grayImage.cols / 2 + 50, grayImage.rows / 2 - 50));
	//����ͼǿ�ǵ���
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

	//�������ĵ�λ��
	float p1, p2;
	cout << "�ֱ����볤�����Բ������ܶ�:";
	cin >> p1 >> p2;
	cout << "�Գ���������ͼ���½ǵ�ֱ�Ǳ߽���ֱ������ϵ!" << endl;
	float centerX, centerY, centerZ;
	float volume1, volume2;
	cout << "��֪�������ʳ����������λ����: A(" << rectX / 2 << ","
		<< rectY / 2 << "," << rectZ / 2 << ")" << endl;
	cout << "  �����Ҳ����Բ���������λ����: B(" << rectX + circleX / 2 << ","
		<< rectY / 2 << "," << rectZ / 2 << ")" << endl;
	cout << "Calculating the numbers����" << endl;
	volume1 = rectX * rectY*rectZ;
	volume2 = circleR * circleR*PI*circleX;
	centerX = ((rectX / 2)*volume1*p1 + (rectX + circleX / 2)*volume2*p2) /
		(volume1*p1 + volume2 * p2);
	centerY = ((rectY / 2)*volume1*p1 + (rectY / 2)*volume2*p2) /
		(volume1*p1 + volume2 * p2);
	centerZ = ((rectZ / 2)*volume1*p1 + (rectZ / 2)*volume2*p2) /
		(volume1*p1 + volume2 * p2);
	cout << "�õ����������Ϊ: C(" << centerX << "," << centerY
		<< "," << centerZ << ")" << endl;
	//������ͼ���������ĵ�λ��
	int rr = 5;
	Mat dstImage = srcImage.clone();
	circle(imageROI1, Point(115 + (int)centerX, 342 - (int)centerY), rr, Scalar(255, 0, 255), -1, 8, 0);

	imshow(WINDOW_NAME1, dstImage);
	imshow(WINDOW_NAME2, imageROI1);

	waitKey(0);
	return 0;
}

