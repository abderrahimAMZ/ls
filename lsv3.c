/*
* this version has long listing feature
*
*/



#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
void reads_directory_and_prints_its_content(char *path);

char* reads_file_displays_access_rights_and_file_type(char* path);
char* reads_file_displays_file_user_name(char* path);
char* reads_file_displays_file_group_name(char* path);
char* reads_file_displays_file_last_modification_time(char* path);



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
	struct stat statbuf;
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
		perror("Error reading directory\n");
		exit(0);
	}	
	while (entry != NULL) {
		
		/*
		if (entry->d_name[0] == '.') 
			continue;
		*/

		if(stat(entry->d_name, &statbuf) == -1) {
			perror("Error getting file status\n");
			exit(0);
		}
		// print all file status
		// print them in l format
		printf("%s ", reads_file_displays_access_rights_and_file_type(entry->d_name));
		printf("%ld ", statbuf.st_nlink);
		printf("%s ", reads_file_displays_file_user_name(entry->d_name));
		printf("%s ", reads_file_displays_file_group_name(entry->d_name));
		printf("%ld ", statbuf.st_size);
		printf("%s ", reads_file_displays_file_last_modification_time(entry->d_name));
		printf("%s\n", entry->d_name);
		entry = readdir(dir);
		/*
		printf("%ld ", statbuf.st_ino);
		printf("%ld ", statbuf.st_blocks);
		printf("%d ", statbuf.st_uid);
		printf("%d ", statbuf.st_gid);
		printf("%d ", statbuf.st_mode);
		printf("%ld ", statbuf.st_atime);
		printf("%ld ", statbuf.st_mtime);
		printf("%ld ", statbuf.st_ctime);
		*/
	}

	// handle directory closing error
	if (closedir(dir) == -1) {
		perror("Error closing directory\n");
		exit(0);
	}

	// set pointer to the beginning so that it can be read again
	rewinddir(dir);

}


char* reads_file_displays_access_rights_and_file_type(char* path) {
	char * access_rights= malloc(sizeof(char) * 10);
	
	strcpy(access_rights, "----------");
	struct stat statbuf;
	if(stat(path, &statbuf) == -1) {
		perror("Error getting file status\n");
		exit(0);
	}
	if (S_ISDIR(statbuf.st_mode)) {
		access_rights[0] = 'd';
	}
	if (S_ISCHR(statbuf.st_mode)) {
		access_rights[0] = 'c';
	}
	if (S_ISBLK(statbuf.st_mode)) {
		access_rights[0] = 'b';
	}
	if (S_ISFIFO(statbuf.st_mode)) {
		access_rights[0] = 'p';
	}
	if (S_ISLNK(statbuf.st_mode)) {
		access_rights[0] = 'l';
	}
	if (S_ISSOCK(statbuf.st_mode)) {
		access_rights[0] = 's';
	}
	else {
		access_rights[0] = '-';
	}

	if (statbuf.st_mode & S_IRUSR) {
		access_rights[1] = 'r';
	}
	if (statbuf.st_mode & S_IWUSR) {
		access_rights[2] = 'w';
	}
	if (statbuf.st_mode & S_IXUSR) {
		access_rights[3] = 'x';
	}
	if (statbuf.st_mode & S_IRGRP) {
		access_rights[4] = 'r';
	}
	if (statbuf.st_mode & S_IWGRP) {
		access_rights[5] = 'w';
	}
	if (statbuf.st_mode & S_IXGRP) {
		access_rights[6] = 'x';
	}
	if (statbuf.st_mode & S_IROTH) {
		access_rights[7] = 'r';
	}
	if (statbuf.st_mode & S_IWOTH) {
		access_rights[8] = 'w';
	}
	if (statbuf.st_mode & S_IXOTH) {
		access_rights[9] = 'x';
	}
	return access_rights;
}


char* reads_file_displays_file_user_name(char* path){
	struct stat statbuf;
	if(stat(path, &statbuf) == -1) {
		perror("Error getting file status\n");
		exit(0);
	}
	struct passwd *pwd;
	pwd = getpwuid(statbuf.st_uid);
	if (pwd == NULL) {
		perror("getpwuid()");
		exit(0);
	}
	return (char*)pwd->pw_name;
}

char* reads_file_displays_file_group_name(char* path){
	struct stat statbuf;
	if(stat(path, &statbuf) == -1) {
		perror("Error getting file status\n");
		exit(0);
	}
	struct group *grp;
	grp = getgrgid(statbuf.st_gid);
	if (grp == NULL) {
		perror("getpwuid()");
		exit(0);
	}
	return (char*)grp->gr_name;
}


char* reads_file_displays_file_last_modification_time(char* path){
	struct stat statbuf;
	char* time_buffer = malloc(sizeof(char) * 255);
	if(stat(path, &statbuf) == -1) {
		perror("Error getting file status\n");
		exit(0);
	}

	// i want the human readable format
	struct timespec ts = statbuf.st_mtim;
	// i want the ls time format in the -l option
	struct tm lt;
	struct tm *tx = localtime_r(&ts.tv_sec, &lt);
	strftime(time_buffer, sizeof(char)*255, "%H:%M %b %d ", tx);

	//time_buffer = ctime(&statbuf.st_mtime);
	// copy the minutes, hours and day to the buffer in a formated string to time_buffer
	return time_buffer;
}











