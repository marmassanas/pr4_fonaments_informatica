#include <fcntl.h>   // Per a open
#include <unistd.h>  // Per a read, close
#include <stdio.h>   // Per a perror
#include <stdlib.h>  // Per a exit

int main(int argc, char *argv[]) {
    if (argc < 3) { //Comprova que s'han passat almenys dos arguments a la línia d'ordres (els noms dels fitxers).
        fprintf(stderr, "Ús: %s <arxiu1> <arxiu2>\n", argv[0]);
        exit(-1);
    }

    int fd1 = open(argv[1], O_RDONLY); //Obre el primer fitxer en mode només lectura
    if (fd1 == -1) { //Comprova si hi ha hagut un error en obrir el primer fitxer.
        perror("Error obrint el primer arxiu");
        exit(-1);
    }

    int fd2 = open(argv[2], O_RDONLY); //Obre el segon fitxer en mode només lectura
    if (fd2 == -1) { //Comprova si hi ha hagut un error en obrir el segon fitxer.
        perror("Error obrint el segon arxiu");
        close(fd1);
        exit(-1);
    }

    char buf1, buf2; //Variables per emmagatzemar un byte de cada fitxer.
    ssize_t bytes_read1, bytes_read2; //Variables per emmagatzemar el nombre de bytes llegits de cada fitxer.
    int byte_position = 0, line_number = 1; //Variables per rastrejar la posició del byte i el número de línia.

    // Comparar byte a byte
    while (1) { //Llegeix 1 byte del segon fitxer i l'emmagatzema a buf2. Retorna el nombre de bytes llegits (normalment 1 si tot va bé, 0 si error).
        bytes_read1 = read(fd1, &buf1, 1);  //Llegeix 1 byte del primer fitxer i l'emmagatzema a buf1. Retorna el nombre de bytes llegits (normalment 1 si tot va bé, 0 si error).
        bytes_read2 = read(fd2, &buf2, 1); //Llegeix 1 byte del segon fitxer i l'emmagatzema a buf2. Retorna el nombre de bytes llegits (normalment 1 si tot va bé, 0 si error).

        // Comprovar si tots dos arxius han arribat al final
        if (bytes_read1 == 0 && bytes_read2 == 0) {
            // Són iguals si cap arxiu té bytes restants
            break;
        }

        // Si un arxiu té més bytes que l'altre
        if (bytes_read1 != bytes_read2) {
            printf("Els arxius tenen diferents longituds.\n");
            close(fd1);
            close(fd2);
            return 0;
        }

        // Comprovar si hi ha una diferència
        if (buf1 != buf2) {
            printf("Diferència trobada al byte %d, línia %d\n",
                   byte_position, line_number);
            close(fd1);
            close(fd2);
            return 0;
            //Comprova si els bytes actuals dels dos fitxers són diferents:Si són diferents, mostra un missatge amb la posició del byte i el número de línia.
            //Tanca els fitxers (close(fd1); close(fd2);) i surt del programa (return 0)
        }

        // Si el byte llegit és un salt de línia (\n), incrementa el comptador de línies.
        if (buf1 == '\n') { 
            line_number++;
        }

        byte_position++; //Incrementa la posició del byte processat
    }


    //Tanca els descriptors dels fitxers per alliberar recursos.
    close(fd1); 
    close(fd2);
    return 0; //Retorna 0 per indicar que el programa ha acabat correctament (a menys que ja hagi trobat una diferència).
}







