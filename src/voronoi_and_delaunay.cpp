#include <GL/glut.h>
#include <voronoi_diagram.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>

using namespace voronoi_diagram;

bool g_show_delaunay = true;
bool g_show_voronoi = true;

int WindowWidth = 640;
int WindowHeight = 640;

std::unique_ptr<VoronoiDiagram> diagram;

voronoi_diagram::EdgesPtr edges;

SitesPtr generateSites(int num_sites, float diagram_width, float diagram_height, int seed=1)
{
	srand(seed);

	SitesPtr sites(new Sites());
	for(int i=0; i<num_sites; i++)
	{
		SitePtr site(new Site((rand()%(int)diagram_width) - diagram_width/2.0f, (rand() % (int)diagram_height) - diagram_height/2.0f));
		sites->push_back(site);
	}

	return sites;
}

void DisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float diagram_width = diagram->getWidth();
	float diagram_height = diagram->getHeight();
	gluOrtho2D(-diagram_width/2.0f, diagram_width/2.0f, -diagram_height/2.0f, diagram_height/2.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// draw the sites
	glPointSize(4.0f);
	glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS); {
		SitesConstPtr sites = diagram->getSites();
		for(Sites::const_iterator site = sites->begin();
			site != sites->end();
			site++)
		{
			glVertex2f((*site)->x, (*site)->y);
		}
	} glEnd();

	if(g_show_voronoi)
	{
		// draw the edges
		glColor3f(1.0f, 0.0f, 0.0f);
		glBegin(GL_LINES); {
			for(Edges::iterator edge = edges->begin();
				edge != edges->end();
				edge++)
			{
				glVertex2f((*edge)->start_->x, (*edge)->start_->y);
				glVertex2f((*edge)->end_->x, (*edge)->end_->y);
			}
		} glEnd();
	}

	if(g_show_delaunay)
	{
		glColor3f(0.0f, 0.0f, 1.0f);
		glBegin(GL_LINES); {
			for(Edges::iterator edge = edges->begin();
				edge != edges->end();
				edge++)
			{
				glVertex2f((*edge)->left_->x, (*edge)->left_->y);
				glVertex2f((*edge)->right_->x, (*edge)->right_->y);
			}
		} glEnd();
	}

	glutSwapBuffers();
}

void MouseFunc(int button, int state, int x, int y)
{

}

void MotionFunc(int x, int y)
{

}

void KeyboardFunc(unsigned char key, int x, int y)
{
	SitesPtr sites;
	float diagram_width = diagram->getWidth();
	float diagram_height = diagram->getHeight();
	switch(key)
	{
	case 'Q':
	case 'q':
	case 27:
		exit(0);
		break;
		/*	case 'T':
	case 't':
		printf("generating new sites\n");
		sites = generateSites(30, diagram_width, diagram_height, time(NULL));
		diagram.reset(new VoronoiDiagram(sites, diagram_width, diagram_height));
		edges = diagram->getEdges();
		break;*/
	case '+':
		diagram->setDimensions(diagram_width*.9, diagram_height*.9);
		edges = diagram->getEdges();
		printf("%3.3f x %3.3f\n", diagram_width, diagram_height);
		break;
	case '-':
		diagram->setDimensions(diagram_width*1.1, diagram_height*1.1);
		edges = diagram->getEdges();
		printf("%3.3f x %3.3f\n", diagram_width, diagram_height);
		break;
	case 'D':
	case 'd':
		g_show_delaunay = !g_show_delaunay;
		if(g_show_delaunay)
			printf("Showing delaunay triangulation\n");
		else
			printf("Not showing delaunay triangulation\n");
		break;
	case 'V':
	case 'v':
		g_show_voronoi = !g_show_voronoi;
		if(g_show_voronoi)
			printf("Showing voronoi diagram\n");
		else
			printf("Not showing voronoi diagram\n");
		break;
	}

	glutPostRedisplay();
}

void ReshapeFunc(int x, int y)
{
    glViewport(0,0,x,y);
    WindowWidth = x;
    WindowHeight = y;
}

int main(int argc, char** argv)
{
	float init_diagram_width = 200.0f;
	float init_diagram_height = 200.0f;
	SitesPtr sites = generateSites(30, init_diagram_width, init_diagram_height);

	diagram.reset(new VoronoiDiagram(sites, init_diagram_width, init_diagram_height));

	edges = diagram->getEdges();

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
