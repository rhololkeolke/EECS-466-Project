/*
  Simple mesh sampling program.

  Takes one sample for each triangle at the center of the barycentric coordinates.
 */

#include <cstdlib>
#include <cstdio>

#include <tiny_obj_loader.h>

#include <vector>
#include <string>

#include "mesh_sampler_utils.h"

using std::vector;
using std::string;
using tinyobj::shape_t;
using tinyobj::mesh_t;

int main(int argc, char** argv)
{
	char* in_filename;
	char* out_filename;

	if(argc <= 2)
	{
		fprintf(stderr, "You must supply a filename for the mesh and sample file");
		return 1;
	}
	else
	{
		in_filename = argv[1];
		out_filename = argv[2];
	}

	printf("Sampling mesh %s\n Saving samples to %s\n", in_filename, out_filename);

	vector<shape_t> shapes;

	string error_text = LoadObj(shapes, in_filename);

	if(error_text.length() != 0)
	{
		fprintf(stderr, "ERROR: %s\n", error_text.c_str());

		return 2;
	}

	if(shapes.size() == 0)
	{
		fprintf(stderr, "Mesh file did not contain any shapes...");
		return 3;
	}

	vector<Point> sample_points;
	for(vector<shape_t>::const_iterator shape = shapes.begin();
		shape != shapes.end();
		shape++)
	{
		for(int i=0; i < (int) shape->mesh.indices.size(); i+= 3)
		{
			Vertex v0;
			v0.x = shape->mesh.positions[shape->mesh.indices[i]*3];
			v0.y = shape->mesh.positions[shape->mesh.indices[i]*3+1];
			v0.z = shape->mesh.positions[shape->mesh.indices[i]*3+2];
			
			Vertex v1;
			v1.x = shape->mesh.positions[shape->mesh.indices[i+1]*3];
			v1.y = shape->mesh.positions[shape->mesh.indices[i+1]*3+1];
			v1.z = shape->mesh.positions[shape->mesh.indices[i+1]*3+2];
			
			Vertex v2;
			v2.x = shape->mesh.positions[shape->mesh.indices[i+2]*3];
			v2.y = shape->mesh.positions[shape->mesh.indices[i+2]*3+1];
			v2.z = shape->mesh.positions[shape->mesh.indices[i+2]*3+2];
			
			Point sample_point;
			sample_point.x = .33*v0.x + .33*v1.x + (1-.66)*v2.x;
			sample_point.y = .33*v0.y + .33*v1.y + (1-.66)*v2.y;
			sample_point.z = .33*v0.z + .33*v1.z + (1-.66)*v2.z;
			sample_points.push_back(sample_point);
		}
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
