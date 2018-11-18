
#include"pch.h"
#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include<windows.h>
using namespace std;

//球的半径
float radius = 1;
int Old_Collision_Flag = 0;		//两球是否发生碰撞
int New_Collision_Flag = 0;
int Flag_Change = 0;				//标记位是否变化

//1球和2球的速度与位置
float Speed1[3] = { 10,0,-6 };
float Position1[3] = { 7,radius,-5 };
float Speed2[3] = { 7,0,10 };
float Position2[3] = { -5,radius,4 };

float TestStepk = radius / 200;	//模拟的时间间隔
float Stepk = 0.02;

float GroundX = 10;	//平板（桌面）在X和Z方向上的尺寸
float GroundZ = 10;

//////////////////////
//////////////////////
#define M_PI 3.1415926
static float c = M_PI / 180.0f; //弧度和角度转换参数  
static int du = 90, oldmy = -1, oldmx = -1; //du是视点绕y轴的角度,opengl里默认y轴是上方向  
static float r = 10.0f, h = 10.0f; //r是视点绕y轴的半径,h是视点高度即在y轴上的坐标  
////////////////////////
//初始化设定
void myInit()
{

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

	//设置深度检测，即只绘制最前面的一层
	glEnable(GL_DEPTH_TEST);
}

////////////////////////////
void BoardDisplay(void)
{
	glColor3f(0.5, 0.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex3f(-GroundX, 0, GroundZ);
	glVertex3f(-GroundX, 0, -GroundZ);
	glVertex3f(GroundX, 0, -GroundZ);
	glVertex3f(GroundX, 0, GroundZ);
	glEnd();
	glFlush();
}
///////////////////////////

//计算小球位置的函数
void BallPosition()
{

	//计算小球当前的速度
	//判断小球是否碰壁
	if (Position1[0] > GroundX - radius || Position1[0] < -GroundX + radius)
		Speed1[0] = -Speed1[0];
	if (Position1[2] > GroundZ - radius || Position1[2] < -GroundZ + radius)
		Speed1[2] = -Speed1[2];
	if (Position2[0] > GroundX - radius || Position2[0] < -GroundX + radius)
		Speed2[0] = -Speed2[0];
	if (Position2[2] > GroundZ - radius || Position2[2] < -GroundZ + radius)
		Speed2[2] = -Speed2[2];

	//判断两小球是否碰撞
	float BallDistance = sqrt(pow(Position1[0] - Position2[0], 2) + pow(Position1[2] - Position2[2], 2));
	if (BallDistance <= 2 * radius) New_Collision_Flag = 1;
	else New_Collision_Flag = 0;
	if (New_Collision_Flag != Old_Collision_Flag) Flag_Change = 1;
	else Flag_Change = 0;
	Old_Collision_Flag = New_Collision_Flag;

	if (New_Collision_Flag == 1 && Flag_Change == 1)
	{
		//X,Z方向上的距离
		float DisX = Position1[0] - Position2[0];
		float DisZ = Position1[2] - Position2[2];
		float cosxita = DisX / BallDistance;
		float sinxita = DisZ / BallDistance;

		//径向（小球连线方向上）的速度
		float Speed1R = Speed1[0] * cosxita + Speed1[2] * sinxita;
		float Speed2R = Speed2[0] * cosxita + Speed2[2] * sinxita;
		//法向上的速度
		float Speed1N = Speed1[0] * sinxita - Speed1[2] * cosxita;
		float Speed2N = Speed2[0] * sinxita - Speed2[2] * cosxita;
		//径向上速度交换，法向上速度不变
		float t;
		t = Speed1R; Speed1R = Speed2R; Speed2R = t;
		Speed1[0] = Speed1R * cosxita + Speed1N * sinxita;
		Speed1[2] = Speed1R * sinxita - Speed1N * cosxita;
		Speed2[0] = Speed2R * cosxita + Speed2N * sinxita;
		Speed2[2] = Speed2R * sinxita - Speed2N * cosxita;
	}
	//else
	//{
	for (int i = 0; i <= 2; i++)
	{
		Position1[i] = Position1[i] + Speed1[i] * Stepk;
		Position2[i] = Position2[i] + Speed2[i] * Stepk;
	}
	//}
}

//显示的函数
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//计算小球当前的位置

	///////////////////

	//printf("At:%.2f %.2f %.2f\n",r*cos(c*du),h,r*sin(c*du)); //这就是视点的坐标  
	glLoadIdentity();
	gluLookAt(r*cos(c*du), h, r*sin(c*du), 0, 0, 0, 0, 1, 0); //从视点看远点,y轴方向(0,1,0)是上方向  

	glColor3f(1.0f, 0.0f, 0.0f);

	//设置地板材质
	GLfloat mat_ambient[] = { 0.021500, 0.174500, 0.021500, 0.550000 };
	GLfloat mat_diffuse[] = { 0.075680, 0.614240, 0.075680, 0.550000 };
	GLfloat mat_specular[] = { 0.633000, 0.727811, 0.633000, 0.550000 };
	GLfloat mat_shininess[] = { 76.800003 }; //材质RGBA镜面指数，数值在0～128范围内


	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	BoardDisplay();					//绘制地板

	BallPosition();
	//移动到当前小球的位置，画出小球
	//设置小球材质
	GLfloat mat_ambient2[] = { 0.250000, 0.207250, 0.207250, 0.922000 };
	GLfloat mat_diffuse2[] = { 1.000000, 0.829000, 0.829000, 0.922000 };
	GLfloat mat_specular2[] = { 0.296648, 0.296648, 0.296648, 0.922000 };
	GLfloat mat_shininess2[] = { 11.264000 }; //材质RGBA镜面指数，数值在0～128范围内

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);


	glPushMatrix();
	glTranslatef(Position1[0], Position1[1], Position1[2]);
	glutSolidSphere(radius, 40, 40);
	glPopMatrix();

	//设置小球材质
	GLfloat mat_ambient3[] = { 0.174500, 0.011750, 0.011750, 0.550000 };
	GLfloat mat_diffuse3[] = { 0.614240, 0.041360, 0.041360, 0.550000 };
	GLfloat mat_specular3[] = { 0.727811, 0.626959, 0.626959, 0.550000 };
	GLfloat mat_shininess3[] = { 76.800003 }; //材质RGBA镜面指数，数值在0～128范围内

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient3);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse3);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular3);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess3);

	glPushMatrix();
	glTranslatef(Position2[0], Position2[1], Position2[2]);
	glutSolidSphere(radius, 40, 40);
	glPopMatrix();

	glutSwapBuffers();
	//////////////////
}

void myOnTimer(int value)
{
	glutPostRedisplay();//标记当前窗口需要重新绘制，调用myDisplay()
	glutTimerFunc(20, myOnTimer, 1);
}

void Mouse(int button, int state, int x, int y) //处理鼠标点击  
{
	if (state == GLUT_DOWN) //第一次鼠标按下时,记录鼠标在窗口中的初始坐标  
	{
		oldmx = x, oldmy = y;
		//cout<<x<<','<<y<<'\n';
	}
}
void onMouseMove(int x, int y) //处理鼠标拖动  
{
	//printf("%d\n",du);  
	du += x - oldmx; //鼠标在窗口x轴方向上的增量加到视点绕y轴的角度上，左右转了  
	h += 0.03f*(y - oldmy); //鼠标在窗口y轴方向上的改变加到视点的y坐标上，上下转  
	oldmx = x, oldmy = y; //把此时的鼠标坐标作为旧值，为下一次计算增量做准备  
}

//reshape函数
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75.0f, (float)w / h, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char**argv)
{
	int defaultt = 3;
	for (; defaultt != 1 && defaultt != 0;)
	{
		cout << "使用默认参数？0：是，1：否\n";
		cin >> defaultt;
		if (defaultt != 0 && defaultt != 1)
			cout << "请输入合法参数\n";
		else if (defaultt == 1)
		{
			cout << "请输入桌板大小\n";
			cin >> GroundX >> GroundZ;
			cout << "请输入球的半径\n";
			cin >> radius;
			Position1[1] = radius; Position2[1] = radius;
			cout << "请输入球1的坐标（二维）\n";
			cin >> Position1[0] >> Position1[2];
			cout << "请输入球2的坐标（二维）\n";
			cin >> Position2[0] >> Position2[2];
			cout << "请输入球1的速度（二维）\n";
			cin >> Speed1[0] >> Speed1[2];
			cout << "请输入球2的速度（二维）\n";
			cin >> Speed2[0] >> Speed2[2];
		}
	}

	//
	glutInit(&argc, argv);
	//窗口的相关参数设定，并创建显示窗口
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("hw1");

	//
	myInit();
	glutReshapeFunc(reshape);
	glutDisplayFunc(myDisplay);
	glutTimerFunc(20, myOnTimer, 1);
	glutMouseFunc(Mouse);
	glutMotionFunc(onMouseMove);
	glutMainLoop();
}
