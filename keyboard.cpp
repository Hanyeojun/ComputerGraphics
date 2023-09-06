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
	Modeling_Rectangle();	// 1��и鿡 �ִ� ���簢�� 
	Modeling_Axis();		// x��� y�� 
	Modeling_Circle();		// �������� ũ�Ⱑ 30�� ��
	draw_Another();

	glFlush();
}

void MyReshape(int w, int h) {
	x_min = w;
	x_max = w;
	y_min = h;
	y_max = h;

	glViewport(0, 0, w, h); // �繰�� �׷����� ���� ���� : ��ü â ���
	glMatrixMode(GL_PROJECTION); // ���� ��ǥ���� ������ ������ ����ϰڴٴ� ��
	glLoadIdentity(); //��� ó���� �̷������ ���� ��ǥ�踦 �ʱ�ȭ
	gluOrtho2D(-1.0 / 2.0 * w, 1.0 / 2.0 * w, -1.0 / 2.0 * h, 1.0 / 2.0 * h); //���� ����(x�ּ�, x�ִ�, y�ּ�, y�ִ�)
}

void MyKey(unsigned char key, int x, int y) { //������ Ű, �ȼ��� x, y ��ǥ
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
		glMatrixMode(GL_PROJECTION); // ���� ��ǥ���� ������ ������ ����ϰڴٴ� ��
		glLoadIdentity(); //��� ó���� �̷������ ���� ��ǥ�踦 �ʱ�ȭ
		gluOrtho2D(-1.0 / 2.0 * x_min, 1.0 / 2.0 * x_max, -1.0 / 2.0 * y_min, 1.0 / 2.0 * y_max); //���� ����(x�ּ�, x�ִ�, y�ּ�, y�ִ�)
		break;
	case GLUT_KEY_DOWN:
		y_min -= 50;
		y_max += 50;
		glMatrixMode(GL_PROJECTION); // ���� ��ǥ���� ������ ������ ����ϰڴٴ� ��
		glLoadIdentity(); //��� ó���� �̷������ ���� ��ǥ�踦 �ʱ�ȭ
		gluOrtho2D(-1.0 / 2.0 * x_min, 1.0 / 2.0 * x_max, -1.0 / 2.0 * y_min, 1.0 / 2.0 * y_max); //���� ����(x�ּ�, x�ִ�, y�ּ�, y�ִ�)
		break;
	case GLUT_KEY_LEFT:
		x_min -= 50;
		x_max += 50;
		glMatrixMode(GL_PROJECTION); // ���� ��ǥ���� ������ ������ ����ϰڴٴ� ��
		glLoadIdentity(); //��� ó���� �̷������ ���� ��ǥ�踦 �ʱ�ȭ
		gluOrtho2D(-1.0 / 2.0 * x_min, 1.0 / 2.0 * x_max, -1.0 / 2.0 * y_min, 1.0 / 2.0 * y_max); //���� ����(x�ּ�, x�ִ�, y�ּ�, y�ִ�)
		break;
	case GLUT_KEY_RIGHT:
		x_min += 50;
		x_max -= 50;
		glMatrixMode(GL_PROJECTION); // ���� ��ǥ���� ������ ������ ����ϰڴٴ� ��
		glLoadIdentity(); //��� ó���� �̷������ ���� ��ǥ�踦 �ʱ�ȭ
		gluOrtho2D(-1.0 / 2.0 * x_min, 1.0 / 2.0 * x_max, -1.0 / 2.0 * y_min, 1.0 / 2.0 * y_max); //���� ����(x�ּ�, x�ִ�, y�ּ�, y�ִ�)
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
	glutPostRedisplay(); //�ٽ� �׸���
}

int main(int argc, char** argv) {
	glutInit(&argc, argv); //glut���� ����ü �ʱ�ȭ

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); //���÷��� �⺻ �÷���� RGB ����
	glutInitWindowPosition(100, 100); //������ ��ġ ����
	glutInitWindowSize(Window_Width, Window_Height); //������â ����
	glutCreateWindow("Task2"); //������â ����

	glutDisplayFunc(RenderScene); //RenderScene()�Լ��� ���÷��� �̺�Ʈ�� ���� �ݹ� �Լ��� ���
	glutReshapeFunc(MyReshape); //������ ũ�� ���濡 ���� ������ ũ�� �缳��

	glutKeyboardFunc(MyKey); //����Ű �Է¹޾Ƽ� MyKey() ����
	glutSpecialFunc(SpecialKey); //Ư������Ű �Է¹޾Ƽ� ����

	glutMainLoop(); //���ѷ���

	return 0;
}

// �� ũ�� ����
// ���� �����ϴ� ������ �� ���� => O
// ����� �̺�Ʈ�� �޾Ƶ鿩�� => O
// �¿� �� ���� ����Ű�� ������ clipping ���� ���ϵ��� => O
// functionŰ�� ������ ���� ��Ÿ���� ��ü�� ����� ���ϵ���
// ������ ����, ���е��� ����, LINELOOP, POLYGON ���