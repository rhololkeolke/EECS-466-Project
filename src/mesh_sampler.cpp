#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cfloat>

#include <tiny_obj_loader.h>

#include <vector>
#include <string>
#include <memory>

using std::vector;
using std::string;
using tinyobj::shape_t;
using tinyobj::mesh_t;

typedef struct Vector_
{
	float i, j, k;
} Vector;

typedef struct Point_
{
	float x, y, z;
} Point;

typedef Point Vertex;

typedef struct BoundingBox_
{
	float x_min, x_max;
	float y_min, y_max;
	float z_min, z_max;
} BoundingBox;

inline Vector cross(const Vector& v1, const Vector& v2)
{
	Vector result;
	result.i = v1.j*v2.k - v1.k*v2.j;
	result.j = v1.k*v2.i - v1.i*v2.k;
	result.k = v1.i*v2.j - v1.j*v2.i;
	return result;
}

inline float dot(const Vector& v1, const Vector& v2)
{
	return v1.i*v2.i + v1.j*v2.j + v1.k*v2.k;
}

std::shared_ptr<Point> intersectMesh(const Point& start, const Vector& dir, const Vertex& v0, const Vertex& v1, const Vertex& v2);

BoundingBox findBoundingBox(const vector<float>& vertices);

int main(int argc, char** argv)
{
	char* filename;
	int num_rays = 40;
	int num_rotations = 1;

	if(argc <= 1)
	{
		fprintf(stderr, "You must supply at least a filename of the obj file to be sampled");
		return 1;
	}
	else
	{
		filename = argv[1];
		if(argc >=3)
		{
			num_rays = atoi(argv[2]);
		}

		if(argc >= 4)
		{
			num_rotations = atoi(argv[3]);
		}
	}

	printf("Sampling mesh %s with %d rays and %d rotations\n", filename, num_rays, num_rotations);

	vector<shape_t> shapes;

	string error_text = LoadObj(shapes, filename);

	if(error_text.length() != 0)
	{
		fprintf(stderr, "ERROR: %s\n", error_text.c_str());

		return 2;
	}

	printf("Number of shapes: %d\n", shapes.size());

	if(shapes.size() == 0)
	{
		fprintf(stderr, "Mesh file did not contain any shapes...");
		return 3;
	}

	BoundingBox bounding_box = findBoundingBox(shapes[0].mesh.positions);

	printf("Bounding box:\n");
	printf("\t [%3.3f, %3.3f]\n", bounding_box.x_min, bounding_box.x_max);
	printf("\t [%3.3f, %3.3f]\n", bounding_box.y_min, bounding_box.y_max);
	printf("\t [%3.3f, %3.3f]\n", bounding_box.z_min, bounding_box.z_max);
	
}

std::shared_ptr<Point> intersectMesh(const Point& start, const Vector& dir, const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
	Vector E1;
	E1.i = v1.x - v0.x;
	E1.j = v1.y - v0.y;
	E1.k = v1.z - v0.z;

	Vector E2;
	E2.i = v2.x - v0.x;
	E2.j = v2.y - v0.y;
	E2.k = v2.z - v0.z;

	Vector T;
	T.i = start.x - v0.x;
	T.j = start.y - v0.y;
	T.k = start.z - v0.z;

	Vector P = cross(dir, E1);
	Vector Q = cross(T, E1);

	float coeff = 1.0f/dot(P, E1);
	float t = coeff*dot(Q, E2);
	float u = coeff*dot(P, T);
	float v = coeff*dot(Q, dir);
	if(u<0 || u>1 || v<0 || v>1 || u+v>1)
		return nullptr;

	std::shared_ptr<Point> result(new Point());
	result->x = start.x + dir.i*t;
	result->y = start.y + dir.j*t;
	result->z = start.z + dir.k*t;

	return result;
}

BoundingBox findBoundingBox(const vector<float>& vertices)
{
	BoundingBox result;
	result.x_min = result.y_min = result.z_min = FLT_MAX;
	result.x_max = result.y_max = result.z_max = -FLT_MAX;

	for(int i=0; i < (int)vertices.size(); i += 3)
	{
		if(vertices[i] < result.x_min)
			result.x_min = vertices[i];
		if(vertices[i] > result.x_max)
			result.x_max = vertices[i];

		if(vertices[i+1] < result.y_min)
			result.y_min = vertices[i+1];
		if(vertices[i+1] > result.y_max)
			result.y_max = vertices[i+1];

		if(vertices[i+2] < result.z_min)
			result.z_min = vertices[i+2];
		if(vertices[i+2] > result.z_max)
			result.z_max = vertices[i+2];
	}

	return result;
}
