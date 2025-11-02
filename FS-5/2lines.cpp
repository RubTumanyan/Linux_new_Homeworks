#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main(int argc , char* argv[]) {

	if(argc != 2) {
		std::cerr << "Usage " << argv[0] << "file path\n";
		return EXIT_FAILURE;
	}

	char* path = argv[1];//its our path for opening

	int fd = open(path , O_CREAT | O_WRONLY , 0644);
	if (fd < 0) {
		perror("Open the file");
		return EXIT_FAILURE;
	}

	int fd2 = open(path , O_WRONLY);
	if (fd2 < 0) {
	   perror("Open the file");
	   close(fd);
	   return EXIT_FAILURE;
        }
   
	//create a first line
	const char* firstline = "first line\n";

	if (write(fd , firstline , std::strlen(firstline)) < 0) {
		perror("Error with writing");
		close(fd);
		close(fd2);
		return EXIT_FAILURE;
	}

	close(fd2);
	fd2 = dup(fd);
	if (fd2 < 0) {
		perror("Error with duplicate");
		close(fd);
		return EXIT_FAILURE;
	}

	const char* secondLine = "second line\n";
	
	//now we write a second line into the same file
	
	if(write(fd2 , secondLine , std::strlen(secondLine)) < 0) {
		perror("Error with writing second line");
		close(fd);
		close(fd2);
		return EXIT_FAILURE;
	}

	close(fd);
	close(fd2);
	std::cout << "We write 2lines successfully!!!" << std::endl;
}

