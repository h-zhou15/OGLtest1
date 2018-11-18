
#include"pch.h"
#include "BasicObj.h"
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

#define Bit8 256;
LPCWSTR VoicePath = TEXT("C:\\Users\\hao\\Desktop\\资料\\三维可视化\\贴图\\碰撞声.wav");
LPCWSTR BmpPath = TEXT("C:\\Users\\hao\\Desktop\\资料\\三维可视化\\贴图\\滑稽.bmp");

//GLuint texture; //纹理图

//材质，光照设置
void Ball::Settings()
{

	GLfloat mat_ambient2[] = { 0.250000, 0.207250, 0.207250, 0.922000 };
	GLfloat mat_diffuse2[] = { 1.000000, 0.829000, 0.829000, 0.922000 };
	GLfloat mat_specular2[] = { 0.296648, 0.296648, 0.296648, 0.922000 };
	GLfloat mat_shininess2[] = { 11.264000 }; //材质RGBA镜面指数，数值在0～128范围内

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);
}

	
void Ball::isOut(double _TableX,double _TableY,double _TableZ)
{
	//判断是否碰到桌子壁
	//目前只是二维情况，后续修改为三维

	if (fabs(fabs(position.x) + radius) >= _TableX || fabs(fabs(position.z) + radius) >= _TableZ)
	{
		Music();
		if (fabs(fabs(position.x) + radius) >= _TableX)
		{
			velocity.vx = -velocity.vx;
		}
		if (fabs(fabs(position.z) + radius) >= _TableZ)
			velocity.vz = -velocity.vz;
	}
}

void BallPosition(Ball &ball_1,Ball &ball_2,double TimeInterval)
{
	//更新位置信息
	ball_1.position.x = ball_1.position.x + ball_1.velocity.vx*TimeInterval;
	ball_1.position.y = ball_1.position.y + ball_1.velocity.vy*TimeInterval;
	ball_1.position.z = ball_1.position.z + ball_1.velocity.vz*TimeInterval;

	ball_2.position.x = ball_2.position.x + ball_2.velocity.vx*TimeInterval;
	ball_2.position.y = ball_2.position.y + ball_2.velocity.vy*TimeInterval;
	ball_2.position.z = ball_2.position.z + ball_2.velocity.vz*TimeInterval;

	//判断两球是否相碰，根据球心相距的距离来看
	double distance = sqrt(pow(ball_1.position.x - ball_2.position.x, 2)
		+ pow(ball_1.position.y - ball_2.position.y, 2)
		+ pow(ball_1.position.z - ball_2.position.z, 2));

	if (distance <= (ball_1.radius + ball_2.radius)) {ball_1.Flag = 1; ball_2.Flag = 1;}
	else { ball_1.Flag = 0; ball_2.Flag = 0; }


	if (ball_1.Flag&&ball_2.Flag&&(ball_1.Flag!=ball_1.oldFlag)&&(ball_2.Flag!=ball_2.oldFlag))	//表示两球相碰,且上一帧 不在碰撞状态
	{

		Music();
		//根据动量守恒和动能定理在各维度求变化后的速度
		//求出两球连线的夹角
		double SinTheta = (ball_2.position.z - ball_1.position.z) / distance;
		double CosTheta = (ball_2.position.x - ball_1.position.x) / distance;

		//将碰撞前的速度分解为平行于连线方向和垂直与连线方向上
		double Vparal_1 = ball_1.velocity.vx*CosTheta + ball_1.velocity.vz*SinTheta;
		double Vvertic_1 = ball_1.velocity.vx*SinTheta + ball_1.velocity.vz*CosTheta;

		double Vparal_2 = ball_2.velocity.vx*CosTheta + ball_2.velocity.vz*SinTheta;
		double Vvertic_2 = ball_2.velocity.vx*SinTheta + ball_2.velocity.vz*CosTheta;

		//两球 碰撞后沿着连线方向的速度变化，垂直与连线方向上的速度不变
		double Vparal_1_ = (ball_1.weight - ball_2.weight)*Vparal_1 / (ball_1.weight + ball_2.weight)
			+ 2 * ball_2.weight*Vparal_2 / (ball_1.weight + ball_2.weight);
		double Vparal_2_ = (ball_2.weight - ball_1.weight)*Vparal_2 / (ball_1.weight + ball_2.weight)
			+ 2 * ball_1.weight*Vparal_1 / (ball_1.weight + ball_2.weight);

		//求出碰撞后球沿x，z方向上的速度
		ball_1.velocity.vx = Vparal_1_ * CosTheta + Vvertic_1 * SinTheta;
		ball_1.velocity.vz = Vparal_1_ * SinTheta + Vvertic_1 * CosTheta;

		ball_2.velocity.vx = Vparal_2_ * CosTheta + Vvertic_2 * SinTheta;
		ball_2.velocity.vz = Vparal_2_ * SinTheta + Vvertic_2 * CosTheta;
	}

	ball_1.oldFlag = ball_1.Flag;
	ball_2.oldFlag = ball_2.Flag;
}



int Ball::loadTexture(IplImage *image, GLuint *text)
{
	if (image == NULL) return -1;
	glGenTextures(1, text);
	glBindTexture(GL_TEXTURE_2D, *text); //bind the texture to it's array
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height,
		0, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);

	return 0;
}


IplImage* Ball::LoadImg()
{
	IplImage* img = cvLoadImage("C:\\Users\\hao\\Desktop\\资料\\三维可视化\\贴图\\滑稽.bmp",1);
	return img;
}

void Music()
{
	PlaySound(VoicePath, NULL, SND_FILENAME | SND_ASYNC);
}