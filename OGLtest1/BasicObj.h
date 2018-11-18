#pragma once

#include"pch.h"

#include<cmath>
#include<algorithm>
#include<GL/glut.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<string>
#include<windows.h>
#include"mmsystem.h"
#pragma comment(lib,"Winmm.lib")
using namespace std;
using namespace cv;

class Coordinate
{
public:
	Coordinate(double a, double b, double c) :x(a), y(b), z(c) {}
	double x;
	double y;
	double z;
};

class Velocity {
public:
	Velocity(double a, double b, double c)
	{
		vx = a; vy = b; vz = c;
	}
	double vx;
	double vy;
	double vz;
};

//球的相关信息
class Ball
{
public:
	Coordinate position;
	Velocity velocity;
	double radius;
	double weight;
	bool oldFlag;
	bool Flag;

	Ball(double _x, double _y, double _z, double _vx, double _vy, double _vz, double _radius, double _m,double _oldflag,double _flag) :
		position(_x, _radius, _z),velocity(_vx, _vy, _vz), 
		radius(_radius), weight(_m),oldFlag(_oldflag),Flag(_flag) {}

	void isOut(double _TableX,double _TableY,double _TableZ);

	int loadTexture(IplImage *image, GLuint *text);

	IplImage* LoadImg();
	//void VelocityChange();

	//小球的材质、光照设置
	void Settings();


};

//void BallCollision(Ball &ball_1, Ball &ball_2);

void BallPosition(Ball &ball_1, Ball &ball_2,double TimeInterval);



//音效
void Music();

//int loadTexture(cv::IplImage *image, GLuint *text);