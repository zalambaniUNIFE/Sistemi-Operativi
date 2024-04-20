#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

static volatile sig_atomic_t count = 0;
void handler(){
    printf("\nNumero di richieste eseguite: %d\n", count);
    exit(0);
}

int main(int argc, char* argv[]){
    int p1p2[2], p2p3[2];
    int fd, pid, status;
    char tipologia[120];

    if (argc != 2){
        printf("Usage: verifica <fileBollette>\n");
        exit(1);
    }

    if (argv[1][0] != '/'){
        printf("%s deve essere un nome assoluto di file\n", argv[1]);
        exit(2);
    }

    if ((fd = open(argv[1], O_RDONLY)) < 0){
        printf("Errore nell'apertura del file %s", argv[1]);
        exit(3);
    }

    signal(SIGINT, handler);

    while(1){
        printf("Inserisci la tipologia: ");
        scanf("%s", tipologia);
        if (strcmp(tipologia, "esci") == 0)
            break;
        if (pipe(p1p2) < 0){
            perror("Errore p1p2");
            exit(4);
        }
        if (pipe(p2p3) < 0){
            perror("Errore p2p3");
            exit(5);
        }
        
        pid = fork();
        if (pid < 0){
            perror("P0: fork P1");
            exit(6);
        }
        if (pid == 0){
            signal(SIGINT, SIG_DFL);
            //figlio P1: grep
            close (p2p3[0]);
            close(p2p3[1]);
            //ridirigo stdout
            close(p1p2[0]);
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);

            execlp("grep", "grep", tipologia, argv[1], (char*)0);
            perror("P1: grep");
            exit(7);
        }
        
        pid = fork();
        if (pid < 0){
            perror ("P0: fork P2");
            exit(8);
        }
        if (pid == 0){
            signal(SIGINT, SIG_DFL);
            //figlio P2: grep (out su p2p3, in da p1p2)
            //ridirigo stdin di p1p2
            close (p1p2[1]);
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);
            //ridirig stdout di p2p3
            close(p2p3[0]);
            close(1);
            dup(p2p3[1]);
            close(p2p3[1]);

            execlp("grep", "grep", "-v", "pagato", (char*)0);
            perror("P2: grep");
            exit(9);
        }                       
        
        pid = fork();
        if (pid < 0){
            perror("P0: fork P3");
            exit(10);
        }
        if (pid == 0){
            signal(SIGINT, SIG_DFL);
            //figlio P3: sort
            close(p1p2[0]);
            close(p1p2[1]);
            //ridirigo stdin
            close(p2p3[1]);
            close(0);
            dup(p2p3[0]);
            close(p2p3[0]);

            execlp("sort", "sort", "-r", "-n", (char*)0);
            perror("P3: sort");
            exit(11);
        }

        close(p1p2[0]);
        close(p1p2[1]);
        close(p2p3[0]);
        close(p2p3[1]);
        wait(&status);
        wait(&status);
        wait(&status);
        count++;
    }
    printf("Numero di richieste eseguite: %d\n", count);
    return 0;
}