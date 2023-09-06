#include <gl/glut.h>
#include <gl/gl.h>
#include <stdio.h>
#include <math.h>

#define	PI	3.1415926
float radius;
float theta, phi;
float delta_theta;
float delta_phi;
float R = 0.0, T = 2.0, speed = 0.015;
int flag = 0;

GLfloat		vertices[][3] = {
	{-1.0, 1.0, 0.0},	//0 : a
	{1.0, 1.0, 0.0},	//1 : c
	{1.0, -1.0, 0.0},	//2 : d
	{-1.0, -1.0, 0.0},	//3 : b

	{0.0, -1.0, 2.0},	//4 : k
	{0.0, 1.0, 2.0},	//5 : l
	{0.0, 1.0, 0.0},	//6 : f
	{0.0, -1.0, 0.0},	//7 : h

	{-1.0, 0.0, 2.0},	//8 : i
	{1.0, 0.0, 2.0},	//9 : j
	{1.0, 0.0, 0.0},	//10 : g
	{-1.0, 0.0, 0.0},	//11 : e
};

GLfloat		colors[][3] = {
		{ 0.0, 0.0, 1.0 },			// blue, 0
		{ 1.0, 0.0, 0.0 },			// red, 1
		{ 1.0, 1.0, 0.0 },			// yellow, 2
		{ 1.0, 1.0, 1.0 },			// white, 3

		{ 0.0, 1.0, 0.0 },			// green, 4
		{ 1.0, 0.0, 1.0 }, 			// magenta, 5
		{ 0.0, 0.0, 0.0 },			// black, 6
		{ 0.0, 0.0, 0.0 },			// black, 7

		{ 1.0, 1.0, 0.0 },			// yellow, 8
		{ 1.0, 1.0, 1.0 },			// white, 9
		{ 0.0, 0.0, 0.0 },			// black, 10
		{ 0.0, 0.0, 0.0 },			// black, 11
};


void init() { //은면제거법. 은면 제거 하는 방법 검색해보기.
	glEnable(GL_DEPTH_TEST);
	radius = 5.0;
	theta = 0.5;
	phi = 0.5;
	delta_theta = 0.1;
	delta_phi = 0.1;
}


void polygon(int a, int b, int c, int d) {
	glColor3fv(colors[a]);
	glBegin(GL_POLYGON);
	glVertex3fv(vertices[a]);
	glVertex3fv(vertices[b]);
	glVertex3fv(vertices[c]);
	glVertex3fv(vertices[d]);
	glEnd();
}


void cube(void) {
	if (flag % 2 == 1) {
		R += speed;
		T += speed / 100;
	}
	polygon(0, 1, 2, 3);	//blue

	glPushMatrix();
	glTranslatef(1.0, 0.0, 0.0);
	glRotatef(R, 0.0, 1.0, 0.0);
	polygon(4,5,6,7);	//green
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-1.0, 0.0, 0.0);
	glRotatef(R, 0.0, -1.0, 0.0);
	polygon(5,6,7,4);	//magenta
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, 1.0, 0.0);
	glRotatef(R, -1.0, 0.0, 0.0);
	polygon(8, 9, 10, 11);	//yellow
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0);
	glRotatef(R, 1.0, 0.0, 0.0);
	polygon(9, 10, 11, 8);	//white
	glPopMatrix();

	glTranslatef(0.0, 0.0, T);
	polygon(1, 2, 3, 0);	//red
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1.0, 20.0);
	//첫 번째 인자 : 값이 작아질수록 물체가 커짐
	//두 번째 인자 : x축으로 더 넓게 받아들이는데, 그것을 같은 크기로 그리려면 더 좁아지게됨
	//glOrtho(-5.0, 5.0, -5.0, 5.0, -10.0, 10.0); // == 카메라의 좌표계
}


void axis(void)
{
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); // x축 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(10.0, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0); // y축 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 10.0, 0.0);

	glColor3f(0.0, 0.0, 1.0); // z축 
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);
	glEnd();
}


void frame_reset(void) {
	glClearColor(0.6, 0.6, 0.6, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void camera(void) {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(10.0, 4.0, 3.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	float x = radius * cos(phi) * cos(theta);
	float y = radius * cos(phi) * cos(theta);
	float z = radius * sin(phi);
	gluLookAt(x, y, z + 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}


void display(void)
{
	frame_reset();

	camera(); // 카메라 설정
	axis(); // 축 그림
	cube(); // 큐브 그림

	glutSwapBuffers();
	glFlush();
}

void Keyboardkey(unsigned char key, int x, int y) {
	if (key == 't' || key == 'T') {
		flag += 1;
	}
	glutPostRedisplay();
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // GLUT_DEPTH : 카메라에서 물체까지의 거리
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow("cube");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(Keyboardkey);
	glutIdleFunc(display);
	glutMainLoop();
}