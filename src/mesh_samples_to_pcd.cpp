#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>

#include "mesh_samplers/mesh_sampler_utils.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
	if(argc <= 2)
	{
		std::cerr << "You must supply a mesh sample filename and pcd output filename" << std:: endl;

		return 1;
	}
	
	// load the xyz samples
	MeshSamples mesh_samples;
	readMeshSamplesFile(argv[1], mesh_samples);

	// create a point cloud from the samples
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	for(std::vector<Point>::const_iterator point = mesh_samples.samples.begin();
		point != mesh_samples.samples.end();
		point++)
	{
		pcl::PointXYZ pcl_point(point->x, point->y, point->z);
		cloud->push_back(pcl_point);
	}

	std::cout << cloud->points.size() << " in sample file" << std::endl;

	// estimate the normals
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
	tree->setInputCloud(cloud);
	n.setInputCloud(cloud);
	n.setSearchMethod(tree);
	n.setKSearch(20);
	n.compute(*normals);

	// merge the normals and point cloud
	pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
	pcl::concatenateFields(*cloud, *normals, *cloud_with_normals);

	// save as a pcd file
	pcl::io::savePCDFileASCII(std::string(argv[2]), *cloud_with_normals);
}
