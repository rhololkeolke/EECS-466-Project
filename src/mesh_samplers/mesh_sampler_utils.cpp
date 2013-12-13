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

BoundingBox findBoundingBox(const std::vector<tinyobj::shape_t>& shapes)
{
	BoundingBox result;
	result.x_min = result.y_min = result.z_min = FLT_MAX;
	result.x_max = result.y_max = result.z_max = -FLT_MAX;

	for(std::vector<tinyobj::shape_t>::const_iterator shape = shapes.begin();
		shape != shapes.end();
		shape++)
	{

		std::vector<float> vertices(shape->mesh.positions.begin(), shape->mesh.positions.end());
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

void readMeshSamplesFile(std::string filename, MeshSamples& data)
{
	FILE* mesh_sample_file;
	mesh_sample_file = fopen(filename.c_str(), "r");

	if(!mesh_sample_file)
	{
		fprintf(stderr, "Failed to open file %s", filename.c_str());
		fclose(mesh_sample_file);
		exit(-1);
	}

	data.samples.clear();
	data.x_min = data.y_min = data.z_min = FLT_MAX;
	data.x_max = data.y_max = data.z_max = -FLT_MAX;

	Point p;
	while(fscanf(mesh_sample_file, "%f %f %f", &p.x, &p.y, &p.z) != EOF)
	{
		data.samples.push_back(p);
	}

	if(p.x < data.x_min)
		data.x_min = p.x;
	if(p.x > data.x_max)
		data.x_max = p.x;

	if(p.y < data.y_min)
		data.y_min = p.y;
	if(p.y > data.y_max)
		data.y_max = p.y;

	if(p.z < data.z_min)
		data.z_min = p.z;
	if(p.z > data.z_max)
		data.z_max = p.z;

	float x_trans = -(data.x_min + ( data.x_max - data.x_min)/2.0f);
	float y_trans = -(data.y_min + ( data.y_max - data.y_min)/2.0f);
	float z_trans = -(data.z_min + ( data.z_max - data.z_min)/2.0f);

	for(std::vector<Point>::iterator sample = data.samples.begin();
		sample != data.samples.end();
		sample++)
	{
		sample->x += x_trans;
		sample->y += y_trans;
		sample->z += z_trans;
	}

	fclose(mesh_sample_file);
}
