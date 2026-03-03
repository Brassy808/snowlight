#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <glut.h>

GLint Width = 640, Height = 300;
const short DELAY = 1000; // Значение секунды для отсчета. Можно меньшить для ускорения работы светофора
long time0 = 0;
int R = Height / 10, dist_btw_cir = R-20; 
int red_x = Width / 2, yellow_x = Width / 2, green_x = Width / 2; // Рассчет позиций x для фонарей
int red_y = Height / 2 + 2 * R + dist_btw_cir, yellow_y = Height / 2, green_y = Height / 2 - 2 * R - dist_btw_cir; // Рассчет позиций y для фонарей
bool paint = true;
const short RED_DELAY = 10; // Не использовать значения < 4
const short GREEN_DELAY = 10; // Не использовать значения < 4
int timer = RED_DELAY;
bool is_red = true;
bool is_yellow = (timer == 3);
bool is_green = false;

void drawCircle(int x, int y, int r, int amountSegments){
	glBegin(GL_POLYGON);				// Окружность - полигон ВРУЧНУЮ
	for(int i = 0; i < amountSegments; i++){
		float angle = 2.0 * 3.1415926 * float(i) / float(amountSegments);
		float dx = r * cosf(angle);
		float dy = r * sinf(angle);
		glVertex2f(x + dx, y + dy);
		}
	glEnd(); 
	}

void Display(void){
	// Закраска фона 
	glBegin(GL_POLYGON);
	glColor3ub(0, 0, 0);
	glVertex2f(0, 0);
	glVertex2f(0, Height);
	glVertex2f(Width, Height);
	glVertex2f(Width, 0);
	glEnd();

	// Отрисовка фонарей
	if (is_red) {
		glColor3ub(255, 0, 0);
		drawCircle(red_x, red_y, R, 50);
	}
	else {
		glColor3ub(50, 50, 50);
		drawCircle(red_x, red_y, R, 50);
	}

	if (is_yellow) {
		glColor3ub(255, 255, 0);
		drawCircle(yellow_x, yellow_y, R, 50);
	}
	else {
		glColor3ub(50, 50, 50);
		drawCircle(yellow_x, yellow_y, R, 50);
	}

	if (is_green) {
		glColor3ub(0, 255, 0);
		drawCircle(green_x, green_y, R, 50);
	}
	else {
		glColor3ub(50, 50, 50);
		drawCircle(green_x, green_y, R, 50);
	}

	glFlush();
	glFinish();		
	}

void Reshape(GLint w, GLint h){
    Width = w;
    Height = h;
	// Перерассчет позиций и радиуса
	R = Height / 10;
	dist_btw_cir = R - 20;
	red_x = Width / 2, yellow_x = Width / 2, green_x = Width / 2;
	red_y = Height / 2 + 2 * R + dist_btw_cir, yellow_y = Height / 2, green_y = Height / 2 - 2 * R - dist_btw_cir;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	}

void Idle(){	// Задержка и ДИНАМИКА
	if (clock()-time0 < DELAY)
		return;
	time0=clock();
	timer -= 1;

	// Обработка переключения фонарей на основе предыдущей позиции перед переключением
	if (timer == 3 or timer == 0) {
		paint = true;
		if (is_red) {
			if (timer == 3) 
				is_yellow = true;
			else {
				timer = GREEN_DELAY;
				is_red = false;
				is_yellow = false;
				is_green = true;
			}
		}
		else if (timer == 3) {
			is_red = false;
			is_yellow = true;
			is_green = false;
		}
		else {
			timer = RED_DELAY;
			is_red = true;
			is_yellow = false;
			is_green = false;
		}
	}
	if (paint){
		glutPostRedisplay();		// Инициализировать перерисовку
		paint = false;
		}
	}


/* Главный цикл приложения */
int main(int argc, char *argv[]){
	if (RED_DELAY <= 3 or GREEN_DELAY <= 3) // Защита от дурака на константах
		return 0;
	time0 = clock();
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(Width, Height);
    glutCreateWindow("Светофор");
    glutDisplayFunc(Display);
    glutReshapeFunc(Reshape);
	glutIdleFunc(Idle);
    glutMainLoop();
	}