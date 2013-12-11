#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cfloat>
#include <cmath>

#include <vector>
#include <string>

using std::vector;
using std::string;

#define PI 3.14159

int WindowWidth = 640;
int WindowHeight = 640;

int MouseX = 0;
int MouseY = 0;
bool MouseLeft = false;
bool MouseRight = false;


typedef struct Point_
{
	float x, y, z;
} Point;

typedef struct MeshSamples_
{
	std::vector<Point> samples;
	float x_min, x_max;
	float y_min, y_max;
	float z_min, z_max;
} MeshSamples;

typedef struct Camera_
{
	float radius;
	float phi;
	float theta;
} Camera;

MeshSamples g_mesh_samples;
Camera g_camera;

void readMeshSamplesFile(std::string filename, MeshSamples& data)
{
	FILE* mesh_sample_file;
	mesh_sample_file = fopen(filename.c_str(), "r");

	if(!mesh_sample_file)
	{
		fprintf(stderr, "Failed to open file %s", filename.c_str());
		fclose(mesh_sample_file);
		exit(-1);
	}

	data.samples.clear();
	data.x_min = data.y_min = data.z_min = FLT_MAX;
	data.x_max = data.y_max = data.z_max = -FLT_MAX;

	Point p;
	while(fscanf(mesh_sample_file, "%f %f %f", &p.x, &p.y, &p.z) != EOF)
	{
		data.samples.push_back(p);
	}

	if(p.x < data.x_min)
		data.x_min = p.x;
	if(p.x > data.x_max)
		data.x_max = p.x;

	if(p.y < data.y_min)
		data.y_min = p.y;
	if(p.y > data.y_max)
		data.y_max = p.y;

	if(p.z < data.z_min)
		data.z_min = p.z;
	if(p.z > data.z_max)
		data.z_max = p.z;

	float x_trans = -(data.x_min + ( data.x_max - data.x_min)/2.0f);
	float y_trans = -(data.y_min + ( data.y_max - data.y_min)/2.0f);
	float z_trans = -(data.z_min + ( data.z_max - data.z_min)/2.0f);

	for(vector<Point>::iterator sample = data.samples.begin();
		sample != data.samples.end();
		sample++)
	{
		sample->x += x_trans;
		sample->y += y_trans;
		sample->z += z_trans;
	}

	fclose(mesh_sample_file);
}

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
	g_camera.radius = 1000.0f;
	g_camera.phi = 1.57;
	g_camera.theta = 1.57;

	if(argc <= 1)
	{
		fprintf(stderr, "You need to supply a filename\n");

		return 1;
	}

	readMeshSamplesFile(argv[1], g_mesh_samples);

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
