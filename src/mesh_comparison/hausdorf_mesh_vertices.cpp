#include <tiny_obj_loader.h>

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cmath>

#include <cstdio>

double hausford_distance(const std::vector<tinyobj::shape_t>& shapes_a, const std::vector<tinyobj::shape_t>& shapes_b);

int main(int argc, char** argv)
{
	if(argc <= 2)
	{
		std::cerr << "You must supply both the original mesh obj file and the reconstructed obj file." << std::endl;

		return 1;
	}

	std::string orig_mesh_filename(argv[1]);
	std::string new_mesh_filename(argv[2]);

	std::vector<tinyobj::shape_t> orig_shapes;
	std::vector<tinyobj::shape_t> new_shapes;

	std::string error_text = LoadObj(orig_shapes, orig_mesh_filename.c_str());

	if(error_text.length() != 0)
	{
		std::cerr << "ERROR: Could not load " << orig_mesh_filename << std::endl;
		std::cerr << "\t" << error_text << std::endl;

		return 2;
	}

	error_text = LoadObj(new_shapes, new_mesh_filename.c_str());

	if(error_text.length() != 0)
	{
		std::cerr << "ERROR: Could not load " << new_mesh_filename << std::endl;
		std::cerr << "\t" << error_text << std::endl;

		return 3;
	}

	if(orig_shapes.size() == 0)
	{
		std::cerr << "ERROR: " << orig_mesh_filename << " contains no shapes." << std::endl;

		return 4;
	}

	if(new_shapes.size() == 0)
	{
		std::cerr << "ERROR: " << new_mesh_filename << " contains no shapes." << std::endl;

		return 5;
	}

	int num_vertices = 0;
	for(std::vector<tinyobj::shape_t>::const_iterator shape = orig_shapes.begin();
		shape != orig_shapes.end();
		shape++)
	{
		num_vertices += (int)shape->mesh.positions.size();
	}
	
	std::cout << orig_mesh_filename << " contains " << num_vertices << " vertices." << std::endl;

	num_vertices = 0;
	for(std::vector<tinyobj::shape_t>::const_iterator shape = new_shapes.begin();
		shape != new_shapes.end();
		shape++)
	{
		num_vertices += (int)shape->mesh.positions.size();
	}
	
	std::cout << new_mesh_filename << " contains " << num_vertices << " vertices." << std::endl;

	float orig_to_new_distance = hausford_distance(orig_shapes, new_shapes);
	float new_to_orig_distance = hausford_distance(new_shapes, orig_shapes);
	float mesh_distance = std::max(orig_to_new_distance, new_to_orig_distance);

	std::cout << "orig to new mesh: " << orig_to_new_distance << std::endl;
	std::cout << "new to orig mesh: " << new_to_orig_distance << std::endl;
	std::cout << "Distance: " << mesh_distance << std::endl;
}

double hausford_distance(const std::vector<tinyobj::shape_t>& shapes_a, const std::vector<tinyobj::shape_t>& shapes_b)
{
	double distance = 0;
	for(std::vector<tinyobj::shape_t>::const_iterator shape_a = shapes_a.begin();
		shape_a != shapes_a.end();
		shape_a++)
	{
		/*		for(std::vector<float>::const_iterator position_a = shape_a->mesh.positions.begin();
			position_a != shape_a->mesh.positions.end();
			position_a++)
			{*/
		for(int i=0; i+2 < (int)shape_a->mesh.positions.size(); i += 3)
		{

			double shortest_dist = std::numeric_limits<double>::max();
			
			for(std::vector<tinyobj::shape_t>::const_iterator shape_b = shapes_b.begin();
				shape_b != shapes_b.end();
				shape_b++)
			{

				/*for(std::vector<float>::const_iterator position_b = shape_b->mesh.positions.begin();
					position_b != shape_b->mesh.positions.end();
					position_b++)
					{*/
				for(int j=0; j+2 < (int)shape_b->mesh.positions.size(); j += 3)
				{
					float x_a, x_b;
					float y_a, y_b;
					float z_a, z_b;

					/*	x_a = *position_a;
					if(++position_a == shape_a->mesh.positions.end())
						break;
					y_a = *position_a;
					if(++position_a == shape_a->mesh.positions.end())
						break;
					z_a = *position_a;

					x_b = *position_b;
					if(++position_b == shape_b->mesh.positions.end())
						break;
					y_b = *position_b;
					if(++position_b == shape_b->mesh.positions.end())
						break;
						z_b = *position_b;*/

					x_a = shape_a->mesh.positions[i];
					y_a = shape_a->mesh.positions[i+1];
					z_a = shape_a->mesh.positions[i+2];

					x_b = shape_b->mesh.positions[j];
					y_b = shape_b->mesh.positions[j+1];
					z_b = shape_b->mesh.positions[j+2];

					double x_diff, y_diff, z_diff;
					x_diff = x_a - x_b;
					y_diff = y_a - y_b;
					z_diff = z_a - z_b;
					
					double temp_dist = sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff);
					if(temp_dist < shortest_dist)
						shortest_dist = temp_dist;
				}
			}

			if(shortest_dist > distance)
				distance = shortest_dist;
		}
	}

	return distance;
}
