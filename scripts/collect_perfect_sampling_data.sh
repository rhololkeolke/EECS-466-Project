#!/usr/bin/env bash

# given a folder will find all *.obj files
# for each file it will sample the meshes
# using the mesh_triangle_sampler program.
# It will then attempt to reconstruct the
# meshes using a variety of algorithms.
# finally it will compute the difference
# between the reconstructed meshes and the
# original and save the output as a csv file

if [[ -z "$1" ]]; then
    echo "You must supply a folder containing obj files"
    exit 1
fi

if [[ -z "$2" ]]; then
    echo "You must supply an output folder"
    exit 1
fi

mesh_files=$(find $1 -name '*.obj')

for mesh_file in $mesh_files; do
    echo $mesh_file
done

mkdir -p $2/xyz

# sample the mesh files

for mesh_file in $mesh_files; do
    echo "Sampling $mesh_file"

    pushd $(dirname $mesh_file)
    mesh_triangle_sampler $(basename $mesh_file) $2/xyz/$(echo $(basename $mesh_file) | sed 's/\.obj$/\.xyz/')
    popd
done

xyz_files=$(find $2/xyz -name '*.xyz')

for xyz_file in $xyz_files; do
    echo $xyz_file
done

mkdir -p $2/pcd

# convert the mesh samples to pcd files
for xyz_file in $xyz_files; do
    echo "Converting $xyz_file to pcd with normals"

    pushd $(dirname $xyz_file)
    mesh_samples_to_pcd $(basename $xyz_file) ../pcd/$(echo $(basename $xyz_file) | sed 's/\.xyz$/\.pcd/')
    popd
done

pcd_files=$(find $2/pcd -name '*.pcd')

for pcd_file in $pcd_files; do
    echo $pcd_file
done

mkdir -p $2/obj

# run the pcd files through the marching cubes and poisson programs
for pcd_file in $pcd_files; do
    pushd $(dirname $pcd_file)
    echo "Reconstructing $pcd_file with marching cubes"
    pcl_marching_cubes $(basename $pcd_file) ../obj/$(echo $(basename $pcd_file) | sed 's/\.pcd$/_marching_cubes\.obj/')

    echo "Reconstructing $pcd_file with poisson"
    pcl_poisson $(basename $pcd_file) ../obj/$(echo $(basename $pcd_file) | sed 's/\.pcd$/_poisson\.obj/')
    popd
done

# run the xyz files through the powercrust programs

mkdir -p $2/off
powercrust_temp_dir=$(mktemp -d)
output_path=$(readlink -e $2)

# convert mesh samples to off files
for xyz_file in $xyz_files; do
    echo "Reconstructing $xyz_file with powercrust"

    pushd $powercrust_temp_dir
    powercrust -i $output_path/xyz/$(basename $xyz_file)

    meshlabserver -i pc.off -o $output_path/obj/$(echo $(basename $xyz_file) | sed 's/\.xyz$/_powercrust\.obj/')
    popd
done

# calculate the mesh differences
touch $2/mesh_differences.csv
echo "mesh name, xyz file, pcd file, poisson file, poisson difference, marching cubes file, marching cubes difference, powercrust file, powercrust difference" >> $2/mesh_differences.csv
