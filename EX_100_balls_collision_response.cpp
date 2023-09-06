#include <windows.h>
#include <math.h>
#include <gl/gl.h>
//#include <gl/glu.h>
#include <gl/glut.h> // (or others, depending on the system in use)

#define		PI			3.1415926
#define		epsilon		1.0

// Size of Clipping Window

float	radius = 5.0;
int		width = 500;
int		height = 500;
float	point_size = 3.0;
int		n = 0;

int		polygon_num = 36;		// 36각형 Ball
int		ball_num = 100;		// ball의 갯수     
//int		ball_num = 2;

//GLenum  draw_type;
int		collide = 0;

struct Point {
	float	x;
	float	y;
};

struct Vector {
	float	x;
	float	y;
};

struct Color {
	float	r;
	float	g;
	float	b;
};

struct Ball {
	Point	center;
	float	radius;
	float	mass;
	Color	color;
	Vector	velocity;
};

Ball	*ball;
Point	*polygon_vertex;

float	delta_x, delta_y;
float	fix_radius;

Point	Window_Center;  //초기 상태에서의 볼 분포의 중심
float	Window_radius;  //초기 상태에서의 볼 분포의 반경 


void init(void) {
	float	delta;
	float	distribution_delta;

	polygon_vertex = new Point[polygon_num];
	delta = 2.0 * PI / polygon_num;
	for (int i = 0; i < polygon_num; i++) {
		polygon_vertex[i].x = cos(delta * i);
		polygon_vertex[i].y = sin(delta * i);
	}

	Window_radius = width / 3.0;
	Window_Center.x = width / 2.0;
	Window_Center.y = height / 2.0;

	distribution_delta = 2.0 * PI / ball_num;
	ball = new Ball[ball_num];

	for (int i = 0; i < ball_num; i++) {
		ball[i].radius = radius * (1.5 + sin((double)(i)));
		ball[i].center.x = Window_Center.x + Window_radius * cos((double)(distribution_delta * i));
		ball[i].center.y = Window_Center.y + Window_radius * sin((double)(distribution_delta * i));
		ball[i].velocity.x = -1.0*(ball[i].center.x - Window_Center.x) / Window_radius;
		ball[i].velocity.y = -1.0*(ball[i].center.y - Window_Center.y) / Window_radius;
		ball[i].mass = 1.5 + sin((double)(i));
		ball[i].color.r = (float)(1.0 - (i % 3) / 2);
		ball[i].color.g = (float)(1.0 - (ball_num - i) / ball_num);
		ball[i].color.b = (float)(1.0 * (i % 10) / 9);
	}

}


void	Draw_Ball(int	index) {
	float	x, y, radius;

	x = ball[index].center.x;
	y = ball[index].center.y;
	radius = ball[index].radius;

	glColor3f(ball[index].color.r, ball[index].color.g, ball[index].color.b);
	glBegin(GL_POLYGON);
		for (int i = 0; i < polygon_num; i++)
			glVertex3f(x + radius * polygon_vertex[i].x, y + radius * polygon_vertex[i].y, 0.0);
	glEnd();
}

void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
}

void	Check_Collision_Ball_Wall(int index) {
	// 오른쪽 벽
	if (ball[index].center.x + ball[index].radius > width &&  ball[index].velocity.x > 0.0)
		ball[index].velocity.x *= (-1.0);

	// 왼쪽 벽
	if (ball[index].center.x - ball[index].radius < 0 && ball[index].velocity.x < 0.0)
		ball[index].velocity.x *= (-1.0);

	// 윗쪽 벽
	if (ball[index].center.y + ball[index].radius > height && ball[index].velocity.y > 0.0)
		ball[index].velocity.y *= (-1.0);

	//아래쪽 벽
	if (ball[index].center.y - ball[index].radius < 0 && ball[index].velocity.y < 0.0)
		ball[index].velocity.y *= (-1.0);
}


void	Check_Collision_Ball_Ball(int i, int j) {
	float	distance;
	Vector	VA, VB, R_AB, normal;
	float	inner_value;

	normal.x = ball[j].center.x - ball[i].center.x;
	normal.y = ball[j].center.y - ball[i].center.y;

	distance = sqrt(normal.x * normal.x + normal.y * normal.y);

	if (distance < ball[j].radius + ball[i].radius) {
		distance = sqrt(normal.x * normal.x + normal.y * normal.y);

		normal.x /= distance;
		normal.y /= distance;

		VA.x = ball[i].velocity.x;
		VA.y = ball[i].velocity.y;

		VB.x = ball[j].velocity.x;
		VB.y = ball[j].velocity.y;

		R_AB.x = VA.x - VB.x;
		R_AB.y = VA.y - VB.y;

		inner_value = R_AB.x * normal.x + R_AB.y * normal.y;
		if (inner_value > 0.0) {
			ball[i].velocity.x = VA.x - (1 + epsilon) * ball[j].mass * inner_value * normal.x / (ball[i].mass + ball[j].mass);
			ball[i].velocity.y = VA.y - (1 + epsilon) * ball[j].mass * inner_value * normal.y / (ball[i].mass + ball[j].mass);

			ball[j].velocity.x = VB.x + (1 + epsilon) * ball[i].mass * inner_value * normal.x / (ball[i].mass + ball[j].mass);
			ball[j].velocity.y = VB.y + (1 + epsilon) * ball[i].mass * inner_value * normal.y / (ball[i].mass + ball[j].mass);

			collide = 1;
		}
	}
}

void	Update_Position(void) {
	for (int i = 0; i < ball_num; i++) {
		ball[i].center.x += ball[i].velocity.x;
		ball[i].center.y += ball[i].velocity.y;
	}
}


void RenderScene(void) {
	int		i;

	glClearColor(1.0, 0.3, 0.3, 0.0); // Set display-window color to white.
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);

	for (i = 0; i < ball_num; i++)
		Check_Collision_Ball_Wall(i);

	for (i = 0; i < ball_num; i++) {
		for (int j = i + 1; j < ball_num; j++) {
			if (i != j)
				Check_Collision_Ball_Ball(i, j);
		}
	}

	Update_Position();

	for (i = 0; i < ball_num; i++)
		Draw_Ball(i);

	glFlush();
	glutSwapBuffers();
}



void SpecialKey(int key, int x, int y) {
	int		i;

	switch (key) {
	case GLUT_KEY_LEFT:
		for (i = 0; i < ball_num; i++) {
			ball[i].velocity.x *= 0.9;
			ball[i].velocity.y *= 0.9;
		}
		break;
	case GLUT_KEY_RIGHT:
		for (i = 0; i < ball_num; i++) {
			ball[i].velocity.x *= 1.1;
			ball[i].velocity.y *= 1.1;
		}
		break;
	default:				break;
	}
}


void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(width, height);
	glutCreateWindow("100 Balls Collision Detection & Response");
	init();
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(MyReshape);
	glutSpecialFunc(SpecialKey);
	glutIdleFunc(RenderScene);
	glutMainLoop();
}