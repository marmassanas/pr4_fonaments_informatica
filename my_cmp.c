#include <fcntl.h>   // Per a open
#include <unistd.h>  // Per a read, close
#include <stdio.h>   // Per a perror
#include <stdlib.h>  // Per a exit

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Ús: %s <arxiu1> <arxiu2>\n", argv[0]);
        exit(-1);
    }

    int fd1 = open(argv[1], O_RDONLY);
    if (fd1 == -1) {
        perror("Error obrint el primer arxiu");
        exit(-1);
    }

    int fd2 = open(argv[2], O_RDONLY);
    if (fd2 == -1) {
        perror("Error obrint el segon arxiu");
        close(fd1);
        exit(-1);
    }

    char buf1, buf2;
    ssize_t bytes_read1, bytes_read2;
    int byte_position = 0, line_number = 1;

    // Comparar byte a byte
    while ((bytes_read1 = read(fd1, &buf1, 1)) > 0 &&
           (bytes_read2 = read(fd2, &buf2, 1)) > 0) {
        byte_position++;

        // Comprovar si hi ha una diferència
        if (buf1 != buf2) {
            printf("Diferència trobada al byte %d, línia %d\n",
                   byte_position, line_number);
            close(fd1);
            close(fd2);
            return 0;
        }

        // Comptar línies
        if (buf1 == '\n') {
            line_number++;
        }
    }

    if (bytes_read1 != bytes_read2) {
        printf("Els arxius tenen diferents longituds.\n");
    }

    close(fd1);
    close(fd2);
    return 0;
}







