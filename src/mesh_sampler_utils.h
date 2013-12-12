#include <tiny_obj_loader.h>
#include <vector>

enum class RotationAxis { X_AXIS, Y_AXIS, Z_AXIS };

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

float intersectMesh(const Point& start, const Vector& dir, const Vertex& v0, const Vertex& v1, const Vertex& v2);

BoundingBox findBoundingBox(const std::vector<float>& vertices);

void rotationMatrixAboutAxis(RotationAxis axis, float angle, float* rot_mat);

void rotateMesh(tinyobj::mesh_t& mesh, float* rot_mat);
