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
#include <locale.h>

//idée : les clients envoient les signal quand ils se lancent
//le serveur renvoi un signal quand le nombre de clients voulus est connectés
//la boite aux lettres peut alors s'ouvrir

int main(int argc, char *argv[])
{

    //création d'une boite aux lettres
    int boite = msgget(1234, IPC_CREAT | 0666);
    if (boite == -1)
    {
        perror("msgget");
        return 1;
    }

    //demander le nombre de joueur tant qu'il est inférieur à 3
    int nbJoueur = 0;
    while (nbJoueur < 1)
    {
        printf("Entrez le nombre de joueur (supérieur à 2): ");
        scanf("%d", &nbJoueur);
    }

    printf("\nEn attente de connexion des joueurs...\n\n");

    //tableau qui stocke le PID
    int tabPid[nbJoueur];
    //tableau qui stocke le prénom
    char tabPrenom[nbJoueur][20];
    
    //boucle jusqu'à ce que le tableau soit rempli
    int i = 0;
    while (i < nbJoueur)
    {
        //réception du message
        struct msgbuf
        {
            long mtype;
            char mtext[20];
        };
        struct msgbuf msg;
        int ret = msgrcv(boite, &msg, sizeof(msg.mtext), 0, 0);
        if (ret == -1)
        {
            perror("msgrcv");
            return 1;
        }

        //stocker le PID et le nom
        tabPid[i] = msg.mtype;
        strcpy(tabPrenom[i], msg.mtext);

        //afficher le nom du joueur et son PID
        printf("Joueur n°%d : %s\n", i+1, tabPrenom[i]);

        i++;
    }

    printf("\nTous les joueurs sont connectés !\n\n");

    //récupérer un mot dans le fichier dico.txt en utf8
    setlocale(LC_ALL, "fr_FR.UTF-8");
    FILE *fichier = fopen("dico.txt", "r");
    if (fichier == NULL)
    {
        perror("fopen");
        return 1;
    }

    //compter le nombre de mot dans le fichier
    int nbMot = 0;
    char mot[20];
    while (fgets(mot, 20, fichier) != NULL)
    {
        nbMot++;
    }

    //choisir un mot de taille supérieur à 5 au hasard dans dico.txt
    srand(time(NULL));
    int nbAleatoire = rand() % nbMot;
    int tailleMot = 0;

    while (tailleMot < 5)
    {
        fseek(fichier, 0, SEEK_SET);
        for (int i = 0; i < nbAleatoire; i++)
        {
            fgets(mot, 20, fichier);
        }
        tailleMot = strlen(mot) - 2;
        nbAleatoire = rand() % nbMot;
    }

    //mettre le mot en majuscule
    for (int i = 0; i < strlen(mot); i++)
    {
        mot[i] = toupper(mot[i]);
    }

    //stocker le mot dans une variable en remplacer les lettres par des "_"
    char motCache[tailleMot-1];
    for (int i = 0; i < tailleMot; i++)
    {
        motCache[i] = '_';
    }

    //afficher le mot à deviner
    printf("Le mot de %d lettres à deviner est : %s !!!\n",tailleMot, motCache);
    //afficher le mot
    printf("Le mot est : %s\n", mot);

    //envoi un signal aux joueurs pour dire que la partie à commencer
    for (int i = 0; i < nbJoueur; i++)
    {
        kill(tabPid[i], SIGUSR1);
    }


    return 0;
}