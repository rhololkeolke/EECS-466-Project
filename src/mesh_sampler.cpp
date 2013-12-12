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

BoundingBox findBoundingBox(const vector<float>& vertices);

void rotationMatrixAboutAxis(RotationAxis axis, float angle, float* rot_mat);

void rotateMesh(mesh_t& mesh, float* rot_mat);

int main(int argc, char** argv)
{
	char* in_filename;
	char* out_filename;
	int num_rays = 40;
	int num_rotations = 1;
	RotationAxis axis = RotationAxis::Y_AXIS;

	if(argc <= 2)
	{
		fprintf(stderr, "You must supply at least a filename of the obj file to be sampled");
		return 1;
	}
	else
	{
		in_filename = argv[1];
		out_filename = argv[2];
		if(argc >=4)
		{
			num_rays = atoi(argv[3]);
		}

		if(argc >= 5)
		{
			num_rotations = atoi(argv[4]);
		}

		if(argc >= 6)
		{
			switch(argv[5][0])
			{
			case 'x':
				axis = RotationAxis::X_AXIS;
				break;
			case 'y':
				axis = RotationAxis::Y_AXIS;
				break;
			case 'z':
				axis = RotationAxis::Z_AXIS;
				break;
			default:
				fprintf(stderr, "Invalid axis %c\n", argv[5][0]);

				return 1;
				break;
			}
		}
	}

	std::string axis_str;
	switch(axis)
	{
	case RotationAxis::X_AXIS:
		axis_str = "x-axis";
		break;
	case RotationAxis::Y_AXIS:
		axis_str = "y-axis";
		break;
	case RotationAxis::Z_AXIS:
		axis_str = "z-axis";
		break;
	default:
		axis_str = "UNKNOWN";
		break;
	}
	printf("Sampling mesh %s with %dx%d rays and %d rotations about the %s.\nSaving to %s\n", in_filename, num_rays, num_rays, num_rotations, axis_str.c_str(), out_filename);

	vector<shape_t> shapes;

	string error_text = LoadObj(shapes, in_filename);

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

	float init_rot_mat[9];
	switch(axis)
	{
	case RotationAxis::X_AXIS:
		rotationMatrixAboutAxis(RotationAxis::Z_AXIS, 1.57, init_rot_mat);
		break;
	case RotationAxis::Y_AXIS:
		init_rot_mat[0] = init_rot_mat[4] = init_rot_mat[8] = 1;
		init_rot_mat[1] = init_rot_mat[2] = init_rot_mat[3] = init_rot_mat[5] = init_rot_mat[6] = init_rot_mat[7] = 0;
		break;
	case RotationAxis::Z_AXIS:
		rotationMatrixAboutAxis(RotationAxis::X_AXIS, -1.57, init_rot_mat);
		break;
	}

	rotateMesh(shapes[0].mesh, init_rot_mat);

	float rot_mat[9];
	rotationMatrixAboutAxis(axis, 6.28/(float)num_rotations, rot_mat);	

	vector<Point> sample_points;
	for(int rot_count=0; rot_count < num_rotations; rot_count++)
	{
		
		BoundingBox bounding_box = findBoundingBox(shapes[0].mesh.positions);

		printf("Bounding box:\n");
		printf("\t [%3.3f, %3.3f]\n", bounding_box.x_min, bounding_box.x_max);
		printf("\t [%3.3f, %3.3f]\n", bounding_box.y_min, bounding_box.y_max);
		printf("\t [%3.3f, %3.3f]\n", bounding_box.z_min, bounding_box.z_max);


		for(int i=0; i<num_rays; i++)
		{
			for(int j=0; j<num_rays; j++)
			{
				Vertex start;
				start.x = i*(bounding_box.x_max - bounding_box.x_min)/(float)num_rays + bounding_box.x_min;
				start.y = j*(bounding_box.y_max - bounding_box.y_min)/(float)num_rays + bounding_box.y_min;
				start.z = bounding_box.z_min;

				Vector dir;
				dir.i = dir.j = 0;
				dir.k = 1.0;

				float min_t = FLT_MAX;
				for(int k=0; k < (int)shapes[0].mesh.indices.size(); k += 3)
				{
					Vertex v0;
					v0.x = shapes[0].mesh.positions[shapes[0].mesh.indices[k]*3];
					v0.y = shapes[0].mesh.positions[shapes[0].mesh.indices[k]*3+1];
					v0.z = shapes[0].mesh.positions[shapes[0].mesh.indices[k]*3+2];

					Vertex v1;
					v1.x = shapes[0].mesh.positions[shapes[0].mesh.indices[k+1]*3];
					v1.y = shapes[0].mesh.positions[shapes[0].mesh.indices[k+1]*3+1];
					v1.z = shapes[0].mesh.positions[shapes[0].mesh.indices[k+1]*3+2];

					Vertex v2;
					v2.x = shapes[0].mesh.positions[shapes[0].mesh.indices[k+2]*3];
					v2.y = shapes[0].mesh.positions[shapes[0].mesh.indices[k+2]*3+1];
					v2.z = shapes[0].mesh.positions[shapes[0].mesh.indices[k+2]*3+2];

					float t = intersectMesh(start, dir, v0, v1, v2);
					if(t < min_t && t >= 0)
						min_t = t;
				}

				if(min_t == FLT_MAX)
					continue;

				Point sample_point;
				sample_point.x = start.x + dir.i*min_t;
				sample_point.y = start.y + dir.j*min_t;
				sample_point.z = start.z + dir.k*min_t;
				sample_points.push_back(sample_point);
			}
		}

		// finally prepare the mesh for the next interation
		rotateMesh(shapes[0].mesh, rot_mat);
	}
	printf("Collected %d sample points\n", sample_points.size());

	FILE* out_file = fopen(out_filename, "w");
	if(out_file == NULL)
	{
		fprintf(stderr, "Failed to save samples to %s\n", out_filename);
		return 4;
	}

	for(vector<Point>::const_iterator point = sample_points.begin();
		point != sample_points.end();
		point++)
	{
		fprintf(out_file, "%3.3f %3.3f %3.3f\n", point->x, point->y, point->z);
	}

	fclose(out_file);

}

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

void rotateMesh(mesh_t& mesh, float* rot_mat)
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
