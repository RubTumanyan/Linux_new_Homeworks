#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

int main(int argc , char* argv[]) {
        // we are check the arguments
	if (argc != 2) {
		std::cerr << "usage " << argv[0] << "file to erase \n";
		return EXIT_FAILURE;
	}

	const char* path = argv[1];

	// we are open the file and check for errors
       
	int fd = open( path , O_WRONLY);
	if (fd == -1) {
		perror("Error");
		return EXIT_FAILURE;
	}
        
	//by this way we are getting size of file
	off_t fileSize = lseek(fd , 0 , SEEK_END );
	if (fileSize == -1) {
		perror("Error");
		close(fd);
		return EXIT_FAILURE;
	}

        if (lseek(fd , 0 , SEEK_SET) == -1) {
		perror("Error");
		close(fd);
		return EXIT_FAILURE;
	}

	const int BUFFERSIZE = 64*1024;
	char buffer[BUFFERSIZE] = {};

	off_t remaining = fileSize;
	while (remaining > 0) {
		ssize_t to_write = remaining > BUFFERSIZE ? BUFFERSIZE : remaining;
		ssize_t written = write(fd , buffer , to_write);
		if (written == -1) {
			perror("Error");
			close(fd);
			return EXIT_FAILURE;
		}
		remaining -= written;
	}
		close(fd);

		if (unlink(path) == -1) {
			perror("ERROR");
			return EXIT_FAILURE;
		}
		std::cout << "File successfully deleted\n";
		return EXIT_SUCCESS;
}
