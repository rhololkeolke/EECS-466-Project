#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <tiny_obj_loader.h>

#include <vector>
#include <string>

using std::vector;
using std::string;
using tinyobj::shape_t;
using tinyobj::mesh_t;

void calculateNormals(mesh_t& mesh);

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

	printf("Number of shapes: %ul\n", shapes.size());

	for(vector<shape_t>::const_iterator shape = shapes.begin();
		shape != shapes.end();
		shape++)
	{
		printf("positions: %ul\n", shape->mesh.positions.size());
		printf("normals: %ul\n", shape->mesh.normals.size());
		printf("texcoords: %ul\n", shape->mesh.texcoords.size());
		printf("indices: %ul\n", shape->mesh.indices.size());

		printf("positions:\n");
		for(vector<float>::const_iterator position = shape->mesh.positions.begin();
			position != shape->mesh.positions.end();
			position++)
		{
			printf("\t %3.3f\n", *position);
		}

		printf("indices:\n");
		for(vector<unsigned int>::const_iterator index = shape->mesh.indices.begin();
			index != shape->mesh.indices.end();
			index++)
		{
			printf("\t %d\n", *index);
		}
	}
}

void calculateNormals(mesh_t& mesh)
{

}
