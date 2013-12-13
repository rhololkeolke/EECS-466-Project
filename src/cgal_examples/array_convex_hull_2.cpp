#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>

#include <GL/glut.h>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;

std::vector<Point_2> input;
std::vector<Point_2> result;

void DisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluOrtho2D(-20, 20, -20, 20);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPointSize(4.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_POINTS); {
		for(std::vector<Point_2>::const_iterator point = input.begin();
			point != input.end();
			point++)
		{
			glVertex2f((float)point->x(), (float)point->y());
		}
	} glEnd();

	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP); {
		for(std::vector<Point_2>::const_iterator point = result.begin();
			point != result.end();
			point++)
		{
			glVertex2f((float)point->x(), (float)point->y());
		}
	} glEnd();

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	input.push_back(Point_2(0,0));
	input.push_back(Point_2(10,0));
	input.push_back(Point_2(10,10));
	input.push_back(Point_2(6,5));
	input.push_back(Point_2(4,1));

	Point_2* points = &input[0];

	Point_2 temp[input.size()];
	Point_2 *ptr = CGAL::convex_hull_2( points, points+input.size(), temp );

	for(int i=0; i < ptr-temp; i++)
	{
		result.push_back(temp[i]);
	}

	std::cout << result.size() << " points on the convex hull." << std::endl;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(200, 200);
	glutCreateWindow("Convex hull example");

	glutDisplayFunc(DisplayFunc);

	glutMainLoop();
	
	return 0;
}
