#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <math.h>
#include <iostream>
#include <string>
#include <ctime>

#define	width 			1000	//윈도우 너비
#define	height			1000	//윈도우 높이
#define	PI				3.1415 //파이 값
#define	polygon_num		50	//원의 모서리

int		wleft = 0; //윈도우 창의 좌측
int		wright = 1000; //윈도우 창의 우측
int		wbottom = 0; //윈도우 창의 하단
int		wtop = 1000; //윈도우 창의 상단
float	radius	;	//파란 공 반지름

float	stick_x = 450.0, stick_y = 10.0;	//스틱의 x, y 좌표
float	stick_move = 15.0;	//스틱의 움직임
float	stick_w = 100.0;	//스틱 너비
float	stick_h = 20.0;		//스틱 높이

float	Llowdist, Lhighdist, Rlowdist, Rhighdist; // 공과 벽과의 거리. 좌하단,좌상단,우하단,우상단

float bx = 280.0; //벽돌 초기 x 좌표
float by = 990.0; //벽돌 초기 y 좌표
float bw = 45.0; //벽돌 너비
float bh = 25.0; //벽돌 높이
int block_cnt = 36; //벽돌 개수
int gamecnt = 0;

int startTime;  // 타이머 시작 시간

                   
//x, y 좌표 표현의 위한 구조체
typedef struct _Point {
	float	x;
	float	y;
} Point;

//벽돌의 x, y좌표와 충돌 여부 판단을 위한 구조체
struct Block {
	float x;
	float y;
	int collision; // 충돌 여부
};

//색상 선택을 위한 구조체
struct Color{
	float R;
	float G;
	float B;
};


Point ball, velocity; //움직이는 공, 공의 속도
Point leftlow, leftmid, lefthigh, rightlow, rightmid, righthigh; //벽을 이루는 직선의 좌표 좌-하중상, 우-하중상
Block* block; //벽돌 구조체
Color Bcolor; //벽돌 색상 구조체


void init_Block(); //벽돌 초기화 함수


void init(void) 
{
	radius = 10.0; // 파란 공 반지름
	ball.x = width / 2;	// 파란 공 x
	ball.y = height / 4; // 파란 공 y

	velocity.x = 0.08;	// x 방향 속도
	velocity.y = 0.15;	// y 방향 속도

	bx = 280.0;
	by = 990.0;
	bw = 45.0;
	bh = 25.0;
	block_cnt = 36;
	gamecnt = 0;


	leftlow.x = 200.0; //좌하단 x
	leftlow.y = 0.0; //좌하단 y

	leftmid.x = 400.0; //좌중단 x
	leftmid.y = 500.0; //좌중단 y

	lefthigh.x = 200.0; //좌상단 x
	lefthigh.y = 1000.0; //좌상단 y

	rightlow.x = 800.0; //우하단 x
	rightlow.y = 0.0; //우하단 y
	
	rightmid.x = 600.0; //우중단 x
	rightmid.y = 500.0; //우중단 y

	righthigh.x = 800.0; //우상단 x
	righthigh.y = 1000.0; //우상단 y

	gamecnt = 0; //부순 벽돌 카운트

	init_Block(); //벽돌 초기화
}


void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(wleft, wleft + width, wbottom, wbottom + height);
}


// 타이머 콜백 함수
void Timer(int value) {
	int currentTime = glutGet(GLUT_ELAPSED_TIME);  // 현재 경과 시간 (밀리초 단위)
	int elapsedTime = (currentTime - startTime) / 1000;  // 초 단위로 변환

	glutPostRedisplay();  // 화면 갱신 요청

	glutTimerFunc(1000, Timer, 0);  // 1초마다 타이머 호출
}


//원그리기
void Modeling_Circle(float radius1, Point CC) {
	float	delta;

	delta = 2 * PI / polygon_num;
	glBegin(GL_POLYGON);
	for (int i = 0; i < polygon_num; i++)
		glVertex2f(CC.x + radius1 * cos(delta * i), CC.y + radius1 * sin(delta * i));
	glEnd();
}


//게임판 그리기
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


//스틱 그리기
void Draw_stick() {
	glColor3f(0.3, 0.3, 0.3); //선의 색상 선택
	glBegin(GL_POLYGON);
	glVertex2f(stick_x, stick_y); //좌하단
	glVertex2f(stick_x + stick_w, stick_y); //우하단
	glVertex2f(stick_x + stick_w, stick_y + stick_h); //우상단
	glVertex2f(stick_x, stick_y + stick_h); //좌상단
	glEnd();
}


//벽돌 초기화
void init_Block() {
	block = new Block[block_cnt]; //벽돌 개수 저장
	int num = 0; //벽돌을 그릴 때 행을 변경하기 위한 변수
	for (int i = 0; i < block_cnt; i++) { //벽돌 전체 개수만큼 반복
		if (i == 9 || i == 18 || i == 27) { //9, 18, 27번째 그릴 때 다음 행으로 넘어가기
			num = 0; //새로운 행
			by -= 30; //30만큼 밑에 출력
		}
		//벽돌의 크기 x = 45, y = 25 (각각의 크기마다 +5픽셀하여 여백주기
		block[i].x = bx + (bw + 5) * num; //벽돌의 x좌표
		block[i].y = by; //벽돌의 y좌표

		block[i].collision = 0; //벽돌 충돌여부 초기화
		num++; //벽돌의 개수가 늘 때마다 늘어남
	}
}


//벽돌 그리기
void Draw_block() {
	for (int i = 0; i < block_cnt; i++) {
		//9의 나머지에 따라 벽돌 색상 변환
		if (i % 9 == 0) { //나머지가 0일 때 : 빨강 계열 색
			Bcolor.R = 0.8;
			Bcolor.G = 0.25;
			Bcolor.B = 0.25;
		}
		else if (i % 9 == 1) { //나머지가 1일 때 : 주황 계열 색
			Bcolor.R = 0.85;
			Bcolor.G = 0.55;
			Bcolor.B = 0.25;
		}
		else if (i % 9 == 2) { //나머지가 2일 때 : 노랑 계열 색
			Bcolor.R = 0.8;
			Bcolor.G = 0.77;
			Bcolor.B = 0.4;
		}
		else if (i % 9 == 3) { //나머지가 3일 때 : 연두 계열 색
			Bcolor.R = 0.7;
			Bcolor.G = 0.8;
			Bcolor.B = 0.25;
		}
		else if (i % 9 == 4) { //나머지가 4일 때 : 초록 계열 색
			Bcolor.R = 0.25;
			Bcolor.G = 0.6;
			Bcolor.B = 0.2;
		}
		else if (i % 9 == 5) { //나머지가 5일 때 : 하늘 계열 색
			Bcolor.R = 0.35;
			Bcolor.G = 0.8;
			Bcolor.B = 0.9;
		}
		else if (i % 9 == 6) { //나머지가 6일 때 : 남색 계열 색
			Bcolor.R = 0.15;
			Bcolor.G = 0.3;
			Bcolor.B = 0.7;
		}
		else if (i % 9 == 7) { //나머지가 7일 때 : 보라 계열 색
			Bcolor.R = 0.45;
			Bcolor.G = 0.3;
			Bcolor.B = 0.8;
		}
		else{ //나머지가 8일 때 : 분홍 계열 색
			Bcolor.R = 0.9;
			Bcolor.G = 0.45;
			Bcolor.B = 0.7;
		}
		glColor3f(Bcolor.R, Bcolor.G, Bcolor.B); //if문에 따라 벽돌 색상 선택
		//시계방향으로 좌표이동하면서 벽돌 그리기
		glBegin(GL_POLYGON);
		glVertex2f(block[i].x, block[i].y); //a : 좌상단
		glVertex2f(block[i].x + bw, block[i].y); //b : 우상단
		glVertex2f(block[i].x + bw, block[i].y - bh); //c : 우하단
		glVertex2f(block[i].x, block[i].y - bh); //d : 좌하단
		glEnd();
	}
}


//공과 벽돌 충돌
void Collide_Block2ball() {
	for (int i = 0; i < block_cnt; i++) {
		//점과 점 사이 거리
		float a = sqrt(pow(ball.x - block[i].x, 2) + pow(ball.y - block[i].y, 2)); //좌상단
		float b = sqrt(pow(ball.x - block[i].x + bw, 2) + pow(ball.y - block[i].y, 2));	//우상단
		float c = sqrt(pow(ball.x - block[i].x + bw, 2) + pow(ball.y - block[i].y - by, 2)); //우하단
		float d = sqrt(pow(ball.x - block[i].x, 2) + pow(ball.y - block[i].y - by, 2)); //좌하단

		//공과 벽돌의 충돌 판단

		if ((block[i].x <= ball.x + radius) && (ball.x - radius <= block[i].x + bw)) { //벽돌 면 충돌
			if((ball.y - radius <= block[i].y - bh) && (ball.y >= block[i].y - bh))
			{
				block[i].collision = 1; //충돌 처리
				block[i].x = -100; //벽돌 지우기
				block[i].y = -100;
				velocity.y *= -1.0;
				gamecnt++; //부순 벽돌 +1
			}
		}
		else {
			block[i].collision = 0;
		}
		if (a <= 0 || b <= 0 || c <= 0 || d <= 0) { //벽돌 모서리 충돌
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


//스틱 움직이기
void Moving_stick(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:		//왼쪽으로 움직이기
		stick_x -= stick_move;
		break;
	case GLUT_KEY_RIGHT:	//오른쪽으로 움직이기
		stick_x += stick_move;
		break;
	case GLUT_KEY_UP:		//위쪽으로 움직이기
		stick_y += stick_move;
		break;
	case GLUT_KEY_DOWN:		//아래쪽으로 움직이기
		stick_y -= stick_move;
		break;
	case GLUT_KEY_F1:		//f1 누르면 게임 재시작
		init();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}


//벽과 스틱 충돌
void Collide_stick2wall() {
	if (stick_x <= 200.0) {	//왼쪽 벽과 충돌
		stick_x = 200.0;
	}
	if (stick_x + stick_w >= 800.0) {	//오른쪽 벽과 충돌
		stick_x = 700.0;
	}
	if (stick_y >= height / 2) { //게임판의 중앙까지만 이동
		stick_y = height / 2;
	}
	if (stick_y < 0) {	//게임판 하단으로 벗어나지 않도록 설정
		stick_y = 10.0;
	}
}


//스틱과 공 충돌
void Collide_stick2ball() {
	if (stick_x <= ball.x) {	//스틱 좌측 비교
		if (ball.x <= stick_x + stick_w) {	//스틱 우측 비교
			if (stick_y + stick_h >= ball.y && stick_y <= ball.y) { //스틱의 y 좌표 비교
				velocity.y *= -1.0;
			}
		}
	}
}


//거리 계산
void Distance() {
	//좌하단
	float m1 = 0.0, n1 = 0.0, h1 = 0.0, theta1 = 0.0;
	m1 = (leftmid.y - leftlow.y) / (leftmid.x - leftlow.x);
	n1 = (ball.y - leftlow.y) / (ball.x - leftlow.x);
	theta1 = atan(m1) - atan(n1);
	h1 = sqrt(pow((ball.x - leftlow.x), 2) + pow((ball.y - leftlow.y), 2));
	Llowdist = h1 * sin(theta1); //좌하단 벽과의 거리

	//좌상단
	float m2 = 0.0, n2 = 0.0, h2 = 0.0, theta2 = 0.0;
	m2 = (lefthigh.y - leftmid.y) / (lefthigh.x - leftmid.x);
	n2 = (ball.y - leftmid.y) / (ball.x - leftmid.x);
	theta2 = atan(m2) - atan(n2);
	h2 = sqrt(pow((ball.x - leftmid.x), 2) + pow((ball.y - leftmid.y), 2));
	Lhighdist = h2 * sin(theta2); //좌상단 벽과의 거리

	//우하단
	float m3 = 0.0, n3 = 0.0, h3 = 0.0, theta3 = 0.0;
	m3 = (rightlow.y - rightmid.y) / (rightlow.x - rightmid.x);
	n3 = (ball.y - rightmid.y) / (ball.x - rightmid.x);
	theta3 = atan(m3) - atan(n3);
	h3 = sqrt(pow((ball.x - rightlow.x), 2) + pow((ball.y - rightlow.y), 2));
	Rlowdist = h3 * sin(theta3); //우하단 벽과의 거리
	
	//우상단
	float m4 = 0.0, n4 = 0.0, h4 = 0.0, theta4 = 0.0;
	m4 = (rightmid.y - righthigh.y) / (rightmid.x - righthigh.x);
	n4 = (ball.y - righthigh.y) / (ball.x - righthigh.x);
	theta4 = atan(m4) - atan(n4);
	h4 = sqrt(pow((ball.x - rightmid.x), 2) + pow((ball.y - rightmid.y), 2));
	Rhighdist = h4 * sin(theta4); //우상단 벽과의 거리
}


//공과 벽의 충돌 함수
void Collide_ball2wall(void) {
	//좌하단
	if (Llowdist <= 0 && (ball.x >= 200.0 && ball.x <= 400) && (ball.y >= 0.0 && ball.y <= 500.0)) {
		velocity.x *= -1.0;
		velocity.x = 0.1;
	}
	//좌상단
	if (Lhighdist <= 0 && (ball.x >= 200.0 && ball.x <= 400) && (ball.y >= 500.0 && ball.y <= 1000.0)) {
		velocity.x *= -1.0;
		velocity.x = 0.12;
	}
	//우하단
	if (Rlowdist <= 0 && (ball.x >= 600.0 && ball.x <= 800) && (ball.y >= 0.0 && ball.y <= 500.0)) {
		velocity.x *= -1.0;
		velocity.x = -0.1;
	}
	//우상단
	if (Rhighdist <= 0 && (ball.x >= 600.0 && ball.x <= 800) && (ball.y >= 500.0 && ball.y <= 1000.0)) {
		velocity.x *= -1.0;
		velocity.x = -0.12;
	}
	//좌
	if (ball.x - radius <= wleft && velocity.x < 0.0) {
		velocity.x *= -1.0;
	}
	//우
	if (ball.x + radius >= wright && velocity.x > 0.0) {
		velocity.x *= -1.0;
	}
	//상
	if (ball.y + radius >= wtop && velocity.y > 0.0) {
		velocity.y *= -1.0;
	}
	//하
	if (ball.y - radius <= wbottom && velocity.y < 0.0) {
		velocity.x = 0.0;
		velocity.y = 0.0;
	}
}


void GameOver() {
	if (gamecnt == block_cnt) { //생성된 벽돌을 다 부수면
		velocity.x = 0; //공 움직임 멈춤
		velocity.y = 0;
	}
}


void RenderScene(void) 
{
	glClearColor(0.2, 0.2, 0.2, 0.0); // Set display-window color to Yellow :윈도우창 바탕색
	glClear(GL_COLOR_BUFFER_BIT);

	Draw_sandglass();	//게임판 그리기
	Draw_stick();	//스틱 그리기
	Draw_block();	//벽돌 그리기

	Collide_Block2ball();	//벽돌과 공 충돌
	Collide_stick2wall();	//스틱과 벽 충돌
	Collide_stick2ball();	//스틱과 공 충돌
	Distance();				//벽과의 거리 계산
	Collide_ball2wall();	//공과 벽 충돌

	// 움직이는 공의 위치 변화 
	ball.x += velocity.x;
	ball.y += velocity.y;

	// 움직이는 공 그리기 
	glColor3f(0.0, 0.1, 0.7);
	Modeling_Circle(radius, ball);

	// 경과된 시간 출력
	int currentTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = (currentTime - startTime) / 1000;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glRasterPos2f(1.0, 0.0); // 비트맵 폰트의 위치

	glColor3f(1.0, 1.0, 1.0);  // 텍스트 색상 (흰색)
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
	startTime = glutGet(GLUT_ELAPSED_TIME); // 타이머 시작 시간 초기화
	glutTimerFunc(1000, Timer, 0); //타이머
	glutMainLoop();
}