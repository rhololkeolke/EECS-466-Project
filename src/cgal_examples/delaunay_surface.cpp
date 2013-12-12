#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <GL/glut.h>

#include <vector>
#include <iostream>

#include <cstdlib>
#include <cfloat>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<float, K>    Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Delaunay;
typedef K::Point_2 Point;

typedef std::vector<std::pair<Point, float> > Points;

Points input;
Delaunay dt;

float g_diagram_width = 200.0f, g_diagram_length = 200.0f, g_diagram_height = 200.0f;

int g_seed = 0;
const int NUM_POINTS = 100;

int WindowWidth=640, WindowHeight=640;

bool g_show_voronoi = true;
bool g_show_delaunay = true;

void generatePoints(int num_points, Points& input, int seed=1)
{
	srand(seed);

	input.clear();
	
	for(int i=0; i<num_points; i++)
	{
		input.push_back(std::make_pair(Point((rand()%(int)g_diagram_width) - g_diagram_width/2.0f, (rand()%(int)g_diagram_length) - g_diagram_length/2.0f), (rand()%(int)g_diagram_height) - g_diagram_height/2.0f));
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
		for(Points::const_iterator point = input.begin();
			point != input.end();
			point++)
		{
			glVertex2f((float)point->first.x(), (float)point->first.y());
		}
	} glEnd();

	if(g_show_delaunay)
	{
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
	}

	if(g_show_voronoi)
	{
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES); {
			for(Delaunay::Edge_iterator edge = dt.edges_begin();
				edge != dt.edges_end();
				edge++)
			{
				CGAL::Object o = dt.dual(edge);

				const K::Segment_2* segment = CGAL::object_cast<K::Segment_2>(&o);
				if(segment)
				{
					glVertex2f((float)segment->source().x(), (float)segment->source().y());
					glVertex2f((float)segment->target().x(), (float)segment->target().y());
				}
				else
				{
					const K::Ray_2* ray = CGAL::object_cast<K::Ray_2>(&o);
					if(ray)
					{
						glVertex2f((float)ray->source().x(), (float)ray->source().y());

						// instead of calculating the exact point where the ray intersects the
						// appropriate clipping plane just set the second vertex as a point
						// really far away
						float x = ray->source().x() + 1000.0f*ray->direction().dx();
						float y = ray->source().y() + 1000.0f*ray->direction().dy();
						glVertex2f(x, y);
					}
				}
			}
		} glEnd();
	}

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
		new_dt.insert(input.begin(), input.end());
		dt = new_dt;
		break;
	case 'V':
	case 'v':
		g_show_voronoi = !g_show_voronoi;
		break;
	case 'D':
	case 'd':
		g_show_delaunay = !g_show_delaunay;
		break;
	case '+':
		break;
	case '-':
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

	dt.insert(input.begin(), input.end());
	
	std::cout << dt.number_of_vertices() << std::endl;

	int nl = 0;
	int nr = 0;
	for(Delaunay::Edge_iterator edge = dt.edges_begin();
		edge != dt.edges_end();
		edge++)
	{
		CGAL::Object o = dt.dual(edge);
		if(CGAL::object_cast<K::Segment_2>(&o))
			nl++;
		else if(CGAL::object_cast<K::Ray_2>(&o))
			nr++;
	}

	std::cout << "The Voronoi diagram has " << nl << " line segments and " << nr << " rays." << std::endl;


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
