
#include"pch.h"
#include<GL/glut.h>
#include<iostream>
#include<math.h>
#include<windows.h>
using namespace std;

//��İ뾶
float radius = 1;
int Old_Collision_Flag = 0;		//�����Ƿ�����ײ
int New_Collision_Flag = 0;
int Flag_Change = 0;				//���λ�Ƿ�仯

//1���2����ٶ���λ��
float Speed1[3] = { 10,0,-6 };
float Position1[3] = { 7,radius,-5 };
float Speed2[3] = { 7,0,10 };
float Position2[3] = { -5,radius,4 };

float TestStepk = radius / 200;	//ģ���ʱ����
float Stepk = 0.02;

float GroundX = 10;	//ƽ�壨���棩��X��Z�����ϵĳߴ�
float GroundZ = 10;

//////////////////////
//////////////////////
#define M_PI 3.1415926
static float c = M_PI / 180.0f; //���ȺͽǶ�ת������  
static int du = 90, oldmy = -1, oldmx = -1; //du���ӵ���y��ĽǶ�,opengl��Ĭ��y�����Ϸ���  
static float r = 10.0f, h = 10.0f; //r���ӵ���y��İ뾶,h���ӵ�߶ȼ���y���ϵ�����  
////////////////////////
//��ʼ���趨
void myInit()
{

	//������ɫģʽ
	glShadeModel(GL_SMOOTH);
	//���ó�ʼ����ɫ�������ɫ�������Ȼ���
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

	//������ȼ�⣬��ֻ������ǰ���һ��
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

//����С��λ�õĺ���
void BallPosition()
{

	//����С��ǰ���ٶ�
	//�ж�С���Ƿ�����
	if (Position1[0] > GroundX - radius || Position1[0] < -GroundX + radius)
		Speed1[0] = -Speed1[0];
	if (Position1[2] > GroundZ - radius || Position1[2] < -GroundZ + radius)
		Speed1[2] = -Speed1[2];
	if (Position2[0] > GroundX - radius || Position2[0] < -GroundX + radius)
		Speed2[0] = -Speed2[0];
	if (Position2[2] > GroundZ - radius || Position2[2] < -GroundZ + radius)
		Speed2[2] = -Speed2[2];

	//�ж���С���Ƿ���ײ
	float BallDistance = sqrt(pow(Position1[0] - Position2[0], 2) + pow(Position1[2] - Position2[2], 2));
	if (BallDistance <= 2 * radius) New_Collision_Flag = 1;
	else New_Collision_Flag = 0;
	if (New_Collision_Flag != Old_Collision_Flag) Flag_Change = 1;
	else Flag_Change = 0;
	Old_Collision_Flag = New_Collision_Flag;

	if (New_Collision_Flag == 1 && Flag_Change == 1)
	{
		//X,Z�����ϵľ���
		float DisX = Position1[0] - Position2[0];
		float DisZ = Position1[2] - Position2[2];
		float cosxita = DisX / BallDistance;
		float sinxita = DisZ / BallDistance;

		//����С�����߷����ϣ����ٶ�
		float Speed1R = Speed1[0] * cosxita + Speed1[2] * sinxita;
		float Speed2R = Speed2[0] * cosxita + Speed2[2] * sinxita;
		//�����ϵ��ٶ�
		float Speed1N = Speed1[0] * sinxita - Speed1[2] * cosxita;
		float Speed2N = Speed2[0] * sinxita - Speed2[2] * cosxita;
		//�������ٶȽ������������ٶȲ���
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

//��ʾ�ĺ���
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//����С��ǰ��λ��

	///////////////////

	//printf("At:%.2f %.2f %.2f\n",r*cos(c*du),h,r*sin(c*du)); //������ӵ������  
	glLoadIdentity();
	gluLookAt(r*cos(c*du), h, r*sin(c*du), 0, 0, 0, 0, 1, 0); //���ӵ㿴Զ��,y�᷽��(0,1,0)���Ϸ���  

	glColor3f(1.0f, 0.0f, 0.0f);

	//���õذ����
	GLfloat mat_ambient[] = { 0.021500, 0.174500, 0.021500, 0.550000 };
	GLfloat mat_diffuse[] = { 0.075680, 0.614240, 0.075680, 0.550000 };
	GLfloat mat_specular[] = { 0.633000, 0.727811, 0.633000, 0.550000 };
	GLfloat mat_shininess[] = { 76.800003 }; //����RGBA����ָ������ֵ��0��128��Χ��


	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	BoardDisplay();					//���Ƶذ�

	BallPosition();
	//�ƶ�����ǰС���λ�ã�����С��
	//����С�����
	GLfloat mat_ambient2[] = { 0.250000, 0.207250, 0.207250, 0.922000 };
	GLfloat mat_diffuse2[] = { 1.000000, 0.829000, 0.829000, 0.922000 };
	GLfloat mat_specular2[] = { 0.296648, 0.296648, 0.296648, 0.922000 };
	GLfloat mat_shininess2[] = { 11.264000 }; //����RGBA����ָ������ֵ��0��128��Χ��

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient2);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular2);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess2);


	glPushMatrix();
	glTranslatef(Position1[0], Position1[1], Position1[2]);
	glutSolidSphere(radius, 40, 40);
	glPopMatrix();

	//����С�����
	GLfloat mat_ambient3[] = { 0.174500, 0.011750, 0.011750, 0.550000 };
	GLfloat mat_diffuse3[] = { 0.614240, 0.041360, 0.041360, 0.550000 };
	GLfloat mat_specular3[] = { 0.727811, 0.626959, 0.626959, 0.550000 };
	GLfloat mat_shininess3[] = { 76.800003 }; //����RGBA����ָ������ֵ��0��128��Χ��

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
	glutPostRedisplay();//��ǵ�ǰ������Ҫ���»��ƣ�����myDisplay()
	glutTimerFunc(20, myOnTimer, 1);
}

void Mouse(int button, int state, int x, int y) //���������  
{
	if (state == GLUT_DOWN) //��һ����갴��ʱ,��¼����ڴ����еĳ�ʼ����  
	{
		oldmx = x, oldmy = y;
		//cout<<x<<','<<y<<'\n';
	}
}
void onMouseMove(int x, int y) //��������϶�  
{
	//printf("%d\n",du);  
	du += x - oldmx; //����ڴ���x�᷽���ϵ������ӵ��ӵ���y��ĽǶ��ϣ�����ת��  
	h += 0.03f*(y - oldmy); //����ڴ���y�᷽���ϵĸı�ӵ��ӵ��y�����ϣ�����ת  
	oldmx = x, oldmy = y; //�Ѵ�ʱ�����������Ϊ��ֵ��Ϊ��һ�μ���������׼��  
}

//reshape����
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
		cout << "ʹ��Ĭ�ϲ�����0���ǣ�1����\n";
		cin >> defaultt;
		if (defaultt != 0 && defaultt != 1)
			cout << "������Ϸ�����\n";
		else if (defaultt == 1)
		{
			cout << "�����������С\n";
			cin >> GroundX >> GroundZ;
			cout << "��������İ뾶\n";
			cin >> radius;
			Position1[1] = radius; Position2[1] = radius;
			cout << "��������1�����꣨��ά��\n";
			cin >> Position1[0] >> Position1[2];
			cout << "��������2�����꣨��ά��\n";
			cin >> Position2[0] >> Position2[2];
			cout << "��������1���ٶȣ���ά��\n";
			cin >> Speed1[0] >> Speed1[2];
			cout << "��������2���ٶȣ���ά��\n";
			cin >> Speed2[0] >> Speed2[2];
		}
	}

	//
	glutInit(&argc, argv);
	//���ڵ���ز����趨����������ʾ����
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
