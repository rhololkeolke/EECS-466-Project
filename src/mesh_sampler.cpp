#include <cstdlib>
#include <cstdio>

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
}
