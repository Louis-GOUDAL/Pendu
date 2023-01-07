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

int fin_de_partie = 0;

void pendu(int numero)
{
    switch (numero)
    {
    case 0:
        printf(" _______\n");
        printf(" |     |\n");
        printf(" |     \n");
        printf(" |     \n");
        printf(" |     \n");
        printf(" |     \n");
        printf("_|___\n");
        break;
    
    case 1:
        printf(" _______\n");
        printf(" |     |\n");
        printf(" |     O\n");
        printf(" |     \n");
        printf(" |     \n");
        printf(" |     \n");
        printf("_|___\n");
        break;
    
    case 2:
        printf(" _______\n");
        printf(" |     |\n");
        printf(" |     O\n");
        printf(" |     |\n");
        printf(" |     \n");
        printf(" |     \n");
        printf("_|___\n");
        break;

    case 3: 
        printf(" _______\n");
        printf(" |     |\n");
        printf(" |     O\n");
        printf(" |    /|\n");
        printf(" |     \n");
        printf(" |     \n");
        printf("_|___\n");
        break;

    case 4:
        printf(" _______\n");
        printf(" |     |\n");
        printf(" |     O\n");
        printf(" |    /|\\\n");
        printf(" |     \n");
        printf(" |     \n");
        printf("_|___\n");
        break;

    case 5: 
        printf(" _______\n");
        printf(" |     |\n");
        printf(" |     O\n");
        printf(" |    /|\\\n");
        printf(" |    / \n");
        printf(" |     \n");
        printf("_|___\n");
        break;
    
    case 6: 
        printf(" _______\n");
        printf(" |     |\n");
        printf(" |     O\n");
        printf(" |    /|\\\n");
        printf(" |    / \\\n");
        printf(" |     \n");
        printf("_|___\n");
        fin_de_partie = 1;
        break;

    default:
        break;
    }
}
struct msgbuf
{
    long mtype;
    char mtext[20];
};
struct msgbuf msg;

int main(int argc, char *argv[])
{

    //definition du nbErreur
    int nbErreur = 0;

    //création d'une boite aux lettre pour l'initialisation
    int boite_init = msgget(1234, IPC_CREAT | 0666);
    if (boite_init == -1)
    {
        perror("msgget");
        return 1;
    }

    //création d'une boite aux lettre pour les tours
    int boite_tour = msgget(2345, IPC_CREAT | 0666);
    if (boite_tour == -1)
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
        int ret = msgrcv(boite_init, &msg, sizeof(msg.mtext), 0, 0);
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

    //envoi un signal aux joueurs pour dire que la partie à commencé
    for (int i = 0; i < nbJoueur; i++)
    {
        if (kill(tabPid[i], SIGUSR1) == -1)
        {
            perror("Error in kill");
        } 
    }

    while(fin_de_partie == 0)
    {
        for (int i = 0; i < nbJoueur; i++)
        {
            //On attend que les joueurs soient prêts
            sleep(3);
            //On envoi un signal au joueur pour dire qu'il doit jouer
            if (kill(tabPid[i], SIGUSR1) == -1)
            {
                perror("Error in kill");
            } 

            //réception de la lettre du joueur
            int ret = msgrcv(boite_tour, &msg, sizeof(msg.mtext), 0, 0);
            if (ret == -1)
            {
                perror("msgrcv");
                return 1;
            }

            //effacer l'écran
            system("clear");

            //afficher la lettre du joueur
            printf("Le joueur %s a joué la lettre %s\n", tabPrenom[i], msg.mtext);

            //vérifier si la lettre est dans le mot
            int lettreTrouve = 0;
            for (int j = 0; j < tailleMot; j++)
            {
                if (toupper(msg.mtext[0]) == toupper(mot[j]))
                {
                    motCache[j] = mot[j];
                    lettreTrouve = 1;
                }
            }

            //afficher si la lettre est dans le mot ou non
            if (lettreTrouve == 1)
            {
                printf("La lettre %s est dans le mot\n", msg.mtext);
            }
            else
            {
                printf("La lettre %s n'est pas dans le mot\n", msg.mtext);
                nbErreur++;
            }

            //afficher l'état du pendu
            pendu(nbErreur);

            //si le nombre d'erreur est égal à 6, on envoie SIGUSR2 à tous les joueurs pour les prévenir de la fin de la partie
            if(nbErreur == 6)
            {
                for (int i = 0; i < nbJoueur; i++)
                {
                    if (kill(tabPid[i], SIGUSR2) == -1)
                    {
                        perror("Error in kill");
                    } 
                }
                printf("Le pendu est pendu !\n");
                printf("Le mot était : %s\n", mot);
                break;
            }
            else
            {
                //afficher le mot à deviner
                printf("Le mot de %d lettres à deviner est : %s !!!\n",tailleMot, motCache);
            }
            
        }
    }

    return 0;
}