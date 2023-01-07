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

int fin_de_jeu = 0;

struct msgbuf
{
    long mtype;
    char mtext[20];
} msg;

void game_over()
{
    fin_de_jeu = 1;
    system("clear");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼███▀▀▀██┼███▀▀▀███┼███▀█▄█▀███┼██▀▀▀┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼█┼┼┼██┼██┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██┼┼┼▄▄▄┼██▄▄▄▄▄██┼██┼┼┼▀┼┼┼██┼██▀▀▀┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██┼┼┼┼██┼██┼┼┼┼┼██┼██┼┼┼┼┼┼┼██┼██┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼███▄▄▄██┼██┼┼┼┼┼██┼██┼┼┼┼┼┼┼██┼██▄▄▄┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼███▀▀▀███┼▀███┼┼██▀┼██▀▀▀┼██▀▀▀▀██▄┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██┼┼┼┼┼██┼┼┼██┼┼██┼┼██┼┼┼┼██┼┼┼┼┼██┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██┼┼┼┼┼██┼┼┼██┼┼██┼┼██▀▀▀┼██▄▄▄▄▄▀▀┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██┼┼┼┼┼██┼┼┼██┼┼█▀┼┼██┼┼┼┼██┼┼┼┼┼██┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼███▄▄▄███┼┼┼─▀█▀┼┼─┼██▄▄▄┼██┼┼┼┼┼██▄┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼████▄┼┼┼▄▄▄▄▄▄▄┼┼┼▄████┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼▀▀█▄█████████▄█▀▀┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼█████████████┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██▀▀▀███▀▀▀██┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼██┼┼┼███┼┼┼██┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼█████▀▄▀█████┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼███████████┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼▄▄▄██┼┼█▀█▀█┼┼██▄▄▄┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼▀▀██┼┼┼┼┼┼┼┼┼┼┼██▀▀┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼▀▀┼┼┼┼┼┼┼┼┼┼┼▀▀┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
    printf("┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼┼\n");
}

void bravo()
{
    while(1)
    {
        sleep(1);
        system("clear");
        printf("                    ,,,                                    \n");
        printf("                   !!!!\\\\\\\\                            \n");
        printf("                 '`!_  ||||                                \n");
        printf("                  ` \\`-'''|                               \n");
        printf("                   `\\   /                                 \n");
        printf("                     )\\  \\                               \n");
        printf("                    /  \\  \\                              \n");
        printf("                        \\                                 \n");
        printf("                                                           \n");
        printf("88                                                         \n");
        printf("88                                                         \n");
        printf("88                                                         \n");
        printf("88,dPPYba,  8b,dPPYba, ,adPPYYba, 8b       d8  ,adPPYba,   \n");
        printf("88P'    '8a 88P'   'Y8 ''     `Y8 `8b     d8' a8'     '8a  \n");
        printf("88       d8 88         ,adPPPPP88  `8b   d8'  8b       d8  \n");
        printf("88b,   ,a8' 88         88,    ,88   `8b,d8'   '8a,   ,a8'  \n");
        printf("8Y'Ybbd8''  88         `'8bbdP'Y8     '8'      `'YbbdP''   \n");

        sleep(1);
        system("clear");
        printf("                    ////                                   \n");
        printf("                   !!!!  _      \\\\\\\\                   \n");
        printf("                   !   -'/   _  ||||                       \n");
        printf("                    \\   /    \\`-'''|                     \n");
        printf("                     \\  |     \\   /                      \n");
        printf("                     )  |      \\  \\                      \n");
        printf("                    /   |       \\  \\                     \n");
        printf("                                 \\                        \n");
        printf("                                                           \n");
        printf("88                                                         \n");
        printf("88                                                         \n");
        printf("88                                                         \n");
        printf("88,dPPYba,  8b,dPPYba, ,adPPYYba, 8b       d8  ,adPPYba,   \n");
        printf("88P'    '8a 88P'   'Y8 ''     `Y8 `8b     d8' a8'     '8a  \n");
        printf("88       d8 88         ,adPPPPP88  `8b   d8'  8b       d8  \n");
        printf("88b,   ,a8' 88         88,    ,88   `8b,d8'   '8a,   ,a8'  \n");
        printf("8Y'Ybbd8''  88         `'8bbdP'Y8     '8'      `'YbbdP''   \n");
    }
}

void debut_partie()
{
    printf("\nLancement de la partie !\n");
}

void mon_tour()
{
    printf("\nC'est à vous de jouer !\n");
}

int main(int argc, char *argv[])
{
    //demande à l'utilisateur de saisir son prénom
    char prenom[20];
    char ma_lettre[20];
    printf("Entrez votre prenom : ");
    scanf("%s", prenom);

    //création d'une boite aux lettre pour l'initialisation
    int boite_init = msgget(1234, 0);
    if(boite_init == -1)
    {
        perror("msgget");
        return 1;
    }

    //création d'une boite aux lettre pour les tours
    int boite_tour = msgget(2345, 0);
    if(boite_tour == -1)
    {
        perror("msgget");
        return 1;
    }

    //envoie le prénom et le PID au serveur
    msg.mtype = getpid();
    sprintf(msg.mtext, "%s", prenom);
    //envoie le message
    int ret = msgsnd(boite_init, &msg, sizeof(msg.mtext), 0);
    if (ret == -1)
    {
        perror("msgsnd");
        return 1;
    }

    //on attend que le serveur envoie un signal pour dire que la partie commence
    signal(SIGUSR1, debut_partie);
    pause();

    //si on recoit SIGUSR2, on appelle la fonction game_over
    signal(SIGUSR2, game_over);
    
    while(fin_de_jeu == 0)
    {  
        signal(SIGUSR1, mon_tour);
        pause();
        
        if(fin_de_jeu == 1)
        {
            break;
        }

        //Demande la lettre au joueur
        printf("Entrez une lettre ou un mot : ");
        scanf("%s", ma_lettre);

        //envoie la lettre et le PID au serveur
        msg.mtype = getpid();
        sprintf(msg.mtext, "%s", ma_lettre);

        //envoie le message dans la boite aux lettres
        int ret = msgsnd(boite_tour, &msg, sizeof(msg.mtext), 0);
        if (ret == -1)
        {
            perror("msgsnd");
            return 1;
        }
        
    }
    
    
    return 0;
}

