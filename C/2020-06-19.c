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
    printf("\nNumero di richieste servite: %d\n", count);
    exit(0);
}

int main(int argc, char* argv[]){
    int fd, pid, status;
    int p1p2[2];
    char categoria[120], data[20], path[200];

    if (argc != 2){
        printf("Usage: trova_video <dir>\n");
        exit(1);
    }

    if (argv[1][0] == '/'){
        printf("%s deve essere un nome relativo di directory\n", argv[1]);
        exit(2);
    }

    signal(SIGINT, handler);

    while(1){
        printf("Inserisci una categoria: ");
        scanf("%s", categoria);
        printf("Inserisci data (YYYMM): ");
        scanf("%s", data);
        sprintf(path, "%s/%s.txt", argv[1], data);
        if((fd = open(path, O_RDONLY)) < 0){
            printf("Errore nell'apertura del file %s", path);
            exit(3);
        }
        
        close(fd);
        
        if (pipe(p1p2) < 0){
            perror("p1p2: pipe");
            exit(4);
        }
        pid = fork();
        if (pid < 0){
            perror("P0: fork P1");
            exit(5);
        }
        if (pid == 0){
            //figlio P1: grep
            signal(SIGINT, SIG_DFL);
            close(p1p2[0]);
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);

            execlp("grep", "grep", categoria, path, (char*)0);
            perror("P1: error");
            exit(6);
        }
        pid = fork();
        if (pid < 0){
            perror("P0: fork P2");
            exit(7);
        }
        if (pid == 0){
            //figlio P2: sort
            signal(SIGINT, SIG_DFL);
            close(p1p2[1]);
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);

            execlp("sort", "sort", "-r", "-n", (char*)0);
            perror("P2: sort");
            exit(8);
        }
        close(p1p2[0]);
        close(p1p2[1]);
        wait(&status);
        wait(&status);
        count++;
    }
    printf("Numero di richieste servite: %d\n", count);
    return 0;
}