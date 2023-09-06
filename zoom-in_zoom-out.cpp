//Zoom-in & Zoom out
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h> // (or others, depending on the system in use)
#include <math.h>

#define	width 			400
#define	height			600
#define	PI				3.1415
#define	polygon_num		10

int		left = 0;
int		bottom = 0;

int		mouse_motion = 0;

float	radius = 20.0;
float	point[1000][2];
int		point_num = 0;
float zoom = 1.0;

typedef struct _Point {
	float	x;
	float	y;
} Point;

Point	circle_center;

void Modeling_Axis(void) {
	glLineWidth(2.0);
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i(0, 0);
	glVertex2i(1.0 * width, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex2f(0.0, 0.0);
	glVertex2f(0.0, 1.0 * height);
	glEnd();
}

void Modeling_Circle(Point CC) {
	float	delta;

	glColor3f(1.0, 0.0, 0.0);
	glPointSize(3.0);
	delta = 2 * PI / polygon_num;
	glBegin(GL_POLYGON);
	for (int i = 0; i < polygon_num; i++)
		glVertex2f(CC.x + radius * cos(delta * i), CC.y + radius * sin(delta * i));
	glEnd();
}


void Modeling_Points_from_Screen(void) {
	glPointSize(5.0);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_POINTS);
	for (int k = 0; k < point_num; k++)
		glVertex2f(point[k][0], point[k][1]);
	glEnd();
}

void RenderScene(void) {
	glClearColor(1.0, 1.0, 0.0, 0.0); // Set display-window color to Yellow
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluOrtho2D (0, width, 0, height);  // mouse1() 	
	//gluOrtho2D(left, left + width, bottom, bottom + height); // mouse2()
	gluOrtho2D(left * zoom, (left + width) * zoom, bottom * zoom, (bottom + height) * zoom); // mouse3()

	Modeling_Axis();
	Modeling_Circle(circle_center);
	Modeling_Points_from_Screen();

	glFlush();
}

//관심영역이 옮겨질 경우 점이 원하는 위치에 찍히지 않음
void mouse1(int button, int state, int x, int y) { //x, y : 몇 번째 픽셀인지 저장
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { //왼쪽 버튼 누를 때
		point[point_num][0] = x;
		point[point_num][1] = height - y;
		point_num++;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		point_num = 0; //아무 점도 없는 것처럼 인식함 -> 57라인
	}
	glutPostRedisplay();
}

//관심영역이 옮겨지더라도 점이 원하는 위치에 찍힘
void mouse2(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		point[point_num][0] = left + x; //관심영역이 옮기진만큼 x에 더해줌
		point[point_num][1] = bottom + height - y;

		circle_center.x = point[point_num][0];
		circle_center.y = point[point_num][1];
		point_num++;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		point_num = 0;
	}
	glutPostRedisplay();
}

//좌클릭한 마우스 좌표를 기준으로 관심영역 변화
void mouse3(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) { //좌클릭하면 확대
		zoom *= 0.8;
		left += (x - width / 2) * zoom;
		bottom += -(y - height / 2) * zoom;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) { //우클릭하면 축소
		zoom *= 1.2;
		left += (x - width / 2) * zoom;
		bottom += -(y - height / 2) * zoom;
	}
	glutPostRedisplay();
}

void motion(int x, int y) { //좌클릭한 상태에서 드래그하면 빨간 원이 따라옴
	circle_center.x = left + x;
	circle_center.y = bottom + height - y;
	glutPostRedisplay();
}

void MySpecial(int key, int x, int y) { //키보드 특수 이벤트 받음
	switch (key) {
	case GLUT_KEY_LEFT:	
		left += 1.0; 
		break;
	case GLUT_KEY_RIGHT:	
		left -= 1.0; 
		break;
	case GLUT_KEY_DOWN:		
		bottom += 1.0; 
		break;
	case GLUT_KEY_UP:		
		bottom -= 1.0; 
		break;
	default:	
		break;
	}
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("Draw Points by Mouse Input");
	glutDisplayFunc(RenderScene);
	//glutMouseFunc(mouse1);
	//glutMouseFunc(mouse2);
	glutMouseFunc(mouse3);
	//glutMotionFunc(motion);
	glutSpecialFunc(MySpecial);
	glutMainLoop();
}