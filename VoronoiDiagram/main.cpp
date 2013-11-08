#include "GL/glut.h"
#include "voronoi_diagram.h"
#include <cstdlib>
#include <iostream>

using voronoi_diagram::Sites;
using voronoi_diagram::SitesPtr;
using voronoi_diagram::SitePtr;
using voronoi_diagram::Site;
using voronoi_diagram::VoronoiDiagram;

int WindowWidth = 320;
int WindowHeight = 320;

std::unique_ptr<VoronoiDiagram> diagram;


SitesPtr generateSites(int num_sites, float diagram_width, float diagram_height, int seed=0)
{
	srand(seed);

	SitesPtr sites(new Sites());
	for(int i=0; i<num_sites; i++)
	{
		SitePtr site(new Site(rand()%(int)diagram_width - diagram_width/2.0f, rand()%(int)diagram_height - diagram_height/2.0f));
		sites->push_back(site);
	}

	return sites;
}

void DisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutSwapBuffers();
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
	}
}

void ReshapeFunc(int x,int y)
{
    glViewport(0,0,x,y);
    WindowWidth = x;
    WindowHeight = y;
}

int main(int argc, char** argv)
{
	SitesPtr sites = generateSites(10, 2, 2);

	diagram.reset(new VoronoiDiagram(sites));

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