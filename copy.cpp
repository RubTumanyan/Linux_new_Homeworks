#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>

int main(int argc, char* argv[]){
    if(argc != 3){
        std::cerr << "Usage: " << argv[0] << " source destination\n";
        return EXIT_FAILURE;
    }

    int src = open(argv[1], O_RDONLY);
    if(src < 0){ perror("open source"); return EXIT_FAILURE; }

    int dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(dst < 0){ perror("open destination"); close(src); return EXIT_FAILURE; }

    struct stat st{};
    if(fstat(src, &st) < 0){ perror("fstat"); close(src); close(dst); return EXIT_FAILURE; }

    off_t logical = 0, physical = 0;
    const size_t bufsize = 64*1024;
    char buf[bufsize];

    off_t offset = 0;
    while(offset < st.st_size){
        off_t data_offset = lseek(src, offset, SEEK_DATA);
        if(data_offset == -1) data_offset = st.st_size;
        off_t hole_offset = lseek(src, offset, SEEK_HOLE);
        if(hole_offset == -1) hole_offset = st.st_size;

        if(offset < data_offset
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>

int main(int argc, char* argv[]){
    if(argc != 3){
        std::cerr << "Usage: " << argv[0] << " source destination\n";
        return EXIT_FAILURE;
    }

    int src = open(argv[1], O_RDONLY);
    if(src < 0){ perror("open source"); return EXIT_FAILURE; }

    int dst = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(dst < 0){ perror("open destination"); close(src); return EXIT_FAILURE; }

    struct stat st{};
    if(fstat(src, &st) < 0){ perror("error"); close(src); close(dst); return EXIT_FAILURE; }

    off_t logical = 0, physical = 0;
    const size_t bufsize = 64*1024;
    char buf[bufsize];

    off_t offset = 0;
    while(offset < st.st_size){
        off_t data_offset = lseek(src, offset, SEEK_DATA);
        if(data_offset == -1) data_offset = st.st_size;
        off_t hole_offset = lseek(src, offset, SEEK_HOLE);
        if(hole_offset == -1) hole_offset = st.st_size;

        if(offset < data_offset){
            off_t hole_len = data_offset - offset;
            if(lseek(dst, hole_len, SEEK_CUR) == -1){ perror("lseek dst"); close(src); close(dst); return EXIT_FAILURE; }
            logical += hole_len;
            offset += hole_len;
        }

        if(data_offset < hole_offset){
            off_t to_copy = hole_offset - data_offset;
            off_t copied = 0;
            while(copied < to_copy){
                ssize_t n = read(src, buf, std::min(bufsize, (size_t)(to_copy - copied)));
                if(n <= 0){ perror("read"); close(src); close(dst); return EXIT_FAILURE; }
                ssize_t w = write(dst, buf, n);
                if(w != n){ perror("write"); close(src); close(dst); return EXIT_FAILURE; }
                copied += n;
            }
            logical += to_copy;
            physical += to_copy;
            offset = hole_offset;
        }
    }

    std::cout << "Successfully copied " << logical << " bytes (data: " << physical << ", hole: " << logical - physical << ").\n";
    close(src);
    close(dst);
    return EXIT_SUCCESS;
}

