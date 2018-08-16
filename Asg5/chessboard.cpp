// Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)

#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

struct {
    int width {640};
    int height {480};
} window;

const GLubyte GREY[] {0xD3, 0xD3, 0xD3};
const GLubyte BLACK[] {0, 0, 0};
const GLubyte WHITE[] {255, 255, 255};
const GLubyte BLUE[] {0, 0, 255};
const GLubyte CYAN[] {0, 255, 255};
const GLubyte GREEN[] {0, 255, 0};
const GLubyte RED[] {255, 0, 0};

int color_scheme = 0;

void keyboard (GLubyte key, int, int) {
    enum {BS = 8, LF = 10, CR = 13, ESC = 27, DEL = 127};
    switch (key) {
        case 'b': case 'B':
            color_scheme = 1;
            break;
        case 'g': case 'G':
            color_scheme = 2;
            break;    
        case 'k': case 'K':
            color_scheme = 0;
            break;
        case 'q': case 'Q': case ESC:
            exit (EXIT_SUCCESS);
    }
    glutPostRedisplay();
}

void draw_chessboard() {
    glBegin (GL_POLYGON);
    glColor3ubv (GREY);
    glVertex2f (0, 0);
    glVertex2f (window.width, 0);
    glVertex2f (window.width, window.height);
    glVertex2f (0, window.height);
    glEnd();
    for (int r = 0; r < 8; ++r) {
        for (size_t i = 0; i < 8; ++i) {
            glBegin (GL_POLYGON);
            if (i % 2 == 0) {
                if (r % 2 == 0) {
                    switch (color_scheme) {
                        case 0:
                            glColor3ubv (BLACK);
                            break;
                        case 1:
                            glColor3ubv (BLUE);
                            break;
                        case 2:
                            glColor3ubv (GREEN);
                            break;
                    }
                } else {
                    switch (color_scheme) {
                        case 0:
                            glColor3ubv (WHITE);
                            break;
                        case 1:
                            glColor3ubv (CYAN);
                            break;
                        case 2:
                            glColor3ubv (RED);
                            break;
                    }
                }
            } else {
                if (r % 2 == 0) {
                    switch (color_scheme) {
                        case 0:
                            glColor3ubv (WHITE);
                            break;
                        case 1:
                            glColor3ubv (CYAN);
                            break;
                        case 2:
                            glColor3ubv (RED);
                            break;
                    }
                } else {
                    switch (color_scheme) {
                        case 0:
                            glColor3ubv (BLACK);
                            break;
                        case 1:
                            glColor3ubv (BLUE);
                            break;
                        case 2:
                            glColor3ubv (GREEN);
                            break;
                    }
                }
            }
            double xoffset = 0.0;
            double yoffset = 0.0;
            if (window.width < window.height) {
                xoffset = window.width * 1.0 / 16.0;
                yoffset = (window.height - (window.width * 7.0 / 8.0)) 
                    / 2.0;
            } else {
                yoffset = window.height * 1.0 / 16.0;
                xoffset = (window.width - (window.height * 7.0 / 8.0)) 
                    / 2.0;
            }
            double side_size = (fmin(window.width, window.height) * 7.0 
                / 8.0);
            glVertex2f ((side_size * i / 8.0) + xoffset, 
                (side_size * r / 8.0) + yoffset);
            glVertex2f ((side_size * (i + 1) / 8.0) + xoffset, 
                (side_size * r / 8.0) + yoffset);
            glVertex2f ((side_size * (i + 1) / 8.0) + xoffset, 
                (side_size * (r + 1) / 8.0) + yoffset);
            glVertex2f ((side_size * i / 8.0) + xoffset, 
                (side_size * (r + 1)/ 8.0) + yoffset);
            glEnd();
        }
    }
}

void display() {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear (GL_COLOR_BUFFER_BIT);
    draw_chessboard();
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
    glutKeyboardFunc (keyboard);
    glutMainLoop();
    return 0;
}
