#include <freeglut.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAX_SNOW 200
#define GROUND_CHUNKS 200

typedef struct {
    float x, y;
    float speed;
} Snowflake;

Snowflake snowflakes[MAX_SNOW];
float snow_height[GROUND_CHUNKS];
int width = 800, height = 600;
float base_ground = 20.0f;

int getChunk(float x) {
    int chunk = (int)(x / width * GROUND_CHUNKS);
    if (chunk < 0) chunk = 0;
    if (chunk >= GROUND_CHUNKS) chunk = GROUND_CHUNKS - 1;
    return chunk;
}

float getSnowHeight(float x) {
    int chunk = getChunk(x);
    return base_ground + snow_height[chunk];
}

void init() {
    srand(time(NULL));
    for (int i = 0; i < GROUND_CHUNKS; i++) {
        snow_height[i] = 0.0f;
    }
    for (int i = 0; i < MAX_SNOW; i++) {
        snowflakes[i].x = (float)(rand() % width);
        snowflakes[i].y = (float)(rand() % height + height);
        snowflakes[i].speed = 0.15f + (float)(rand() % 30) / 10.0f;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(0.9f, 0.95f, 1.0f);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= GROUND_CHUNKS; i++) {
        float x = (float)i / GROUND_CHUNKS * width;
        float h = (i < GROUND_CHUNKS) ? getSnowHeight(x) : base_ground;
        glVertex2f(x, 0);
        glVertex2f(x, h);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < MAX_SNOW; i++) {
        glVertex2f(snowflakes[i].x, snowflakes[i].y);

        snowflakes[i].y -= snowflakes[i].speed;

        float current_ground = getSnowHeight(snowflakes[i].x);

        if (snowflakes[i].y <= current_ground) {

            int chunk = getChunk(snowflakes[i].x);
            if (chunk >= 0 && chunk < GROUND_CHUNKS) {
                snow_height[chunk] += 0.4f;
            }

            snowflakes[i].y = height + (float)(rand() % 50);
            snowflakes[i].x = (float)(rand() % width);
            snowflakes[i].speed = 0.5f + (float)(rand() % 30) / 10.0f;
        }
    }
    glEnd();

    glutSwapBuffers();
    glutPostRedisplay();
}

void reshape(int w, int h) {
    width = w;
    height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Снегопад с сугробами");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    init();
    glutMainLoop();
    return 0;
}
