#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

static volatile sig_atomic_t count = 0;
void header(){
    printf("\nNumero di ricerche effettuate: %d\n", count);
    exit(0);
}

int main(int argc, char* argv[]){
    int p1p2[2];
    int fd, pid, status;
    char cognome[100], prefisso[20];
    if (argc != 2){
        printf("Usage: trovaParola <fileAnagrafica>\n");
        exit(1);
    }
    if ((fd = open(argv[1], O_RDONLY)) < 0){
        printf("Errore nell'apertura del file %s\n", argv[1]);
        exit(2);
    }
    close(fd);
    signal(SIGINT, header);

    while (1){
        printf("Inserire il cognome di un cliente: ");
        scanf ("%s", cognome);
        if (strcmp(cognome, "fine") == 0)
            break;
        printf("Inserire il prefisso telefonico: ");
        scanf ("%s", prefisso);
        if (strcmp(cognome, "fine") == 0)
            break;
       
        if (strcmp(prefisso, "fine") == 0)
            break;

        if (pipe(p1p2) < 0){
            perror ("p1p2 pipe");
            exit(3);
        }

        pid = fork();
        if (pid < 0){
            perror("P0: fork P1");
            exit(4);
        }
        if (pid == 0){
            //figlio P1 grep
            signal(SIGINT, SIG_DFL);

            close(p1p2[0]);
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);

            execlp("grep", "grep", cognome, argv[1], (char*)0);
            perror ("P1: grep");
            exit(5);
        }
        pid = fork();
        if (pid < 0){
            perror("P0: fork P2");
            exit(6);
        }
        if (pid == 0){
            //figlio P2 grep
            signal(SIGINT, SIG_DFL);

            close(p1p2[1]);
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);

            execlp("grep", "grep", prefisso, (char*)0);
            perror ("P2: grep");
            exit(7);
        }
        close (p1p2[0]);
        close (p1p2[1]);
        wait(&status);
        wait (&status);
        count++;
    }
    printf("Numero di ricerche effettuate: %d\n", count);
    return 0;
}