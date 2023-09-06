#include <windows.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)

#define		PI				3.1415926
#define		Window_Width	500
#define		Window_Height	500

int			num = 10;
float		radius = 30;

int x_min, x_max, y_min, y_max;

GLenum draw_type = GL_POLYGON;

void Modeling_Circle(void) {
	float	delta, theta;
	float	x, y;

	glColor3f(1.0, 1.0, 0.0);
	glPointSize(3.0);
	delta = 2 * PI / num;

	glBegin(draw_type);
	for (int i = 0; i < num; i++) {
		theta = delta * i;
		x = radius * cos(theta);
		y = radius * sin(theta);
		glVertex2f(x, y);
	}
	glEnd();
}

void Modeling_Axis(void) {
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(-250.0, 0.0);
	glVertex2f(250.0, 0.0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(0, -250.0);
	glVertex2f(0, 250.0);
	glEnd();
}

void Modeling_Rectangle(void) {
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2i(100, 100);
	glVertex2i(150, 100);
	glVertex2i(150, 150);
	glVertex2i(100, 150);
	glEnd();
}

void Modeling_Ground(void) {
	glColor3f(1.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	glVertex2i(250, 250);
	glVertex2i(-250, 250);
	glVertex2i(-250, -250);
	glVertex2i(250, -250);
	glEnd();
}

void draw_Another(void) {
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	glVertex2i(300, -150);
	glVertex2i(500, -150);
	glVertex2i(500, 150);
	glVertex2i(300, 150);
	glEnd();
}


void RenderScene(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	Modeling_Ground();
	Modeling_Rectangle();	// 1사분면에 있는 정사각형 
	Modeling_Axis();		// x축과 y축 
	Modeling_Circle();		// 반지름의 크기가 30인 원
	draw_Another();

	glFlush();
}

void MyReshape(int w, int h) {
	x_min = w;
	x_max = w;
	y_min = h;
	y_max = h;

	glViewport(0, 0, w, h); // 사물이 그려지는 영역 설정 : 전체 창 사용
	glMatrixMode(GL_PROJECTION); // 투상 좌표계의 공간을 앞으로 계산하겠다는 뜻
	glLoadIdentity(); //행렬 처리가 이루어지기 전에 좌표계를 초기화
	gluOrtho2D(-1.0 / 2.0 * w, 1.0 / 2.0 * w, -1.0 / 2.0 * h, 1.0 / 2.0 * h); //직교 투영(x최소, x최대, y최소, y최대)
}

void MyKey(unsigned char key, int x, int y) { //들어오는 키, 픽셀의 x, y 좌표
	switch (key)
	{
	case '1':
		radius -= 5;
		break;
	case '2':
		radius += 5;
		break;
	case '3':
		if (num > 3) num -= 1;
		break;
	case '4':
		num += 1;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void SpecialKey(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		y_min += 50;
		y_max -= 50;
		glMatrixMode(GL_PROJECTION); // 투상 좌표계의 공간을 앞으로 계산하겠다는 뜻
		glLoadIdentity(); //행렬 처리가 이루어지기 전에 좌표계를 초기화
		gluOrtho2D(-1.0 / 2.0 * x_min, 1.0 / 2.0 * x_max, -1.0 / 2.0 * y_min, 1.0 / 2.0 * y_max); //직교 투영(x최소, x최대, y최소, y최대)
		break;
	case GLUT_KEY_DOWN:
		y_min -= 50;
		y_max += 50;
		glMatrixMode(GL_PROJECTION); // 투상 좌표계의 공간을 앞으로 계산하겠다는 뜻
		glLoadIdentity(); //행렬 처리가 이루어지기 전에 좌표계를 초기화
		gluOrtho2D(-1.0 / 2.0 * x_min, 1.0 / 2.0 * x_max, -1.0 / 2.0 * y_min, 1.0 / 2.0 * y_max); //직교 투영(x최소, x최대, y최소, y최대)
		break;
	case GLUT_KEY_LEFT:
		x_min -= 50;
		x_max += 50;
		glMatrixMode(GL_PROJECTION); // 투상 좌표계의 공간을 앞으로 계산하겠다는 뜻
		glLoadIdentity(); //행렬 처리가 이루어지기 전에 좌표계를 초기화
		gluOrtho2D(-1.0 / 2.0 * x_min, 1.0 / 2.0 * x_max, -1.0 / 2.0 * y_min, 1.0 / 2.0 * y_max); //직교 투영(x최소, x최대, y최소, y최대)
		break;
	case GLUT_KEY_RIGHT:
		x_min += 50;
		x_max -= 50;
		glMatrixMode(GL_PROJECTION); // 투상 좌표계의 공간을 앞으로 계산하겠다는 뜻
		glLoadIdentity(); //행렬 처리가 이루어지기 전에 좌표계를 초기화
		gluOrtho2D(-1.0 / 2.0 * x_min, 1.0 / 2.0 * x_max, -1.0 / 2.0 * y_min, 1.0 / 2.0 * y_max); //직교 투영(x최소, x최대, y최소, y최대)
		break;
	case GLUT_KEY_F1:
		draw_type = GL_LINES;
		break;
	case GLUT_KEY_F2:
		draw_type = GL_TRIANGLES;
		break;
	case GLUT_KEY_F3:
		draw_type = GL_QUADS;
		break;
	case GLUT_KEY_F4:
		draw_type = GL_LINE_LOOP;
		break;
	case GLUT_KEY_F5:
		draw_type = GL_POLYGON;
		break;
	default:
		break;
	}
	glutPostRedisplay(); //다시 그리기
}

int main(int argc, char** argv) {
	glutInit(&argc, argv); //glut관련 구조체 초기화

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //디스플레이 기본 컬러모드 RGB 설정
	glutInitWindowPosition(100, 100); //윈도우 위치 지정
	glutInitWindowSize(Window_Width, Window_Height); //윈도우창 설정
	glutCreateWindow("Task2"); //윈도우창 생성

	glutDisplayFunc(RenderScene); //RenderScene()함수를 디스플레이 이벤트에 대한 콜백 함수로 등록
	glutReshapeFunc(MyReshape); //렌더링 크기 변경에 따라 윈도우 크기 재설정

	glutKeyboardFunc(MyKey); //문자키 입력받아서 MyKey() 동작
	glutSpecialFunc(SpecialKey); //특수문자키 입력받아서 동작

	glutMainLoop(); //무한루프

	return 0;
}

// 원 크기 조절
// 원을 형성하는 선분의 수 조절 => O
// 스페셜 이벤트를 받아들여서 => O
// 좌우 및 상하 방향키를 누르면 clipping 영역 변하도록 => O
// function키를 누르면 원을 나타내는 물체의 모양이 변하도록
// 점들의 집합, 선분들의 집합, LINELOOP, POLYGON 등등