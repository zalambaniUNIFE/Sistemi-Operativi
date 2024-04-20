#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

static volatile sig_atomic_t count = 0;
void header(){
    printf("\nNumero di richieste servite: %d\n", count);
    exit(0);
}

int main(int argc, char* argv[]){
    int fd, N, pid, status;
    int p1p2[2], p2p3[2];
    char id[100], data[80], path[120], str_n[20];
    
    if (argc != 2){
        printf("Usage: tavoli_prenotati <dir>\n");
        exit(1);
    }
    if (argv[1][0] == '/'){
        printf("%s deve essere un nome relativo di directory\n", argv[1]);
        exit(2);
    }

    if((fd = open(argv[1], __O_DIRECTORY)) < 0){
        printf("Ertrore nell'apertura della directory\n");
        exit(3);
    }
    close(fd);

    signal(SIGINT,header);

    while(1){
        printf("Inserisci l'identificativo del ristorante: ");
        scanf("%s", id);
        printf("Inserisci la data: ");
        scanf("%s", data);
        printf("Inserisci N risultati da mostrare: ");
        scanf("%d", &N);
        sprintf(path, "%s/%s.txt", argv[1], data);
        if ((fd = open(path, O_RDONLY)) < 0){
            printf("Errore apertura file %s\n", path);
            exit(4);
        }
        close(fd);
        if (pipe(p1p2) < 0){
            perror("p1p2 errore");
            exit(5);
        }
        if (pipe(p2p3) < 0){
            perror("p2p3 errore");
            exit(6);
        }
        pid = fork();
        if (pid < 0){
            perror("P0: fork P1");
            exit(7);
        }
        if (pid == 0){
            //figlio P1: grep
            signal(SIGINT, SIG_DFL);
            close(p2p3[0]);
            close(p2p3[1]);

            close(p1p2[0]);
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);

            execlp("grep", "grep", id, path, (char*)0);
            perror("P1: grep");
            exit(8);
        }
        pid = fork();
        if (pid < 0){
            perror("P0: fork P2");
            exit(9);
        }
        if (pid == 0){
            //figlio P2: sort
            signal(SIGINT, SIG_DFL);
            close(p1p2[1]);
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);

            close(p2p3[0]);
            close(1);
            dup(p2p3[1]);
            close(p2p3[1]);

            execlp("sort", "sort", "-n", "-r", (char*)0);
            perror("P2: sort");
            exit(10);
        }
        pid = fork();
        if (pid < 0){
            perror("P0: fork P3");
            exit(11);
        }
        if (pid == 0){
            //figlio P3: head
            signal(SIGINT, SIG_DFL);
            close(p1p2[0]);
            close(p1p2[1]);

            close(p2p3[1]);
            close(0);
            dup(p2p3[0]);
            close(p2p3[0]);

            sprintf (str_n, "%d", N);

            execlp("head", "head", "-n", str_n, (char*)0);
            perror("P3: head");
            exit(12);
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
    printf("Numero di richieste servite: %d", count);
    return 0;
}