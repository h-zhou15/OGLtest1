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

GLuint texture; //纹理图
IplImage *img;
GLfloat angle = 0.0;
unsigned char* textureImage;
static GLint spinx = 0;
static GLint spiny = 0;

//自定义函数，读入图片作为纹理图
void makeTextureImg(IplImage *image)
{
	int width = image->width;//对全局变量赋值 
	int height = image->height;
	CvScalar s;

	//读入彩色图
	/*textureImage = new unsigned char[width * height * 3];
	for (int i=0; i<height; i++)
	for (int j=0; j<width; j ++)
	{
	s = cvGet2D(image, i, j);
	textureImage[i * 3 * width + 3 * j] = s.val[0];
	textureImage[i * 3 * width + 3 * j + 1] = s.val[1];
	textureImage[i * 3 * width + 3 * j + 2] = s.val[2];
	}  */

	//读入灰度图
	textureImage = new unsigned char[width * height];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			s = cvGet2D(image, i, j);//获取对应点的像素值  
			textureImage[i * width + j] = s.val[0];
		}
}

int loadTexture(IplImage *image, GLuint *text)
{
	if (image == NULL) return -1;
	glGenTextures(1, text);
	glBindTexture(GL_TEXTURE_2D, *text); //bind the texture to it's array
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	//方法一：通过makeTextureImg()函数获取由openCV提供的图像灰度信息
	//方法一解析具体实现过程
	//makeTextureImg(image);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, image->width, image->height, 
	//0, GL_LUMINANCE, GL_UNSIGNED_BYTE, textureImage);

	//方法二：通过image->imageData函数获取由openCV提供的图像灰度信息
	//GL_RGB，GL_LUMINANCE分别指读入图像为彩色图，灰度图
	//方法二更直接更简单

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height,
		0, GL_RGB, GL_UNSIGNED_BYTE, image->imageData);

	//glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, image->width, image->height, 
	//0, GL_LUMINANCE, GL_UNSIGNED_BYTE, image->imageData);
	return 0;
}

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glRotatef((GLfloat)spinx, 1.0, 0.0, 0.0);//左右旋转
	glRotatef((GLfloat)spiny, 0.0, 1.0, 0.0);//上下旋转
	glEnable(GL_TEXTURE_2D);//开启纹理特性
	glutSolidTeapot(2.0);//实心茶壶为opengl中提供的例子
	glutSwapBuffers();//双缓存
	angle = angle + 0.01;
	glPopMatrix();
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

//定义鼠标交互
void mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		switch (state) {
		case GLUT_DOWN:
			spinx = (spinx + 5) % 360;
			glutPostRedisplay();//标记当前窗口需要重新绘制
			break;
		default:
			break;
		}
		break;

	case GLUT_MIDDLE_BUTTON:
		switch (state) {
		case GLUT_DOWN:
			spiny = (spiny + 5) % 360;
			glutPostRedisplay();
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("A basic OpenGL Window");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);

	//读入纹理图
	img = cvLoadImage("C:\\Users\\hao\\Desktop\\资料\\三维可视化\\贴图\\滑稽.bmp", 1);//读入彩色图
	//img = cvLoadImage("F:/qt/project_try/imageProcess_3/data/2D/car.bmp", 0);//读入灰度图
	loadTexture(img, &texture);

	glutMainLoop();

	return 0;
}
