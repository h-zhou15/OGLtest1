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

GLuint texture; //����ͼ
IplImage *img;
GLfloat angle = 0.0;
unsigned char* textureImage;
static GLint spinx = 0;
static GLint spiny = 0;

//�Զ��庯��������ͼƬ��Ϊ����ͼ
void makeTextureImg(IplImage *image)
{
	int width = image->width;//��ȫ�ֱ�����ֵ 
	int height = image->height;
	CvScalar s;

	//�����ɫͼ
	/*textureImage = new unsigned char[width * height * 3];
	for (int i=0; i<height; i++)
	for (int j=0; j<width; j ++)
	{
	s = cvGet2D(image, i, j);
	textureImage[i * 3 * width + 3 * j] = s.val[0];
	textureImage[i * 3 * width + 3 * j + 1] = s.val[1];
	textureImage[i * 3 * width + 3 * j + 2] = s.val[2];
	}  */

	//����Ҷ�ͼ
	textureImage = new unsigned char[width * height];
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			s = cvGet2D(image, i, j);//��ȡ��Ӧ�������ֵ  
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

	//����һ��ͨ��makeTextureImg()������ȡ��openCV�ṩ��ͼ��Ҷ���Ϣ
	//����һ��������ʵ�ֹ���
	//makeTextureImg(image);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, image->width, image->height, 
	//0, GL_LUMINANCE, GL_UNSIGNED_BYTE, textureImage);

	//��������ͨ��image->imageData������ȡ��openCV�ṩ��ͼ��Ҷ���Ϣ
	//GL_RGB��GL_LUMINANCE�ֱ�ָ����ͼ��Ϊ��ɫͼ���Ҷ�ͼ
	//��������ֱ�Ӹ���

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
	glRotatef((GLfloat)spinx, 1.0, 0.0, 0.0);//������ת
	glRotatef((GLfloat)spiny, 0.0, 1.0, 0.0);//������ת
	glEnable(GL_TEXTURE_2D);//������������
	glutSolidTeapot(2.0);//ʵ�Ĳ��Ϊopengl���ṩ������
	glutSwapBuffers();//˫����
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

//������꽻��
void mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		switch (state) {
		case GLUT_DOWN:
			spinx = (spinx + 5) % 360;
			glutPostRedisplay();//��ǵ�ǰ������Ҫ���»���
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

	//��������ͼ
	img = cvLoadImage("C:\\Users\\hao\\Desktop\\����\\��ά���ӻ�\\��ͼ\\����.bmp", 1);//�����ɫͼ
	//img = cvLoadImage("F:/qt/project_try/imageProcess_3/data/2D/car.bmp", 0);//����Ҷ�ͼ
	loadTexture(img, &texture);

	glutMainLoop();

	return 0;
}
