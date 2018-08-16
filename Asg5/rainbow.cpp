// Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

struct {
    int width {384};
    int height {256};
} window;

const GLubyte RED[] {255, 0, 0};
const GLubyte ORANGE[] {255, 165, 0};
const GLubyte YELLOW[] {255, 255, 0};
const GLubyte GREEN[] {0, 255, 0};
const GLubyte BLUE[] {0, 0, 255};
const GLubyte INDIGO[] {75, 0, 130};
const GLubyte VIOLET[] {238, 130, 238};
const vector<const GLubyte*> colors {RED, ORANGE, YELLOW, GREEN,
    BLUE, INDIGO, VIOLET};
constexpr GLfloat aspect_ratio = 2.0 / 3.0;

void draw_rainbow_flag() {
    for (size_t i = 0; i < 7; ++i) {
        glBegin (GL_POLYGON);
        glColor3ubv (colors[i]);
        glVertex2f (window.width * i / 7.0, 0);
        glVertex2f (window.width * (i + 1) / 7.0, 0);
        glVertex2f (window.width * (i + 1) / 7.0, window.height);
        glVertex2f (window.width * i / 7.0, window.height);
        glEnd();
    }
}

void display() {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear (GL_COLOR_BUFFER_BIT);
    draw_rainbow_flag();
    glutSwapBuffers();
}

void reshape (int width, int height) {
    window.width = width;
    window.height = height;
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D (0, window.width, 0, window.height);
    glMatrixMode (GL_MODELVIEW);
    glViewport (0, 0, window.width, window.height);
    glutPostRedisplay();
}

int main (int argc, char** argv) {
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize (window.width, window.height);
    glutInitWindowPosition (256, 256);
    glutCreateWindow (basename (argv[0]));
    glutDisplayFunc (display);
    glutReshapeFunc (reshape);
    glutMainLoop();
    return 0;
}

