#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/search/kdtree.h>
#include <pcl/surface/poisson.h>

#include <iostream>
#include <vector>

#include <cstdio>

int main(int argc, char** argv)
{
	if(argc <= 2)
	{
		std::cerr << "You must supply a pcd filename and an output filename" << std::endl;

		return 1;
	}

	pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
	pcl::io::loadPCDFile<pcl::PointNormal>(std::string(argv[1]), *cloud_with_normals);

	std::cout << cloud_with_normals->points.size() << " points in " << std::string(argv[1]) << std::endl;

	pcl::search::KdTree<pcl::PointNormal>::Ptr tree (new pcl::search::KdTree<pcl::PointNormal>);
	tree->setInputCloud(cloud_with_normals);

	pcl::Poisson<pcl::PointNormal> poisson;
	pcl::PolygonMesh triangles;

	poisson.setDepth(10);
	
	poisson.setInputCloud(cloud_with_normals);
	poisson.setSearchMethod(tree);
	poisson.reconstruct(triangles);

	std::cout << triangles.polygons.size() << " polygons in reconstructed mesh" << std::endl;

	pcl::PointCloud<pcl::PointXYZ>::Ptr mesh_cloud(new pcl::PointCloud<pcl::PointXYZ>);
	pcl::fromROSMsg(triangles.cloud, *mesh_cloud);

	FILE* mesh_file = fopen(argv[2], "w");

	if(!mesh_file)
	{
		std::cerr << "ERROR: Could not open file " << std::string(argv[2]) << std::endl;

		return 2;
	}

	for(pcl::PointCloud<pcl::PointXYZ>::const_iterator point = mesh_cloud->points.begin();
		point != mesh_cloud->points.end();
		point++)
	{
		fprintf(mesh_file, "v %f %f %f\n",
				point->x,
				point->y,
				point->z);
	}

	fprintf(mesh_file, "\n");

	for(std::vector<pcl::Vertices>::const_iterator polygon = triangles.polygons.begin();
		polygon != triangles.polygons.end();
		polygon++)
	{
		fprintf(mesh_file, "f %d %d %d\n",
				polygon->vertices[0]+1,
				polygon->vertices[1]+1,
				polygon->vertices[2]+1);
	}

	fclose(mesh_file);

}
