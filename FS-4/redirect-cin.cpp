#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>   
#include <fcntl.h>  
#include <cstdlib>  

void initialize(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        std::exit(EXIT_FAILURE);
    }

    const char* filePath = argv[1];

    int fd = open(filePath, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        std::exit(EXIT_FAILURE);
    }

    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("Error redirecting stdin");
        close(fd);
        std::exit(EXIT_FAILURE);
    }
    close(fd);
}

int main(int argc, char** argv)
{
    initialize(argc, argv);

 
    std::string input;
    std::cin >> input;
    
    
    std::string reversed(input.rbegin(), input.rend());
 
    
    std::cout << reversed << std::endl;

    return 0;
}

