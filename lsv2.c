/*
* we will try to print current directory's content if no argument is passed and supports multiple dir inputs
*/




#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

void reads_directory_and_prints_its_content(char *path);

extern int errno;
int main(int argc, char *argv[])
{
	
	// if no argument is passed, print current directory's content
	if ( argc == 1) {
		reads_directory_and_prints_its_content(".");
		exit(0);
	}
	int i = 1;

	// we iterate through the arguments and print their content
	while (argv[i] != NULL) {
		printf("%s:\n", argv[i]);
		reads_directory_and_prints_its_content(argv[i]);
		i++;	
	}

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
		perror("Error reading directory\n");
		exit(0);
	}	
	while (entry != NULL) {
		
		if (entry->d_name[0] == '.') 
			continue;

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
