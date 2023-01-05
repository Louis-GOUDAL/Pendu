#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <time.h>
#include <ctype.h>

void ma_partie(int sig)
{
    printf("La partie commence !\n");
}

int main(int argc, char *argv[])
{
    //demande à l'utilisateur de saisir son prénom
    char prenom[20];
    printf("Entrez votre prenom : ");
    scanf("%s", prenom);

    //ouvre la boite aux lettres
    int boite = msgget(1234, 0);
    if(boite == -1)
    {
        perror("msgget");printf("La partie commence !\n");
        return 1;
    }

    //envoie le prénom et le PID au serveur
    struct msgbuf
    {
        long mtype;
        char mtext[20];
    } msg;
    msg.mtype = getpid();
    sprintf(msg.mtext, "%s", prenom);
    //envoie le message
    int ret = msgsnd(boite, &msg, sizeof(msg.mtext), 0);
    if (ret == -1)
    {
        perror("msgsnd");
        return 1;
    }

    //reception du signal SIGUSR1
    signal(SIGUSR1, ma_partie);
    pause();
    
    return 0;
}

