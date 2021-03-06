// OGLtest1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include "pch.h"
#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include<windows.h>
#include "BasicObj.h"
#include <opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<string>
using namespace std;
using namespace cv;


#define PI 3.1415926
//#define IMG_PATH  "C:\\Users\\hao\\Desktop\\资料\\三维可视化\\贴图\\滑稽.bmp"

//string path = "C:\\Users\\hao\\Desktop\\资料\\三维可视化\\贴图\\滑稽.bmp";
string imgpath = "C:\\Users\\hao\\Desktop\\资料\\三维可视化\\贴图\\滑稽.bmp";
GLuint texture; //纹理图

double TimeInterval = 0.08;
GLfloat TableSizeX = 10.0;
GLfloat TableSizeY = 0.0;
GLfloat TableSizeZ = 15.0;

GLfloat _X = 0;
GLfloat _Z = 20;
GLfloat eyeR = 10;
GLfloat Theta = 0;

GLint Width = 800;
GLint Height = 600;

Ball ball_1(-2.0, 1.0, 0.0, //-2，1，0
	10.0, 0.0, 4.0, //10，0，4
	1, 1.0,
	0, 0);
Ball ball_2(2.0, 1.0, 0.0,//坐标	2，1，0  
	-5, 0.0, 5.0, //速度			-5，0，5
	1, 1.0,		//球的半径和质量
	0, 0);	//,碰撞old、标志位

void myInit()
{

//	ball_1.LoadImg(imgpath);


		//设置着色模式
	glShadeModel(GL_SMOOTH);
	//设置初始背景色，清除颜色缓存和深度缓存
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// light source attributes
	GLfloat LightAmbient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightDiffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat LightSpecular[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat LightPosition[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//	gluLookAt(0, 10, 40, 0, 0, 0, 0, 1, 0);
	//	glLoadIdentity();
		//设置深度检测，即只绘制最前面的一层
	glEnable(GL_DEPTH_TEST);
}

void TableInit(void)
{
	glColor3f(0.5, 1.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex3f(-TableSizeX, TableSizeY, TableSizeZ);
	glVertex3f(-TableSizeX, TableSizeY, -TableSizeZ);
	glVertex3f(TableSizeX, TableSizeY, -TableSizeZ);
	glVertex3f(TableSizeX, TableSizeY, TableSizeZ);
	glEnd();

	//	glutSwapBuffers();
	glFlush();
}


void Mydisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(eyeR*sin(Theta), 10, eyeR*cos(Theta)+15, 0, 0, 0, 0, 1, 0);
	glColor3f(1.0, 1.0, 1.0);

	//判断是否碰壁,如果碰壁则对应一同做速度改变
	ball_1.isOut(TableSizeX, TableSizeY, TableSizeZ);
	ball_2.isOut(TableSizeX, TableSizeY, TableSizeZ);

	//绘制桌面

	//设置地板材质
	GLfloat mat_ambient[] = { 0.021500, 0.174500, 0.021500, 0.550000 };
	GLfloat mat_diffuse[] = { 0.075680, 0.614240, 0.075680, 0.550000 };
	GLfloat mat_specular[] = { 0.633000, 0.727811, 0.633000, 0.550000 };
	GLfloat mat_shininess[] = { 76.800003 }; //材质RGBA镜面指数，数值在0～128范围内


	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	TableInit();

	//计算当前小球的位置
	BallPosition(ball_1, ball_2, TimeInterval);


	//绘制小球并转移坐标系
	
	ball_1.Settings();

	glPushMatrix();
	glTranslatef(ball_1.position.x, ball_1.position.y, ball_1.position.z);
	
	glEnable(GL_TEXTURE_2D);	//开启纹理贴图

	glutSolidSphere(ball_1.radius, 20, 20);
//	glutSwapBuffers();
	glPopMatrix();



	GLfloat mat_ambient3[] = { 0.174500, 0.011750, 0.011750, 0.550000 };
	GLfloat mat_diffuse3[] = { 0.614240, 0.041360, 0.041360, 0.550000 };
	GLfloat mat_specular3[] = { 0.727811, 0.626959, 0.626959, 0.550000 };
	GLfloat mat_shininess3[] = { 76.800003 }; //材质RGBA镜面指数，数值在0～128范围内

	glPushMatrix();
	glTranslatef(ball_2.position.x, ball_2.position.y, ball_2.position.z);
	glutSolidSphere(ball_2.radius, 20, 20);
	glPopMatrix();

//	glFlush();
	glutSwapBuffers();
}

void TimerFunc(int value)
{

	//标记当前窗口需要重绘
	glutPostRedisplay();
	//	glLoadIdentity();
		//结尾定时器回调
	glutTimerFunc(50, TimerFunc, 1);

}
/*
void MyTexture()
{
	//贴图工作

	Mat Img = imread(path);
	imshow("滑稽", Img);
	//设置长宽
	int ImgWidth = Img.cols + (512 - 400);		//512
	int ImgHeight = Img.rows + (512 - 400);
	int ImgChannel = Img.channels();

	//cout << "宽：" << ImgWidth << "height:" << ImgHeight << endl;

	//获取图像指针
	int PixelLength = ImgWidth * ImgHeight*ImgChannel;
	GLubyte* Pixels = new GLubyte[PixelLength];

}*/

void OnMouse(int button, int state, int x, int y)
{}

void MouseMotion(int x, int y)
{
	_X = x - 0.5*Width;		//转换成世界坐标
	_Z = 0.5*Height - y;

	Theta = atan(_X / _Z);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0f, (float)w / h, 1.0f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char *argv[])
{
	IplImage* img = cvLoadImage("C:\\Users\\hao\\Desktop\\资料\\三维可视化\\贴图\\滑稽.bmp", 1);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("小球碰撞");
	myInit();
	glutReshapeFunc(reshape);
	glutMouseFunc(OnMouse);
	glutMotionFunc(MouseMotion);
	glutDisplayFunc(Mydisplay);
	glutTimerFunc(250, TimerFunc, 1);
	ball_1.loadTexture(img, &texture);

	glutMainLoop();
	//	return 0;
}
