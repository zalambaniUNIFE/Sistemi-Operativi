#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#define DIM 80
#define DIM_DATA 32
#define DIM_STRN 20
#define ROOT_PATH "/var/local/ticket"

static volatile sig_atomic_t count = 0;

void handler() {
	printf("\nNumero di richieste servite: %d\n", count);
	exit(0);
}


int main(int argc, char* argv[]){
    int pid, status, N, fd, gg, mm, yyyy;
    int p1p2[2], p2p3[2];
    char path[DIM], data[DIM_DATA], str_n[DIM_STRN];
    
    if (argc != 3){
        printf ("Usage: trova_biglietti <destinazione> <N>\n");
        exit(1);
    }
    N = atoi(argv[2]);
    if (N <= 0){
        printf ("%d must be a positive integer\n", N);
        exit(2);
    }
    sprintf(path, "%s/%s.txt", ROOT_PATH, argv[1]);
    fd = open(path, O_RDONLY);
    if (fd < 0){
        perror("open");
        exit(3);
    }
    close (fd);

    signal(SIGINT, handler);

    while (1){
        printf ("Inserisci giorno, mese ed anno (GG/MM/YYYY): ");
        scanf ("%d%d%d", &gg, &mm, &yyyy);
        
        if (gg == -1 || mm == -1 | yyyy == -1){
            printf ("Termino il programma\n");
            break;
        }
        else if (gg < 0 || mm < 0 || yyyy < 0){
            printf ("GG/MM/YYYY è composto da interi positivi\n");
            continue;
        }
        else if (gg < 1 || gg > 31){
            printf ("GG è compreso tra 1 e 31\n");
            continue;
        }
        else if (mm < 1 || mm > 12){
            printf ("MM è compreso tra 1 e 12\n");
            continue;
        }

        sprintf(data, "%02d%02d%04d", gg, mm, yyyy);
        
        // creo la pipe p1p2
        if (pipe(p1p2) < 0){
            perror ("p1p2 error");
            exit (4);
        }

        // creo la piepe p2p3
        if (pipe(p2p3) < 0){
            perror("p2p3 error");
            exit (5);
        }

        pid = fork();
        if (pid < 0){
            perror ("P0: fork P1");
            exit(6);
        }
        if (pid == 0){
            // figlio P1
            signal (SIGINT, SIG_DFL);
            //P1: input offerte per data
            //p2p3 non mi serve
            close (p2p3[0]);
            close (p2p3[1]);

            close(p1p2[0]);
            
            close(1);
            dup(p1p2[1]);
            close(p1p2[1]);
            
            //exec di P1
            execlp("grep", "grep", data, path, (char*)0);
            perror ("P1: grep");
            exit(7);
        } //fine P1, torno al padre P0

        pid = fork();
        if (pid < 0){
            perror("P0: fork P2");
            exit(8);
        }
        if (pid == 0){
            //figlio P2
            signal (SIGINT, SIG_DFL);
            //P2: ordinare per prezzo crescente le offerte
            //chiudo output p1p2
            close (p1p2[1]);
            //chiudo input p2p3
            close (p2p3[0]);
            //redirigo stdin
            close(0);
            dup(p1p2[0]);
            close (p1p2[0]);
            //redirigo stdout
            close (1);
            dup (p2p3[1]);
            close (p2p3[1]);

            //exec di P2
            execlp("sort", "sort", "-n", (char*)0);
            perror("P2: sort");
            exit(9);
        }//fine figlio P2, torno al padre P0

        pid = fork();
        if (pid < 0){
            perror ("P0: fork P3");
            exit(10);
        }
        if (pid == 0){
            //figlio P3
            signal (SIGINT, SIG_DFL);
            //P3: output delle N migliori offerte
            //chiudo p1p2
            close (p1p2[0]);
            close (p1p2[1]);
            //redirigo stdin
            close (0);
            dup(p2p3[0]);
            //chiudo p2p3
            close (p2p3[0]);
            close (p2p3[1]);
            
            sprintf (str_n, "%d", N);

            execlp("head", "head", "-n", str_n, (char*)0);
            perror("P3: head");
            exit(11);
        }//fine figlio P3, torno al padre P0
        //chiudo le pipe
        close (p1p2[0]);
        close (p1p2[1]);
        close (p2p3[0]);
        close (p2p3[1]);

        wait (&status);
        wait (&status);
        wait (&status);

        count++;
    }
    printf("Numero di richieste servite: %d\n", count);
    return 0;
}