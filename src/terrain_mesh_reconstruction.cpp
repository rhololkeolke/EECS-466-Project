#include <GL/glut.h>
#include <voronoi_diagram.h>
#include <animated_voronoi_diagram.h>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cfloat>

#include <vector>
#include <string>

#define PI 3.14159265359

using namespace voronoi_diagram;

bool g_show_delaunay = true;
bool g_show_voronoi = true;

int WindowWidth = 640;
int WindowHeight = 640;

int MouseX = 0;
int MouseY = 0;
bool MouseLeft = false;
bool MouseRight = false;

std::unique_ptr<VoronoiDiagram> diagram;

voronoi_diagram::EdgesPtr edges;

typedef struct TerrainData_
{
	SitesPtr sites;
	float min_x, max_x;
	float min_y, max_y;
	float min_z, max_z;
} TerrainData;

typedef struct Camera_
{
	float radius;
	float phi;
	float theta;
} Camera;

TerrainData g_terrain_data;
Camera g_camera;

void readTerrainData(std::string filename, TerrainData& data, int line_skip=1)
{
	FILE *terrain_file;
	terrain_file = fopen(filename.c_str(), "r");

	if(!terrain_file)
	{
		fprintf(stderr, "Failed to open file %s", filename.c_str());
		fclose(terrain_file);
		exit(-1);
	}

	data.sites.reset(new Sites());
	data.min_x = data.min_y = data.min_z = FLT_MAX;
	data.max_x = data.max_y = data.max_z = -FLT_MAX;

	float x, y, z;
	int line = 0;
	while(fscanf(terrain_file, "%f %f %f" , &x, &y, &z) != EOF)
	{
		if((line++) % line_skip != 0)
		{
			continue;
		}
		
		SitePtr site(new Site(x, y, z));
		data.sites->push_back(site);

		if(x < data.min_x)
			data.min_x = x;
		if(x > data.max_x)
			data.max_x = x;
		if(y < data.min_y)
			data.min_y = y;
		if(y > data.max_y)
			data.max_y = y;
		if(z < data.min_z)
			data.min_z = z;
		if(z > data.max_z)
			data.max_z = z;
	}

	// translate all of the points to the origin
	float x_trans = -(data.min_x + (data.max_x - data.min_x)/2.0f);
	float y_trans = -(data.min_y + (data.max_y - data.min_y)/2.0f);

	for(Sites::iterator site = data.sites->begin();
		site != data.sites->end();
		site++)
	{
		(*site)->x += x_trans;
		(*site)->y += y_trans;
	}

	fclose(terrain_file);
}

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

	//gluOrtho2D(-diagram->getWidth()/2.0f, diagram->getWidth()/2.0f, -diagram->getHeight()/2.0f, diagram->getHeight()/2.0f);
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
		for(Sites::const_iterator site = g_terrain_data.sites->begin();
			site != g_terrain_data.sites->end();
			site++)
		{
			glVertex3f((*site)->x, (*site)->y, (*site)->z);
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
				glVertex3f((*edge)->left_->x, (*edge)->left_->y, (*edge)->left_->z);
				glVertex3f((*edge)->right_->x, (*edge)->right_->y, (*edge)->right_->z);
			}
		} glEnd();
	}

	glutSwapBuffers();

	//diagram->display();
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
		g_camera.radius *= .9f;
		break;
	case '-':
		g_camera.radius *= 1.1f;
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
		/*case 'N':
	case 'n':
		diagram->nextEvent();
		break;*/
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
	g_camera.radius = 1000.0f;
	g_camera.phi = 1.57;
	g_camera.theta = 1.57;

	/*	float init_diagram_width = 200.0f;
	float init_diagram_height = 200.0f;
	SitesPtr sites = generateSites(30, init_diagram_width, init_diagram_height);

	diagram.reset(new VoronoiDiagram(sites, init_diagram_width, init_diagram_height));

	edges = diagram->getEdges();*/

	printf("Reading terrain data\n");
	readTerrainData("../terrain_data/clevel.xyz", g_terrain_data, 10000);
	//readTerrainData("../terrain_data/test.xyz", g_terrain_data);
	//readTerrainData("../terrain_data/downsampled_clevel.xyz", g_terrain_data);
	printf("Number of sites: %d\n", g_terrain_data.sites->size());
	printf("X Data Range: [%3.3f, %3.3f]\n", g_terrain_data.min_x, g_terrain_data.max_x);
	printf("Y Data Range: [%3.3f, %3.3f]\n", g_terrain_data.min_y, g_terrain_data.max_y);
	printf("Z Data Range: [%3.3f, %3.3f]\n", g_terrain_data.min_z, g_terrain_data.max_z);

	diagram.reset(new VoronoiDiagram(g_terrain_data.sites,
									 (g_terrain_data.max_x - g_terrain_data.min_x),
									 (g_terrain_data.max_y - g_terrain_data.min_y)));
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
