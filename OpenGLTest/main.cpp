#include <iostream>
#include <glm/glm.hpp>
#include <GL/freeglut.h>
using namespace std;
static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;


using vec3 = glm::vec3;



void int_opengl() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void draw_triangle() {
	glColor3ub(255, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5f, 0, 0);
	glVertex3f(0.5f, 0, 0);
	glVertex3f(0, 0.5f, 0);
	glEnd();
}

void display_func() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_triangle();
	glutSwapBuffers();
}

int main(int argc, char *argv[]) {
	//GLUT wWindow initialization
	glutInit(&argc, argv);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutCreateWindow("Hello, FreeGlut!");

	//OpenGL initialization
	int_opengl();

	//GLUT callbacks
	glutDisplayFunc(display_func);

	glutMainLoop();

	return 0;
}