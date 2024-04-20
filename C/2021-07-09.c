#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#define ROOT_PATH "/home/alice"
#define DIM_PATH 80
#define DIM_SPETTACOLO 120
#define DIM_N 20

static volatile sig_atomic_t counter = 0;

void handler(){
    printf ("\nNumero di richieste servite: %d\n", counter);
    exit(0);
}

int main(int argc, char* argv[]){
    int p1p2[2], p2p3[2];
    int pid, fd, status, n;
    char path[DIM_PATH], nomeSpettacolo[DIM_SPETTACOLO], str_n[DIM_N];

    if (argc != 2){
        printf ("Usage: trova_spettacoli <nomeTeatro>\n");
        exit(1);
    }
    sprintf(path, "%s/%s.txt", ROOT_PATH, argv[1]);
    fd = open(path, O_RDONLY, 644);
    if (fd < 0){
        printf ("Errore nell'apertura del file %s\n", path);
        exit(2);
    }
    close (fd);

    signal(SIGINT, handler);

    while(1){
        printf ("Inserisci il nome di uno spettacolo: ");
        scanf ("%s", nomeSpettacolo);
        printf ("Inserisci il numero di spettacoli da trovare: ");
        scanf ("%d", &n);

        if (n == 0){
            printf ("Termino il programma\n");
            break;
        }
        else if (n < 0){
            printf ("N deve essere un numero maggiore o uguale a 0\n");
            continue;
        }

        if (pipe(p1p2) < 0){
            perror ("Errore creazione p1p2");
            exit(3);
        }
        if (pipe(p2p3) < 0){
            perror ("Errore creazione p2p3");
            exit(4);
        }

        pid = fork();
        if (pid < 0){
            perror ("P0: fork P1");
            exit(5);
        }
        if (pid == 0){
            //figlio P1
            signal (SIGINT, SIG_DFL);
            //P1: grep nomeSpettacolo
            //chiudo p2p3
            close(p2p3[0]);
            close(p2p3[1]);
            //redirigo stdout
            close(p1p2[0]);
            
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);
            //exec
            execlp("grep", "grep", nomeSpettacolo, path, (char*)0);
            perror("P1: grep");
            exit(6);
        }//fine P1, torno a P0
        pid = fork();
        if (pid < 0){
            perror ("P0: fork P2");
            exit(7);
        }
        if (pid == 0){
            //figlio P2
            signal (SIGINT, SIG_DFL);
            //P2: sort spettacoli da più imminente a più lontano
            //chiudo output p1p2
            close(p1p2[1]);
            //chiudo input p2p3
            close(p2p3[0]);
            //redirigo stdin p1p2
            close(0);
            dup(p1p2[0]);
            close(p1p2[0]);
            //redirigo stdout p2p3
            close(1);
            dup(p2p3[1]);
            close(p2p3[1]);
            //exec di P2
            execlp("sort", "sort", "-n", (char*)0);
            perror ("P2: sort");
            exit(8);
        }//fine P2, torno a P0
        pid = fork();
        if (pid < 0){
            perror("P0: fork P3");
            exit(9);
        }
        if (pid == 0){
            // figlio P3
            signal (SIGINT, SIG_DFL);
            //P3: stampa N spettacoli
            //chiudo p1p2
            close(p1p2[0]);
            close(p1p2[1]);
            //redirigo stdin
            close(0);
            dup(p2p3[0]);
            close(p2p3[0]);
            
            close (p2p3[1]);

            sprintf(str_n, "%d", n);
            execlp("head", "head", "-n", str_n, (char*)0);
            perror("P3: head");
            exit(10);
        }//fine P3, torno a P0
        close (p1p2[0]);
        close (p1p2[1]);
        close (p2p3[0]);
        close (p2p3[1]);

        wait (&status);
        wait (&status);
        wait (&status);

        counter++;
    }
    printf ("Numero di richieste servite: %d\n", counter);
    return 0;
}