#include "GL/glut.h"
#include "animated_voronoi_diagram.h"
#include <functional>
#include <cstdlib>
#include <iostream>
#include <cfloat>

using voronoi_diagram::Sites;
using voronoi_diagram::SitesPtr;
using voronoi_diagram::SitesConstPtr;
using voronoi_diagram::SitePtr;
using voronoi_diagram::Site;
using voronoi_diagram::AnimatedVoronoiDiagram;

int WindowWidth = 640;
int WindowHeight = 640;

float g_diagram_width = 200.0f, g_diagram_height = 200.0f;

std::unique_ptr<AnimatedVoronoiDiagram> diagram;

voronoi_diagram::EdgesPtr edges;

int iteration = 0;

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

SitesPtr readXYZ(std::string filename, float* width, float* height, int line_skip=1)
{
	FILE* xyz_file;
	xyz_file = fopen(filename.c_str(), "r");

	if(!xyz_file)
	{
		fprintf(stderr, "Failed to open file %s", filename.c_str());
		fclose(xyz_file);
		exit(-1);
	}

	SitesPtr sites(new Sites());

	float x_min, x_max;
	float y_min, y_max;

	x_min = y_min = FLT_MAX;
	x_max = y_max = -FLT_MAX;
	
	float x, y, z;
	int line = 0;
	while(fscanf(xyz_file, "%f %f %f", &x, &y, &z) != EOF)
	{
		if((line++) % 100000 != 0)
			continue;

		SitePtr site(new Site(x, y));
		sites->push_back(site);

		if(x < x_min)
			x_min = x;
		if(x > x_max)
			x_max = x;
		if(y < y_min)
			y_min = y;
		if(y > y_max)
			y_max = y;
	}
	fclose(xyz_file);

	// translate the x and y coordinates
	float x_translation = -(x_min + (x_max - x_min)/2.0f);
	float y_translation = -(y_min + (y_max - y_min)/2.0f);

	for(Sites::iterator site = sites->begin();
		site != sites->end();
		site++)
	{
		(*site)->x += x_translation;
		(*site)->y += y_translation;
	}

	*width = (x_max - x_min);
	*height = (y_max - y_min);

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
		printf("[%d] next event\n", iteration++);
		diagram->nextEvent();
		break;
	case 'R':
	case 'r':
		printf("restarting animation\n");
		iteration = 0;
		diagram->restartAnimation();
		break;
	case 'T':
	case 't':
		// printf("Generating new points");
		break;
	case '+':
		printf("%3.3f x %3.3f\n", diagram->getViewWidth(), diagram->getViewHeight());
		diagram->setViewWidth(diagram->getViewWidth()*.9);
		diagram->setViewHeight(diagram->getViewHeight()*.9);
		break;
	case '-':
		diagram->setViewWidth(diagram->getViewWidth()*1.1);
		diagram->setViewHeight(diagram->getViewHeight()*1.1);
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
	//	SitesPtr sites = generateSites(30, g_diagram_width, g_diagram_height);
	float diagram_width, diagram_height;
	SitesPtr sites = readXYZ("../terrain_data/clevel.xyz", &g_diagram_width, &g_diagram_height);

	diagram.reset(new AnimatedVoronoiDiagram(sites, g_diagram_width, g_diagram_height));
	diagram->restartAnimation();

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
