#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc , char* argv[]) {
	
	if (argc < 3) {
		fprintf(stderr , "Using path <file_path> %s and " , argv[0] );
		return EXIT_FAILURE;
        }

	const char* file = argv[1];
	const char* file2 = argv[2];

	int desfd = open(file,O_RDONLY);
	if (desfd < 0) {
		fprintf(stderr , "Error open file %s " , argv[1]);
	        return EXIT_FAILURE;
	}
       	int desfd1 = open(file2,O_WRONLY | O_CREAT | O_TRUNC , 0644 );
        if (desfd1 < 0) {
                fprintf(stderr , "Error open file %s " , argv[1]);
                return EXIT_FAILURE;
        }
	
	int bufferSize = 4096;
	char buffer[bufferSize];
	int bytesRead;

	while ((bytesRead = read(desfd, buffer , bufferSize )) > 0) {
		int bytesWritten = 0;
		while(bytesWritten < bytesRead) {
			int ret = write(desfd1 , buffer + bytesWritten , bytesRead - bytesWritten);
			if (ret < 0) {
				fprintf(stderr , "Error");
				close(desfd);
				close(desfd1);
				return EXIT_FAILURE;
			}
			bytesWritten += ret;
		}
	}
	close(desfd);
        close(desfd1);
	
        printf("file copied succesfully");
        
}
