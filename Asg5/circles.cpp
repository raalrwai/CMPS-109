// Connor Thurmond (cthurmon) Rami Alrwais (raalrwai)

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

#include <GL/freeglut.h>
#include <libgen.h>

struct {
     int width {640};
     int height {480};
} window;

template <typename item_t>
struct cycle_iterator {
    const vector<item_t> items;
    size_t curr_item {0};
    cycle_iterator (initializer_list<item_t> list):
                    items(vector<item_t> (list)) {}
    const item_t& operator*() { return items[curr_item]; }
    const item_t& operator[] (size_t index) {
        return items[(curr_item + index) % items.size()];
    }
    cycle_iterator& operator++() { 
        curr_item = (curr_item + 1) % items.size();
        return *this;
    }
};

const GLubyte GREY[] {0xD3, 0xD3, 0xD3};
GLubyte RED[] {0xFF, 0x00, 0x00};
GLubyte BLUE[] {0x00, 0x00, 0xFF};
GLubyte GREEN[] {0x00, 0xFF, 0x00};
cycle_iterator<GLubyte*> colors {BLUE, GREEN, RED};
constexpr GLfloat aspect_ratio = 2.0 / 3.0;

void draw_circles() {
    glBegin (GL_POLYGON);
    glColor3ubv (GREY);
    glVertex2f (0, 0);
    glVertex2f (window.width, 0);
    glVertex2f (window.width, window.height);
    glVertex2f (0, window.height);
    glEnd();
    glBegin (GL_POLYGON);
    glColor3ubv (colors[0]);
    const GLfloat delta = 2 * M_PI / 64;
    const GLfloat radius = 
        fmin(window.height, window.width) * 4.5 / 10.0;
    const GLfloat xoffset = window.width / 2.0;
    const GLfloat yoffset = window.height / 2.0;
    for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
        GLfloat xpos = radius * cos (theta) + xoffset;
        GLfloat ypos = radius * sin (theta) + yoffset;
        glVertex2f (xpos, ypos);
    }
    glEnd();
    glBegin (GL_POLYGON);
    glColor3ubv (colors[1]);
    const GLfloat radius2 = radius * 2.0 / 3.0;
    for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
        GLfloat xpos = radius2 * cos (theta) + xoffset;
        GLfloat ypos = radius2 * sin (theta) + yoffset;
        glVertex2f (xpos, ypos);
    }
    glEnd();
    glBegin (GL_POLYGON);
    glColor3ubv (colors[2]);
    const GLfloat radius3 = radius * 1.0 / 3.0;
    for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
        GLfloat xpos = radius3 * cos (theta) + xoffset;
        GLfloat ypos = radius3 * sin (theta) + yoffset;
        glVertex2f (xpos, ypos);
    }
    glEnd();
}

void display() {
    glClearColor (0.0, 0.0, 0.0, 0.0);
    glClear (GL_COLOR_BUFFER_BIT);
    draw_circles();
    glutSwapBuffers();
}

void invert_colors() {
    for (size_t color = 0; color < 3; ++color) {
        for (size_t rgb = 0; rgb < 3; ++rgb) {
            colors[color][rgb] ^= 0xFF;
        }
    }
}

void keyboard (GLubyte key, int, int) {
    enum {BS = 8, LF = 10, CR = 13, ESC = 27, DEL = 127};
    switch (key) {
        case ' ': case BS: case CR: case LF: case DEL:
            invert_colors();
            break;
        case 'c': case 'C':
            ++colors;
            break;
        case 's': case 'S':
            invert_colors();
            break;
        case 'q': case 'Q': case ESC:
            exit (EXIT_SUCCESS);
    }
    glutPostRedisplay();
}

void mouse (int button, int state, int, int) {
    if (state == GLUT_DOWN) {
        switch (button) {
            case GLUT_LEFT_BUTTON:
                ++colors;
                break;
            case GLUT_MIDDLE_BUTTON:
                ++colors;
                break;
            case GLUT_RIGHT_BUTTON:
                ++colors;
                break;
        }
    }
    glutPostRedisplay();
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
    glutMouseFunc (mouse);
    glutMainLoop();
    return 0;
}
