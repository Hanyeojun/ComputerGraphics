#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <iostream>
#include <string>
#include <ctime>

#define	width 			1000	//������ �ʺ�
#define	height			1000	//������ ����
#define	PI				3.1415 //���� ��
#define	polygon_num		50	//���� �𼭸�

int		wleft = 0; //������ â�� ����
int		wright = 1000; //������ â�� ����
int		wbottom = 0; //������ â�� �ϴ�
int		wtop = 1000; //������ â�� ���
float	radius	;	//�Ķ� �� ������

float	stick_x = 450.0, stick_y = 10.0;	//��ƽ�� x, y ��ǥ
float	stick_move = 15.0;	//��ƽ�� ������
float	stick_w = 100.0;	//��ƽ �ʺ�
float	stick_h = 20.0;		//��ƽ ����

float	Llowdist, Lhighdist, Rlowdist, Rhighdist; // ���� ������ �Ÿ�. ���ϴ�,�»��,���ϴ�,����

float bx = 280.0; //���� �ʱ� x ��ǥ
float by = 990.0; //���� �ʱ� y ��ǥ
float bw = 45.0; //���� �ʺ�
float bh = 25.0; //���� ����
int block_cnt = 36; //���� ����
int gamecnt = 0;

int startTime;  // Ÿ�̸� ���� �ð�

                   
//x, y ��ǥ ǥ���� ���� ����ü
typedef struct _Point {
	float	x;
	float	y;
} Point;

//������ x, y��ǥ�� �浹 ���� �Ǵ��� ���� ����ü
struct Block {
	float x;
	float y;
	int collision; // �浹 ����
};

//���� ������ ���� ����ü
struct Color{
	float R;
	float G;
	float B;
};


Point ball, velocity; //�����̴� ��, ���� �ӵ�
Point leftlow, leftmid, lefthigh, rightlow, rightmid, righthigh; //���� �̷�� ������ ��ǥ ��-���߻�, ��-���߻�
Block* block; //���� ����ü
Color Bcolor; //���� ���� ����ü


void init_Block(); //���� �ʱ�ȭ �Լ�


void init(void) 
{
	radius = 10.0; // �Ķ� �� ������
	ball.x = width / 2;	// �Ķ� �� x
	ball.y = height / 4; // �Ķ� �� y

	velocity.x = 0.08;	// x ���� �ӵ�
	velocity.y = 0.15;	// y ���� �ӵ�

	bx = 280.0;
	by = 990.0;
	bw = 45.0;
	bh = 25.0;
	block_cnt = 36;
	gamecnt = 0;


	leftlow.x = 200.0; //���ϴ� x
	leftlow.y = 0.0; //���ϴ� y

	leftmid.x = 400.0; //���ߴ� x
	leftmid.y = 500.0; //���ߴ� y

	lefthigh.x = 200.0; //�»�� x
	lefthigh.y = 1000.0; //�»�� y

	rightlow.x = 800.0; //���ϴ� x
	rightlow.y = 0.0; //���ϴ� y
	
	rightmid.x = 600.0; //���ߴ� x
	rightmid.y = 500.0; //���ߴ� y

	righthigh.x = 800.0; //���� x
	righthigh.y = 1000.0; //���� y

	gamecnt = 0; //�μ� ���� ī��Ʈ

	init_Block(); //���� �ʱ�ȭ
}


void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(wleft, wleft + width, wbottom, wbottom + height);
}


// Ÿ�̸� �ݹ� �Լ�
void Timer(int value) {
	int currentTime = glutGet(GLUT_ELAPSED_TIME);  // ���� ��� �ð� (�и��� ����)
	int elapsedTime = (currentTime - startTime) / 1000;  // �� ������ ��ȯ

	glutPostRedisplay();  // ȭ�� ���� ��û

	glutTimerFunc(1000, Timer, 0);  // 1�ʸ��� Ÿ�̸� ȣ��
}


//���׸���
void Modeling_Circle(float radius1, Point CC) {
	float	delta;

	delta = 2 * PI / polygon_num;
	glBegin(GL_POLYGON);
	for (int i = 0; i < polygon_num; i++)
		glVertex2f(CC.x + radius1 * cos(delta * i), CC.y + radius1 * sin(delta * i));
	glEnd();
}


//������ �׸���
void Draw_sandglass() {
	glBegin(GL_POLYGON);
	glColor3f(0.75, 0.73, 1.0);
	glVertex2f(200, 0);
	glVertex2f(400, 500);
	glVertex2f(600, 500);
	glVertex2f(800, 0);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.75, 0.73, 1.0);
	glVertex2f(400, 500);
	glVertex2f(200, 1000);
	glVertex2f(800, 1000);
	glVertex2f(600, 500);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(200, 0.5);
	glVertex2f(400, 500);
	glVertex2f(200, 999.5);
	glVertex2f(800, 999.5);
	glVertex2f(600, 500);
	glVertex2f(800, 0.5);
	glVertex2f(200, 0.5);
	glEnd();
}


//��ƽ �׸���
void Draw_stick() {
	glColor3f(0.3, 0.3, 0.3); //���� ���� ����
	glBegin(GL_POLYGON);
	glVertex2f(stick_x, stick_y); //���ϴ�
	glVertex2f(stick_x + stick_w, stick_y); //���ϴ�
	glVertex2f(stick_x + stick_w, stick_y + stick_h); //����
	glVertex2f(stick_x, stick_y + stick_h); //�»��
	glEnd();
}


//���� �ʱ�ȭ
void init_Block() {
	block = new Block[block_cnt]; //���� ���� ����
	int num = 0; //������ �׸� �� ���� �����ϱ� ���� ����
	for (int i = 0; i < block_cnt; i++) { //���� ��ü ������ŭ �ݺ�
		if (i == 9 || i == 18 || i == 27) { //9, 18, 27��° �׸� �� ���� ������ �Ѿ��
			num = 0; //���ο� ��
			by -= 30; //30��ŭ �ؿ� ���
		}
		//������ ũ�� x = 45, y = 25 (������ ũ�⸶�� +5�ȼ��Ͽ� �����ֱ�
		block[i].x = bx + (bw + 5) * num; //������ x��ǥ
		block[i].y = by; //������ y��ǥ

		block[i].collision = 0; //���� �浹���� �ʱ�ȭ
		num++; //������ ������ �� ������ �þ
	}
}


//���� �׸���
void Draw_block() {
	for (int i = 0; i < block_cnt; i++) {
		//9�� �������� ���� ���� ���� ��ȯ
		if (i % 9 == 0) { //�������� 0�� �� : ���� �迭 ��
			Bcolor.R = 0.8;
			Bcolor.G = 0.25;
			Bcolor.B = 0.25;
		}
		else if (i % 9 == 1) { //�������� 1�� �� : ��Ȳ �迭 ��
			Bcolor.R = 0.85;
			Bcolor.G = 0.55;
			Bcolor.B = 0.25;
		}
		else if (i % 9 == 2) { //�������� 2�� �� : ��� �迭 ��
			Bcolor.R = 0.8;
			Bcolor.G = 0.77;
			Bcolor.B = 0.4;
		}
		else if (i % 9 == 3) { //�������� 3�� �� : ���� �迭 ��
			Bcolor.R = 0.7;
			Bcolor.G = 0.8;
			Bcolor.B = 0.25;
		}
		else if (i % 9 == 4) { //�������� 4�� �� : �ʷ� �迭 ��
			Bcolor.R = 0.25;
			Bcolor.G = 0.6;
			Bcolor.B = 0.2;
		}
		else if (i % 9 == 5) { //�������� 5�� �� : �ϴ� �迭 ��
			Bcolor.R = 0.35;
			Bcolor.G = 0.8;
			Bcolor.B = 0.9;
		}
		else if (i % 9 == 6) { //�������� 6�� �� : ���� �迭 ��
			Bcolor.R = 0.15;
			Bcolor.G = 0.3;
			Bcolor.B = 0.7;
		}
		else if (i % 9 == 7) { //�������� 7�� �� : ���� �迭 ��
			Bcolor.R = 0.45;
			Bcolor.G = 0.3;
			Bcolor.B = 0.8;
		}
		else{ //�������� 8�� �� : ��ȫ �迭 ��
			Bcolor.R = 0.9;
			Bcolor.G = 0.45;
			Bcolor.B = 0.7;
		}
		glColor3f(Bcolor.R, Bcolor.G, Bcolor.B); //if���� ���� ���� ���� ����
		//�ð�������� ��ǥ�̵��ϸ鼭 ���� �׸���
		glBegin(GL_POLYGON);
		glVertex2f(block[i].x, block[i].y); //a : �»��
		glVertex2f(block[i].x + bw, block[i].y); //b : ����
		glVertex2f(block[i].x + bw, block[i].y - bh); //c : ���ϴ�
		glVertex2f(block[i].x, block[i].y - bh); //d : ���ϴ�
		glEnd();
	}
}


//���� ���� �浹
void Collide_Block2ball() {
	for (int i = 0; i < block_cnt; i++) {
		//���� �� ���� �Ÿ�
		float a = sqrt(pow(ball.x - block[i].x, 2) + pow(ball.y - block[i].y, 2)); //�»��
		float b = sqrt(pow(ball.x - block[i].x + bw, 2) + pow(ball.y - block[i].y, 2));	//����
		float c = sqrt(pow(ball.x - block[i].x + bw, 2) + pow(ball.y - block[i].y - by, 2)); //���ϴ�
		float d = sqrt(pow(ball.x - block[i].x, 2) + pow(ball.y - block[i].y - by, 2)); //���ϴ�

		//���� ������ �浹 �Ǵ�

		if ((block[i].x <= ball.x + radius) && (ball.x - radius <= block[i].x + bw)) { //���� �� �浹
			if((ball.y - radius <= block[i].y - bh) && (ball.y >= block[i].y - bh))
			{
				block[i].collision = 1; //�浹 ó��
				block[i].x = -100; //���� �����
				block[i].y = -100;
				velocity.y *= -1.0;
				gamecnt++; //�μ� ���� +1
			}
		}
		else {
			block[i].collision = 0;
		}
		if (a <= 0 || b <= 0 || c <= 0 || d <= 0) { //���� �𼭸� �浹
			block[i].collision = 1;
			block[i].x = -100;
			block[i].y = -100;
			velocity.y *= -1.0;
			gamecnt++;
		}
		else {
			block[i].collision = 0;
		}
	}
}


//��ƽ �����̱�
void Moving_stick(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:		//�������� �����̱�
		stick_x -= stick_move;
		break;
	case GLUT_KEY_RIGHT:	//���������� �����̱�
		stick_x += stick_move;
		break;
	case GLUT_KEY_UP:		//�������� �����̱�
		stick_y += stick_move;
		break;
	case GLUT_KEY_DOWN:		//�Ʒ������� �����̱�
		stick_y -= stick_move;
		break;
	case GLUT_KEY_F1:		//f1 ������ ���� �����
		init();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}


//���� ��ƽ �浹
void Collide_stick2wall() {
	if (stick_x <= 200.0) {	//���� ���� �浹
		stick_x = 200.0;
	}
	if (stick_x + stick_w >= 800.0) {	//������ ���� �浹
		stick_x = 700.0;
	}
	if (stick_y >= height / 2) { //�������� �߾ӱ����� �̵�
		stick_y = height / 2;
	}
	if (stick_y < 0) {	//������ �ϴ����� ����� �ʵ��� ����
		stick_y = 10.0;
	}
}


//��ƽ�� �� �浹
void Collide_stick2ball() {
	if (stick_x <= ball.x) {	//��ƽ ���� ��
		if (ball.x <= stick_x + stick_w) {	//��ƽ ���� ��
			if (stick_y + stick_h >= ball.y && stick_y <= ball.y) { //��ƽ�� y ��ǥ ��
				velocity.y *= -1.0;
			}
		}
	}
}


//�Ÿ� ���
void Distance() {
	//���ϴ�
	float m1 = 0.0, n1 = 0.0, h1 = 0.0, theta1 = 0.0;
	m1 = (leftmid.y - leftlow.y) / (leftmid.x - leftlow.x);
	n1 = (ball.y - leftlow.y) / (ball.x - leftlow.x);
	theta1 = atan(m1) - atan(n1);
	h1 = sqrt(pow((ball.x - leftlow.x), 2) + pow((ball.y - leftlow.y), 2));
	Llowdist = h1 * sin(theta1); //���ϴ� ������ �Ÿ�

	//�»��
	float m2 = 0.0, n2 = 0.0, h2 = 0.0, theta2 = 0.0;
	m2 = (lefthigh.y - leftmid.y) / (lefthigh.x - leftmid.x);
	n2 = (ball.y - leftmid.y) / (ball.x - leftmid.x);
	theta2 = atan(m2) - atan(n2);
	h2 = sqrt(pow((ball.x - leftmid.x), 2) + pow((ball.y - leftmid.y), 2));
	Lhighdist = h2 * sin(theta2); //�»�� ������ �Ÿ�

	//���ϴ�
	float m3 = 0.0, n3 = 0.0, h3 = 0.0, theta3 = 0.0;
	m3 = (rightlow.y - rightmid.y) / (rightlow.x - rightmid.x);
	n3 = (ball.y - rightmid.y) / (ball.x - rightmid.x);
	theta3 = atan(m3) - atan(n3);
	h3 = sqrt(pow((ball.x - rightlow.x), 2) + pow((ball.y - rightlow.y), 2));
	Rlowdist = h3 * sin(theta3); //���ϴ� ������ �Ÿ�
	
	//����
	float m4 = 0.0, n4 = 0.0, h4 = 0.0, theta4 = 0.0;
	m4 = (rightmid.y - righthigh.y) / (rightmid.x - righthigh.x);
	n4 = (ball.y - righthigh.y) / (ball.x - righthigh.x);
	theta4 = atan(m4) - atan(n4);
	h4 = sqrt(pow((ball.x - rightmid.x), 2) + pow((ball.y - rightmid.y), 2));
	Rhighdist = h4 * sin(theta4); //���� ������ �Ÿ�
}


//���� ���� �浹 �Լ�
void Collide_ball2wall(void) {
	//���ϴ�
	if (Llowdist <= 0 && (ball.x >= 200.0 && ball.x <= 400) && (ball.y >= 0.0 && ball.y <= 500.0)) {
		velocity.x *= -1.0;
		velocity.x = 0.1;
	}
	//�»��
	if (Lhighdist <= 0 && (ball.x >= 200.0 && ball.x <= 400) && (ball.y >= 500.0 && ball.y <= 1000.0)) {
		velocity.x *= -1.0;
		velocity.x = 0.12;
	}
	//���ϴ�
	if (Rlowdist <= 0 && (ball.x >= 600.0 && ball.x <= 800) && (ball.y >= 0.0 && ball.y <= 500.0)) {
		velocity.x *= -1.0;
		velocity.x = -0.1;
	}
	//����
	if (Rhighdist <= 0 && (ball.x >= 600.0 && ball.x <= 800) && (ball.y >= 500.0 && ball.y <= 1000.0)) {
		velocity.x *= -1.0;
		velocity.x = -0.12;
	}
	//��
	if (ball.x - radius <= wleft && velocity.x < 0.0) {
		velocity.x *= -1.0;
	}
	//��
	if (ball.x + radius >= wright && velocity.x > 0.0) {
		velocity.x *= -1.0;
	}
	//��
	if (ball.y + radius >= wtop && velocity.y > 0.0) {
		velocity.y *= -1.0;
	}
	//��
	if (ball.y - radius <= wbottom && velocity.y < 0.0) {
		velocity.x = 0.0;
		velocity.y = 0.0;
	}
}


void GameOver() {
	if (gamecnt == block_cnt) { //������ ������ �� �μ���
		velocity.x = 0; //�� ������ ����
		velocity.y = 0;
	}
}


void RenderScene(void) 
{
	glClearColor(0.2, 0.2, 0.2, 0.0); // Set display-window color to Yellow :������â ������
	glClear(GL_COLOR_BUFFER_BIT);

	Draw_sandglass();	//������ �׸���
	Draw_stick();	//��ƽ �׸���
	Draw_block();	//���� �׸���

	Collide_Block2ball();	//������ �� �浹
	Collide_stick2wall();	//��ƽ�� �� �浹
	Collide_stick2ball();	//��ƽ�� �� �浹
	Distance();				//������ �Ÿ� ���
	Collide_ball2wall();	//���� �� �浹

	// �����̴� ���� ��ġ ��ȭ 
	ball.x += velocity.x;
	ball.y += velocity.y;

	// �����̴� �� �׸��� 
	glColor3f(0.0, 0.1, 0.7);
	Modeling_Circle(radius, ball);

	// ����� �ð� ���
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = (currentTime - startTime) / 1000;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRasterPos2f(1.0, 0.0); // ��Ʈ�� ��Ʈ�� ��ġ

	glColor3f(1.0, 1.0, 1.0);  // �ؽ�Ʈ ���� (���)
	std::string timeStr = "Time: " + std::to_string(elapsedTime) + "s";
	for (char c : timeStr) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
	GameOver();

	glutSwapBuffers();
	glFlush();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowPosition(300, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(width, height);
	glutCreateWindow("Break Out Game");

	init();

	glutReshapeFunc(MyReshape);
	glutDisplayFunc(RenderScene);
	glutIdleFunc(RenderScene);
	glutSpecialFunc(Moving_stick);
	startTime = glutGet(GLUT_ELAPSED_TIME); // Ÿ�̸� ���� �ð� �ʱ�ȭ
	glutTimerFunc(1000, Timer, 0); //Ÿ�̸�
	glutMainLoop();
}