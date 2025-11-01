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

    // Open file for reading
    int fd = open(filePath, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        std::exit(EXIT_FAILURE);
    }

    // Redirect standard input (stdin) to this file
    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("Error redirecting stdin");
        close(fd);
        std::exit(EXIT_FAILURE);
    }

    // We can safely close original fd now
    close(fd);
}

int main(int argc, char** argv)
{
    // initialize the program based on the requirements
    initialize(argc, argv);

    // read the string 
    std::string input;
    std::cin >> input;
    
    // logic to get the reversed string
    std::string reversed(input.rbegin(), input.rend());
 
    // print to the console
    std::cout << reversed << std::endl;

    return 0;
}

