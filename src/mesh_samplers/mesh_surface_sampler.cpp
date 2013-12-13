#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cfloat>

#include <tiny_obj_loader.h>

#include <vector>
#include <string>
#include <memory>

#include "mesh_sampler_utils.h"

using std::vector;
using std::string;
using tinyobj::shape_t;
using tinyobj::mesh_t;

int main(int argc, char** argv)
{
	char* in_filename;
	char* out_filename;
	int num_rays = 40;

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

	}

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


	BoundingBox bounding_box = findBoundingBox(shapes);

	printf("Bounding box:\n");
	printf("\t [%3.3f, %3.3f]\n", bounding_box.x_min, bounding_box.x_max);
	printf("\t [%3.3f, %3.3f]\n", bounding_box.y_min, bounding_box.y_max);
	printf("\t [%3.3f, %3.3f]\n", bounding_box.z_min, bounding_box.z_max);
	
	vector<Point> sample_points;
	for(int i=0; i<num_rays; i++)
	{
		for(int j=0; j<num_rays; j++)
		{
			Vertex start;
			start.x = i*(bounding_box.x_max - bounding_box.x_min)/(float)num_rays + bounding_box.x_min;
			start.y = j*(bounding_box.y_max - bounding_box.y_min)/(float)num_rays + bounding_box.y_min;
			start.z = bounding_box.z_max;
			
			Vector dir;
			dir.i = dir.j = 0;
			dir.k = -1.0;
			
			float min_t = FLT_MAX;
			for(std::vector<shape_t>::const_iterator shape = shapes.begin();
				shape != shapes.end();
				shape++)
			{
				
				for(int k=0; k < (int)shape->mesh.indices.size(); k += 3)
				{
					Vertex v0;
					v0.x = shape->mesh.positions[shape->mesh.indices[k]*3];
					v0.y = shape->mesh.positions[shape->mesh.indices[k]*3+1];
					v0.z = shape->mesh.positions[shape->mesh.indices[k]*3+2];
				
					Vertex v1;
					v1.x = shape->mesh.positions[shape->mesh.indices[k+1]*3];
					v1.y = shape->mesh.positions[shape->mesh.indices[k+1]*3+1];
					v1.z = shape->mesh.positions[shape->mesh.indices[k+1]*3+2];
				
					Vertex v2;
					v2.x = shape->mesh.positions[shape->mesh.indices[k+2]*3];
					v2.y = shape->mesh.positions[shape->mesh.indices[k+2]*3+1];
					v2.z = shape->mesh.positions[shape->mesh.indices[k+2]*3+2];
				
					float t = intersectMesh(start, dir, v0, v1, v2);
					if(t < min_t && t >= 0)
						min_t = t;
				}
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
