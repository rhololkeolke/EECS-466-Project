#include "GL/glut.h"
#include "animated_voronoi_diagram.h"
#include <functional>
#include <cstdlib>
#include <iostream>

using voronoi_diagram::Sites;
using voronoi_diagram::SitesPtr;
using voronoi_diagram::SitesConstPtr;
using voronoi_diagram::SitePtr;
using voronoi_diagram::Site;
using voronoi_diagram::AnimatedVoronoiDiagram;

int WindowWidth = 320;
int WindowHeight = 320;

float g_diagram_width = 200.0f, g_diagram_height = 200.0f;

std::unique_ptr<AnimatedVoronoiDiagram> diagram;

voronoi_diagram::EdgesPtr edges;

SitesPtr generateSites(int num_sites, float diagram_width, float diagram_height, int seed=1)
{
	srand(seed);

	SitesPtr sites(new Sites());
	for(int i=0; i<num_sites; i++)
	{
		SitePtr site(new Site((rand()%(int)diagram_width) - diagram_width/2.0f, (rand()%(int)diagram_height) - diagram_height/2.0f));
		sites->push_back(site);
	}

	return sites;
}

void DisplayFunc(void)
{
	diagram->display();
}

void MouseFunc(int button,int state,int x,int y)
{

}

void MotionFunc(int x, int y)
{

}

void KeyboardFunc(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'Q':
	case 'q':
		exit(0);
		break;
	case 'N':
	case 'n':
		printf("next event\n");
		diagram->nextEvent();
		break;
	case 'R':
	case 'r':
		printf("restarting animation\n");
		diagram->restartAnimation();
		break;
	}
	glutPostRedisplay();
}

void ReshapeFunc(int x,int y)
{
    glViewport(0,0,x,y);
    WindowWidth = x;
    WindowHeight = y;
}

int main(int argc, char** argv)
{
	SitesPtr sites = generateSites(10, g_diagram_width, g_diagram_height);

	diagram.reset(new AnimatedVoronoiDiagram(sites, g_diagram_width, g_diagram_height));

	//edges = diagram->getEdges();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("Voronoi Diagram");

	glutDisplayFunc(DisplayFunc);
	glutReshapeFunc(ReshapeFunc);
	glutMouseFunc(MouseFunc);
    glutMotionFunc(MotionFunc);
    glutKeyboardFunc(KeyboardFunc);

	glutMainLoop();

	return 0;
}