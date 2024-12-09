#include <fcntl.h>   // Per a open
#include <unistd.h>  // Per a read, write, close
#include <stdio.h>   // Per a perror
#include <stdlib.h>  // Per a exit

#define PERMISSIONS 00644 // rw-r--r--

int main(int argc, char *argv[]) {
    if (argc < 2) {  //Comprova que el programa ha rebut com a mínim un argument (a part del nom del programa).Si no és així, mostra un missatge d'error 
        exit(-1);
    }

    // Intenta crear un arxiu amb el nom especificat com a primer argument (argv[1]).
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, PERMISSIONS); // O_WRONLY: Obrir l'arxiu en mode només escriptura. O_CREAT: Crear l'arxiu si no existeix. O_TRUNC: Si l'arxiu ja existeix, es buida.
    if (fd == -1) { //Si open falla (per exemple, per manca de permisos o espai insuficient), retorna -1
        perror("Error creant l'arxiu");
        exit(-1);
    }

    char buffer; //Variable on es guardarà un caràcter llegit de l'entrada estàndard.
    ssize_t bytes_read, bytes_written; //Guardaran el nombre de bytes llegits i escrits respectivament.

    // Llegir caràcters d'entrada estàndard
    //Llegeix un caràcter (1 byte) de l'entrada estàndard (STDIN_FILENO) i l'emmagatzema a buffer. 
    while ((bytes_read = read(STDIN_FILENO, &buffer, 1)) > 0) { //read retorna el nombre de bytes llegits
        // Escriu el contingut de buffer a la sortida estàndard (STDOUT_FILENO).
        bytes_written = write(STDOUT_FILENO, &buffer, bytes_read);
        if (bytes_written == -1) { //Comprova si write falla (-1)
            perror("Error escrivint a la sortida estàndard");
            close(fd);
            exit(-1);
        }

        // Escriu el contingut de buffer a l'arxiu indicat pel descriptor fd.
        bytes_written = write(fd, &buffer, bytes_read);
        if (bytes_written == -1) { //Comprova si write falla (-1)
            perror("Error escrivint a l'arxiu");
            close(fd);
            exit(-1);
        }
    }

    //Si read retorna -1, indica que s'ha produït un error en la lectura de l'entrada estàndard i es mostra un missatge d'error.
    if (bytes_read == -1) {
        perror("Error llegint de l'entrada estàndard");
    }

    close(fd);//Tanca l'arxiu
    return 0;//el programa ha finalitzat
}

