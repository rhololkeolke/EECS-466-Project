/*
  Simple mesh sampling program.

  Takes one sample for each triangle at the center of the barycentric coordinates.
 */

#include <cstdlib>
#include <cstdio>

#include <tiny_obj_loader.h>

#include <vector>
#include <string>
#include <random>

#include "mesh_sampler_utils.h"

using std::vector;
using std::string;
using tinyobj::shape_t;
using tinyobj::mesh_t;

int main(int argc, char** argv)
{
	char* in_filename;
	char* out_filename;
	int num_samples = 1;
	bool add_noise = false;
	float std_dev = 0;

	if(argc <= 2)
	{
		fprintf(stderr, "You must supply a filename for the mesh and sample file");
		return 1;
	}
	else
	{
		in_filename = argv[1];
		out_filename = argv[2];
		if(argc >= 4)
		{
			num_samples = atoi(argv[3]);
		}
		if(argc >= 5)
		{
			add_noise = true;
			std_dev = atof(argv[4]);
		}
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::normal_distribution<> gauss(0, std_dev);

	std::uniform_real_distribution<> uniform(0, 1);
	
	printf("Sampling mesh %s with %d samples per triangle.\nSaving samples to %s\n", in_filename, num_samples, out_filename);

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

			for(int j=0; j<num_samples; j++)
			{
				float u = uniform(gen);
				float v = (1-u)*uniform(gen);
				Point sample_point;
				sample_point.x = u*v0.x + v*v1.x + (1-u-v)*v2.x;
				sample_point.y = u*v0.y + v*v1.y + (1-u-v)*v2.y;
				sample_point.z = u*v0.z + v*v1.z + (1-u-v)*v2.z;

				if(add_noise)
				{
					float dim_chance = uniform(gen);
					if(dim_chance < .33)
					{
						sample_point.x += gauss(gen);
					}
					else if(dim_chance < .66)
					{
						sample_point.y += gauss(gen);
					}
					else
					{
						sample_point.z += gauss(gen);
					}
				}
				sample_points.push_back(sample_point);
			}
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
