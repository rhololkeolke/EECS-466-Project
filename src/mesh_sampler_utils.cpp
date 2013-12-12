#include "mesh_sampler_utils.h"

#include <cmath>
#include <cfloat>

float intersectMesh(const Point& start, const Vector& dir, const Vertex& v0, const Vertex& v1, const Vertex& v2)
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

	Vector P = cross(dir, E2);
	Vector Q = cross(T, E1);

	float coeff = 1.0f/dot(P, E1);
	float t = coeff*dot(Q, E2);
	float u = coeff*dot(P, T);
	float v = coeff*dot(Q, dir);
	if(u<0 || u>1 || v<0 || v>1 || u+v>1)
		return -1;

	if(std::isfinite(t))
		return t;
	else
		return -1;
}

BoundingBox findBoundingBox(const std::vector<float>& vertices)
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

void rotationMatrixAboutAxis(RotationAxis axis, float angle, float* rot_mat)
{
	switch(axis)
	{
	case RotationAxis::X_AXIS:
		rot_mat[0] = 1;
		rot_mat[1] = rot_mat[2] = rot_mat[3] = rot_mat[6] = 0;
		rot_mat[4] = cos(angle);
		rot_mat[5] = -sin(angle);
		rot_mat[7] = sin(angle);
		rot_mat[8] = cos(angle);
		break;
	case RotationAxis::Y_AXIS:
		rot_mat[0] = cos(angle);
		rot_mat[1] = rot_mat[3] = rot_mat[5] = rot_mat[7] = 0;
		rot_mat[4] = 1;
		rot_mat[2] = sin(angle);
		rot_mat[6] = -sin(angle);
		rot_mat[8] = cos(angle);
		break;
	case RotationAxis::Z_AXIS:
		rot_mat[2] = rot_mat[5] = rot_mat[6] = rot_mat[7] = 0;
		rot_mat[8] = 1;
		rot_mat[0] = cos(angle);
		rot_mat[1] = -sin(angle);
		rot_mat[3] = sin(angle);
		rot_mat[4] = cos(angle);
		break;
	}
}

void rotateMesh(tinyobj::mesh_t& mesh, float* rot_mat)
{
	float x, y, z;
	for(int i=0; i < mesh.positions.size(); i += 3)
	{
		x = mesh.positions[i];
		y = mesh.positions[i+1];
		z = mesh.positions[i+2];

		mesh.positions[i] = rot_mat[0]*x + rot_mat[1]*y + rot_mat[2]*z;
		mesh.positions[i+1] = rot_mat[3]*x + rot_mat[4]*y + rot_mat[5]*z;
		mesh.positions[i+2] = rot_mat[6]*x + rot_mat[7]*y + rot_mat[8]*z;
	}
}

