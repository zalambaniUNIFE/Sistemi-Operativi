#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

static volatile sig_atomic_t byteCount = 0;

void handler(){
    printf ("\nNumero di byte ricevuti da P2: %d\n", byteCount);
    exit(0);
}

int main(int argc, char* argv[]){
    int p0p1[2], p0p2[2];
    int fd, pid, status, nread, nwrite;
    char path[120], ref[100], res[1024];

    if (argc != 3){
        printf ("Usage: test_eseguiti <dir> <data>\n");
        exit(1);
    }

    if (argv[1][0] == '/'){
        printf ("<dir> è un nome relativo di directory\n");
        exit(2);
    }
    
    sprintf(path, "%s/%s.txt", argv[1], argv[2]);
    if ((fd = open(path, O_RDONLY)) < 0){
        printf("Errore nell'apertura del file %s\n", path);
        exit(3);
    }
    close (fd);
            
    signal(SIGINT, handler);
    
    if (pipe(p0p1) < 0){
        perror("p0p1 error");
        exit(4);
    }
    if (pipe(p0p2) < 0){
        perror("p0p2 error");
        exit(5);
    }
    
    pid = fork();
    if (pid < 0){
        perror ("P0: fork P1");
        exit(6);
    }
    if (pid == 0){
        //figlio P1
        //P1: input del nome da P0 e output su P2
        //chiudo scrittura p0p1
        close(p0p1[1]);
        //chiudo lettura p0p2
        close (p0p2[0]);
        signal (SIGINT, SIG_DFL);
        while (1){
            nread = read(p0p1[0], ref, sizeof(ref) - 1);
            if (nread < 0){
                perror ("P1: read");
                exit(7);
            }
            ref[nread] = '\0';

            pid = fork();
            if (pid < 0){
                printf("P1: fork P2");
                exit(8);
            }
            if (pid == 0){
                //nipote P2
                //P2: grep nome persona
                //chiudo lettura p0p1
                close(p0p1[0]);
                //redirigo stdout
                close(1);
                dup (p0p2[1]);
                close(p0p2[1]);
                //exec
                execlp("grep", "grep", ref, path, (char*)0);
                perror ("P2: grep");
                exit(9);
            }
            wait(&status);
        }
        
    }
    //chiudo lettura p0p1
    close (p0p1[0]);
    //chiudo scrittura p0p2
    close (p0p2[1]);
    
    while(1){
        printf("Inserisci il nome del refertatore: ");
        scanf("%s", ref);
        nwrite = write(p0p1[1], ref, strlen(ref) + 1);
        if (nwrite != (int)(strlen(ref) + 1)){
            perror ("P0: write");
            exit(10);
        }
        nread = read(p0p2[0], res, sizeof(res) - 1);
        if (nread < 0){
            perror("P0: read");
            exit(11);
        }
        res[nread] = '\0';
        printf("%s", res);

        byteCount += nread;
    }
    return 0;
}