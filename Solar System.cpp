////태양계 시물레이션
//#include <windows.h>
//#include <math.h>
//#include <gl/gl.h>
//#include <gl/glut.h> // (or others, depending on the system in use)
//
//#define		PI	3.1415926
//
//int			Width = 800; //윈도우 너비
//int			Height = 800; //윈도우 높이
//
//int day = 0, times = 0; //날짜, 시간
//
//float		sun_radius = 0.4; //태양 반지름
//float		earth_radius = 0.12; //지구 반지름
//float		moon_radius = 0.04; //달 반지름
//
//float		earthRotation = 0.0, moonRotation = 0.0;
//
//float		earth_rotation_speed = 0.2;
//float		moon_rotation_speed = 2;
//
//
//void init(void) {
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	/*gluOrtho2D(-1.0 * Width / 2.0, Width / 2.0, -1.0 * Height / 2.0, Height / 2.0);*/
//	glOrtho(-3.0, 3.0, -3.0, 3.0, -2.0, 3.0);
//}
//
//
//void Draw_Circle(float c_radius) {
//	float	delta;
//	int		num = 30;
//
//	delta = 2 * PI / num;
//	glBegin(GL_POLYGON);
//	for (int i = 0; i < num; i++)
//		glVertex2f(c_radius * cos(delta * i), c_radius * sin(delta * i));
//	glEnd();
//}
//
//
//void MyTimer(int value) {
//	day = (day + 10) % 360;
//	times = (times + 15) % 360;
//
//	glutTimerFunc(100, MyTimer, 1);
//	glutPostRedisplay();
//}
//
//
//void RenderScene(void) {
//	glClearColor(0.0, 0.0, 0.0, 0.0); // Set display-window color to black.
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glEnable(GL_DEPTH_TEST);
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	earthRotation += earth_rotation_speed;
//	moonRotation += moon_rotation_speed;
//
//
//	/***********코드 작성하기 ***************/
//
//	/***지구***/
//	//glLoadIdentity(); // 전역 좌표계 초기화
//	glTranslatef(0.0, 0.0, -2.0);
//	glPushMatrix(); // 지구을 표현하는 지역 좌표계 추가
//	// 지구의 공전
//	glRotatef((float)day, 0.0, 1.0, 0.0);
//	glTranslatef(0.7, 0.0, 0.0);
//
//	// 지구의 자전
//	glRotatef((float)times, 0.0, 1.0, 0.0);
//	glColor3f(0.0, 1.0, 0.0); // 초록
//	glutSolidSphere(0.12, 30, 8); //지구 구 형태 출력
//	//Draw_Circle(earth_radius); //지구 2차원 원 형태 출력
//	
//	/***달***/
//	glPushMatrix(); // 달을 표현하는 지역 좌표계 추가
//	// 달의 공전
//	glRotatef((float)day, 0.0, 1.0, 0.0);
//	glTranslatef(0.2, 0.0, 0.0);
//	glColor3f(1.0, 1.0, 0.0); // 노랑
//	glutSolidSphere(0.04, 30, 8); // 달 구 형태 출력
//	//Draw_Circle(moon_radius); // 달 2차원 원 형태 출력
//
//	glPopMatrix(); // 저장해 둔 달의 지역 좌표계를 제거 함
//	glPopMatrix(); // 저장해 둔 지구의 지역 좌표계를 제거 함
//
//	glPopMatrix(); // 저장해둔 화성의 지역 좌표계를 제거 함
//
//	// 태양 : 빨간색의 구 
//	glColor3f(1.0, 0.0, 0.0); //빨강
//	glutSolidSphere(0.4, 30, 16); //태양 구 형태 출력
//	//Draw_Circle(sun_radius);//태양 2차원 원 형태 출력
//
//	glFlush();
//	glutSwapBuffers();
//}
//
//
//void main(int argc, char** argv) {
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//	glutInitWindowPosition(200, 200);
//	glutInitWindowSize(Width, Height); //윈도우 창 크기
//	glutCreateWindow("Solar System");
//
//	init();
//
//	glutDisplayFunc(RenderScene);
//	glutIdleFunc(RenderScene);
//	glutTimerFunc(100, MyTimer, 1);
//
//	glutMainLoop();
//}

#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)

#define		PI	3.1415926

int			Width = 800;
int			Height = 800;

float		sun_radius = 50.0;
float		earth_radius = 10.0;
float		moon_radius = 5.0;
float		jupiter_radius = 20.0;
float		galilean_radius = 8.0;

float		sun_to_Earth = 120.0;
float		earth_to_Moon = 30.0;
float		sun_to_Jupiter = 250.0;
float		jupiter_to_Galilean = 50.0;

float		earthR = 0.0, moonR = 0.0;
float		earth_speed = 0.01;
float		moon_speed = 0.05;

float		jupiterR = 100.0, galileanR = 0.0;
float		jupiter_speed = 0.09;
float		galilean_moon_speed = -3.0;


void init(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0 * Width / 2.0, Width / 2.0, -1.0 * Height / 2.0, Height / 2.0);
}

void Draw_Circle(float radius) {
	float	delta;
	int		num = 36;

	delta = 2 * PI / num;
	glBegin(GL_POLYGON);
	for (int i = 0; i < num; i++)
		glVertex2f(radius * cos(delta * i), radius * sin(delta * i));
	glEnd();
}

void My_solution(void)
{
	earthR += earth_speed;
	moonR += moon_speed;

	jupiterR += jupiter_speed;
	galileanR += galilean_moon_speed;


	glColor3f(1.0, 0.0, 0.0);
	Draw_Circle(sun_radius);

	glPushMatrix();
	glRotatef(earthR, 0.0, 0.0, 1.0);
	glTranslatef(sun_to_Earth, 0, 0);
	glColor3f(0.0, 1.0, 0.0);
	Draw_Circle(earth_radius);

	glPushMatrix();
	glRotatef(moonR, 0.0, 0.0, 1.0);
	glTranslatef(earth_to_Moon, 0, 0);
	glColor3f(1.0, 1.0, 0.0);
	Draw_Circle(moon_radius);
	glPopMatrix();

	/*glPushMatrix();
	glRotatef(jupiterR, 0.0, 0.0, 1.0);
	glTranslatef(sun_to_Jupiter, 0, 0);
	glColor3f(0.0, 1.0, 1.0);
	Draw_Circle(jupiter_radius);

	glPushMatrix();
	glRotatef(galileanR, 0.0, 0.0, 1.0);
	glTranslatef(jupiter_to_Galilean, 0, 0);
	glColor3f(1.0, 0.0, 1.0);
	Draw_Circle(galilean_radius);
	glPopMatrix();
	glPopMatrix();*/

	glPopMatrix();
}

void RenderScene(void) {
	//glClear 함수를 주석 처리하면 어떤 결과가 나올까요?
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0); // Set display-window color to black.

	My_solution();

	glutSwapBuffers();
	glFlush();

}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(Width, Height);
	glutCreateWindow("Solar System");
	init();
	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	glutMainLoop();
}

