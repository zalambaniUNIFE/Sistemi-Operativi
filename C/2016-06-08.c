#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#define DIM_PATH 200
#define DIM_ARGOMENTO 80
#define DIM_DATA 20

static volatile sig_atomic_t count = 0;
void header(){
    printf("\nNumero di richieste servite: %d\n", count);
    exit(0);
}

int main(int argc, char* argv[]){
    int p1p2[2], pid, fd, status;
    char path[DIM_PATH], argomento[DIM_ARGOMENTO], data[DIM_DATA];
    
    if (argc != 2){
        printf ("Usage: trova_news <dir>\n");
        exit(1);
    }
    if (argv[1][0] != '/'){
        printf ("La directory deve essere assoluta\n");
        exit(2);
    }

    printf("Inserire argomento (\"fine\" per terminare):\n");
    scanf("%s", argomento);
    
    while(strcmp(argomento, "fine")){
        printf ("Inserisci data (YYYYMMDD): ");
        scanf("%s", data);
        sprintf (path, "%s/%s.txt", argv[1], data);
        if ((fd = open(path, O_RDONLY)) < 0){
            printf("Errore nell'apertura del file %s\n", path);
            continue;
        }
        else{
            close(fd);
            signal(SIGINT, header);
            if (pipe(p1p2) < 0){
                perror("p1p2 errore");
                exit(3);
            }
            pid = fork();
            if (pid < 0){
                perror("P0: fork P1");
                exit(4);
            }
            if (pid == 0){
                //figlio P1
                //P1: grep a P2
                signal (SIGINT, SIG_DFL);
                close(p1p2[0]);
                close(1);
                dup(p1p2[1]);
                close(p1p2[1]);

                execlp("grep", "grep", argomento, path, (char*)0);
                perror("P1: grep");
                exit(5);
            }
            pid = fork();
            if (pid < 0){
                perror("P0: fork P2");
                exit(8);
            }
            if (pid == 0){
                //figlio P2
                //P2: sort
                signal (SIGINT, SIG_DFL);
                close(p1p2[1]);
                close(0);
                dup(p1p2[0]);
                close(p1p2[0]);

                execlp("sort", "sort", "-r", "-n", (char*)0);
                perror("P2: sort");
                exit(7);
            }
            count++;
            close (p1p2[0]);
            close(p1p2[1]);
            wait(&status);
            wait(&status);
        }
        printf("Inserire argomento (\"fine\" per terminare):\n");
        scanf("%s", argomento);
    }
    printf("Numero di richieste servite: %d\n", count);
    return 0;
}