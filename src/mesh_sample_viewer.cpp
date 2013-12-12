#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cfloat>
#include <cmath>

#include <vector>
#include <string>

#include "mesh_sampler_utils.h"

using std::vector;
using std::string;

#define PI 3.14159

int WindowWidth = 640;
int WindowHeight = 640;

int MouseX = 0;
int MouseY = 0;
bool MouseLeft = false;
bool MouseRight = false;

MeshSamples g_mesh_samples;
Camera g_camera;

void DisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0, 1.0, .01, 10000.0);

	gluLookAt(g_camera.radius*cos(g_camera.theta)*sin(g_camera.phi),
			  g_camera.radius*cos(g_camera.phi),
			  g_camera.radius*sin(g_camera.theta)*sin(g_camera.phi),
			  0.0, 0.0, 0.0,
			  0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPointSize(4.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS); {
		for(vector<Point>::const_iterator sample = g_mesh_samples.samples.begin();
			sample != g_mesh_samples.samples.end();
			sample++)
		{
			glVertex3f(sample->x, sample->y, sample->z);
		}

	} glEnd();

	glutSwapBuffers();
}

void ReshapeFunc(int x, int y)
{
    glViewport(0,0,x,y);
    WindowWidth = x;
    WindowHeight = y;
}

void MouseFunc(int button, int state, int x, int y)
{
	MouseX = x;
	MouseY = y;

	if(button == GLUT_LEFT_BUTTON)
		MouseLeft = !(bool) state;
	if(button == GLUT_RIGHT_BUTTON)
		MouseRight = !(bool) state;
}

void MotionFunc(int x, int y)
{

	if(MouseLeft)
	{
		g_camera.theta += 0.01*PI*(MouseX - x);
		g_camera.phi += 0.01*PI*(MouseY - y);
		if(g_camera.phi > (PI - 0.01))
			g_camera.phi = PI - 0.01;
		if(g_camera.phi < 0.01)
			g_camera.phi = 0.01;
	}

	if(MouseRight)
	{
		g_camera.radius += 0.2*(MouseY - y);
		if(g_camera.radius <= 0)
			g_camera.radius = 0.2;
	}

	MouseX = x;
	MouseY = y;

	glutPostRedisplay();
}

void KeyboardFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'Q':
	case 'q':
	case 27:
		exit(0);
		break;
	case '-':
		g_camera.radius *= 1.1;
		break;
	case '+':
		g_camera.radius *= .9;
		break;
	}

	glutPostRedisplay();
}

int main(int argc, char** argv)
{

	if(argc <= 1)
	{
		fprintf(stderr, "You need to supply a filename\n");

		return 1;
	}

	readMeshSamplesFile(argv[1], g_mesh_samples);

	g_camera.radius = g_mesh_samples.z_max+10;
	g_camera.phi = 1.57;
	g_camera.theta = 1.57;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("Voronoi and Delaunay");

	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MotionFunc);
	glutKeyboardFunc(KeyboardFunc);

	glutMainLoop();

	return 0;
}
