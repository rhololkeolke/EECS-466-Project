#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Projection_traits_xy_3.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <GL/glut.h>

#include <vector>
#include <iostream>

#include <cstdlib>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Projection_traits_xy_3<K> Gt;
typedef CGAL::Delaunay_triangulation_2<Gt> Delaunay;
typedef K::Point_3 Point;

std::vector<Point> input;
Delaunay dt;

float g_diagram_width = 200.0f, g_diagram_length = 200.0f, g_diagram_height = 200.0f;

int g_seed = 0;
const int NUM_POINTS = 100;

int WindowWidth=640, WindowHeight=640;

void generatePoints(int num_points, std::vector<Point>& input, int seed=1)
{
	srand(seed);

	input.clear();
	
	for(int i=0; i<num_points; i++)
	{
		Point new_point((rand()%(int)g_diagram_width) - g_diagram_width/2.0f, (rand()%(int)g_diagram_length) - g_diagram_length/2.0f, (rand()%(int)g_diagram_height) - g_diagram_height/2.0f);
		input.push_back(new_point);
	}
}

void DisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-g_diagram_width/2.0f, g_diagram_width/2.0f, -g_diagram_length/2.0f, g_diagram_length/2.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPointSize(4.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS); {
		for(std::vector<Point>::const_iterator point = input.begin();
			point != input.end();
			point++)
		{
			glVertex2f((float)point->x(), (float)point->y());//, (float)point->z());
		}
	} glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLES); {
		for(Delaunay::Finite_faces_iterator face = dt.finite_faces_begin();
			face != dt.finite_faces_end();
			face++)
		{
			for(int i=0; i<3; i++)
			{
				glVertex2f((float)face->vertex(i)->point().x(), (float)face->vertex(i)->point().y()); //, (float)v.point().z());
			}
		}
	} glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES); {
		
	} glEnd();

	glutSwapBuffers();
}

void KeyboardFunc(unsigned char key, int x, int y)
{
	Delaunay new_dt;
	switch(key)
	{
	case 'Q':
	case 'q':
		exit(0);
		break;
	case 'T':
	case 't':
		generatePoints(NUM_POINTS, input, ++g_seed);
		printf("seed %d\n", g_seed);
		for(std::vector<Point>::const_iterator point = input.begin();
			point != input.end();
			point++)
		{
			new_dt.insert(*point);
		}
		dt = new_dt;
		break;
	}

	glutPostRedisplay();
}

void ReshapeFunc(int x, int y)
{
	glViewport(0, 0, x, y);
	WindowWidth = x;
	WindowHeight = y;
}

int main(int argc, char** argv)
{
	generatePoints(NUM_POINTS, input, ++g_seed);
	
	for(std::vector<Point>::const_iterator point = input.begin();
		point != input.end();
		point++)
	{
		dt.insert(*point);
	}
	
	std::cout << dt.number_of_vertices() << std::endl;

	int nl = 0;
	int nr = 0;
	for(Delaunay::Edge_iterator edge = dt.edges_begin();
		edge != dt.edges_end();
		edge++)
	{
		//CGAL::Object o = dt.dual(edge);
		//if(CGAL::object_cast<Delaunay::Segment>(&o))
		//	nl++;
		//else if(CGAL::object_cast<Delaunay::Ray>(&o))
		//	nr++;
	}

	std::cout << "The Voronoi diagram has " << nl << " line segments and " << nr << "rays." << std::endl;


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("CGAL Delaunay Triangulation");

	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutKeyboardFunc(KeyboardFunc);

	glutMainLoop();
	
	return 0;
}
