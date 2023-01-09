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
#include <stdbool.h>

#define MAX_JOUEUR 10
#define CLE_INIT 1234
#define CLE_TOUR 2345

int fin_de_partie = 0;

struct msgbuf
{
    long mtype;
    char mtext[20];
};

struct utilisateur
{
    int pid;
    char prenom[20];
};

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

int main(int argc, char *argv[])
{
    int nbLettreTrouvee = 0; //definition du nbLettreTrouvee
    int nbErreur = 0; //definition du nbErreur
    int boite_init = msgget(CLE_INIT, IPC_CREAT | 0666); //création d'une boite aux lettre pour l'initialisation
    int boite_tour = msgget(CLE_TOUR, IPC_CREAT | 0666); //création d'une boite aux lettre pour les tours
    int nbJoueur = 0; //definition du nombre de joueur
    struct msgbuf msg; //definition du message
    struct utilisateur tabUtilisateur[MAX_JOUEUR]; //definiton du tableau d'utilisateurs
    int nbMot = 0; //definition du nombre de mot
    char mot[20]; //definition du mot
    bool isLettreTrouve = false; //definition du booleen isLettreTrouve
    bool isMotTrouve = false; //definition du booleen isMotTrouve

    //demander le nombre de joueur tant qu'il est inférieur à 3
    while (nbJoueur < 1 || nbJoueur > MAX_JOUEUR)
    {
        printf("Entrez le nombre de joueur (supérieur à 2): ");
        scanf("%d", &nbJoueur);
    }

    printf("\nEn attente de connexion des joueurs...\n\n");

    //boucle jusqu'à ce que le tableau soit rempli
    int i = 0; //definition de i
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
        tabUtilisateur[i].pid = msg.mtype;
        strcpy(tabUtilisateur[i].prenom, msg.mtext);

        //afficher le nom du joueur
        printf("Joueur n°%d : %s\n", i+1, tabUtilisateur[i].prenom);

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

    //supprimer le retour à la ligne de mon mot s'il y en a un 
    for(int i=0; i<strlen(mot); i++)
    {
        if(mot[i] == '\r')
        {
            mot[i] = '\0';
        }
    }

    //mettre le mot en majuscule
    for (int i = 0; i < strlen(mot); i++)
    {
        mot[i] = toupper(mot[i]);
    }

    //stocker le mot dans une variable en remplacer les lettres par des "_"
    char motCache[tailleMot];
    for (int i = 0; i <= tailleMot; i++)
    {
        if(i != tailleMot)
        {
            motCache[i] = '_';
        }
        else
        {
            motCache[i] = '\0';
        }
    }


    //afficher le mot à deviner
    printf("Le mot de %d lettres à deviner est : %s !!!\n",tailleMot, motCache);

    //envoi un signal aux joueurs pour dire que la partie à commencé
    for (int i = 0; i < nbJoueur; i++)
    {
        if (kill(tabUtilisateur[i].pid, SIGUSR1) == -1)
        {
            perror("Error in kill");
        } 
    }

    while(fin_de_partie == 0)
    {
        for (int i = 0; i < nbJoueur; i++)
        {
            //On attend que les joueurs soient prêts
            sleep(1);
            //On envoi un signal au joueur pour dire qu'il doit jouer
            if (kill(tabUtilisateur[i].pid, SIGUSR1) == -1)
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

            if(strlen(msg.mtext) == 1)
            {
                //afficher la lettre du joueur
                printf("Le joueur %s a joué la lettre '%s'\n", tabUtilisateur[i].prenom, msg.mtext);

                //vérifier si la lettre est dans le mot
                for (int j = 0; j < tailleMot; j++)
                {
                    if (toupper(msg.mtext[0]) == toupper(mot[j]))
                    {
                        motCache[j] = mot[j];
                        isLettreTrouve = true;
                        nbLettreTrouvee++;
                    }
                }

                //afficher si la lettre est dans le mot ou non
                if (isLettreTrouve == true)
                {
                    printf("La lettre '%s' est dans le mot\n", msg.mtext);
                    isLettreTrouve = false;
                }
                else
                {
                    printf("La lettre '%s' n'est pas dans le mot\n", msg.mtext);
                    nbErreur++;
                }
            }
            else
            {
                //afficher le mot du joueur
                printf("Le joueur %s a joué le mot '%s'\n", tabUtilisateur[i].prenom, msg.mtext);

                //vérifier si le mot est le bon
                //mettre le mot en majuscule
                for (int i = 0; i < strlen(msg.mtext); i++)
                {
                    msg.mtext[i] = toupper(msg.mtext[i]);
                }
                if (strcmp(msg.mtext, mot) == 0)
                {
                    nbLettreTrouvee = tailleMot;
                }
                else
                {
                    printf("Le mot est incorrect\n");
                    nbErreur++;
                }
            }

            //afficher l'état du pendu
            pendu(nbErreur);

            //On envoi un signal si la partie est finie
            if(nbErreur == 6)
            {
                for (int i = 0; i < nbJoueur; i++)
                {
                    if (kill(tabUtilisateur[i].pid, SIGUSR2) == -1)
                    {
                        perror("Error in kill");
                    } 
                }
                printf("\nLe pendu s'est pendu :/ !!!\n");
                printf("\nLe mot était : %s !!!\n", mot);
            }
            else if(nbLettreTrouvee == tailleMot)
            {
                for (int i = 0; i < nbJoueur; i++)
                {
                    if (kill(tabUtilisateur[i].pid, SIGUSR2) == -1)
                    {
                        perror("Error in kill");
                    } 
                }
                printf("\nVous avez trouvé le mot : %s !!!\n", mot);
            }
            else
            {
                //afficher le mot à deviner
                printf("Le mot de %d lettres à deviner est : %s !!!\n",tailleMot, motCache);
            }
            
        }
    }

    sleep(1);

    //On envoi un signal aux joueurs pour dire qu'ils ont gagnés ou perdus
    for (int i = 0; i < nbJoueur; i++)
    {
        if(nbErreur == 6)
        {
            if (kill(tabUtilisateur[i].pid, SIGUSR2) == -1)
            {
                perror("Error in kill");
            } 
        }
        else
        {
            if (kill(tabUtilisateur[i].pid, SIGUSR1) == -1)
            {
                perror("Error in kill");
            } 
        }

    }

    return 0;
}