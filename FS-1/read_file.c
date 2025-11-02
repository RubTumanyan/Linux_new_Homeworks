#include <fcntl.h>    
#include <unistd.h>   
#include <stdio.h>    
#include <stdlib.h>   

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char* filePath = argv[1];

    
    int fd = open(filePath, O_RDONLY);
    if (fd == -1) {
        perror("Ошибка при открытии файла");
        return EXIT_FAILURE;
    }

    const size_t bufferSize = 1024;
    char buffer[bufferSize];
    ssize_t bytesRead;

    
    while ((bytesRead = read(fd, buffer, bufferSize)) > 0) {
        ssize_t bytesWritten = 0;
        while (bytesWritten < bytesRead) {
            ssize_t result = write(STDOUT_FILENO, buffer + bytesWritten, bytesRead - bytesWritten);
            if (result == -1) {
                perror("Ошибка при записи в stdout");
                close(fd);
                return EXIT_FAILURE;
            }
            bytesWritten += result;
        }
    }

    if (bytesRead == -1) {
        perror("Ошибка при чтении файла");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    return 0;
}
