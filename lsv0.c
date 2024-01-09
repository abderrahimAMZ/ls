



#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

void reads_directory_and_prints_its_content(char *path);

extern int errno;
int main(int argc, char *argv[])
{
	
	if ( argc == 1) {
		perror("Path isn't provided\n");
		exit(0);
	}
	
	reads_directory_and_prints_its_content(argv[1]);

	


	return EXIT_SUCCESS;
}




void reads_directory_and_prints_its_content(char *path) {


	DIR *dir;
	dir = opendir(path);
	// handle directory opening error
	if (dir == NULL) {
		perror("Error opening directory\n");
		exit(0);
	}
	// reading directory
	struct dirent *entry;
	entry = readdir(dir);

	if (entry == NULL) {
		if (errno == EBADF) {
			perror("Invalid directory stream descriptor\n");
			exit(0);
		}
	}	
	while (entry != NULL) {
		printf("%s\n", entry->d_name);
		entry = readdir(dir);
	}

	// handle directory closing error
	if (closedir(dir) == -1) {
		perror("Error closing directory\n");
		exit(0);
	}

	// set pointer to the beginning so that it can be read again
	rewinddir(dir);

}
