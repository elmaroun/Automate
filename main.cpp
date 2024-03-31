/*
   Projet réalisé par :
   - Ilias Maroun
   - Mohamed El hauari
   - Rajae El-abida
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// definitiion des structures qui vont representer l'automate
struct Transition {
    int etat_depart;
    int etat_arrive;
    char etiquet;
};

struct Automate {
    int etat_initial[50];
    int etat_final[50];
    struct Transition *transitions;
};

//definition des variable necessaire
int nbr_etats,nbr_etats9;
int Etats[100];
int nbr_lignes8;
char  str[100],Alphabet[100];
int nombre_alphabet =0;

int nbr_lignes, nbr_etats_initiaux, nbr_etats_finaux;
int nbr_lignes_union, nbr_etats_initiaux_union, nbr_etats_finaux_union;
int nbr_lignes_etoile = 0, nbr_etats_initiaux_etoile=0, nbr_etats_finaux_etoile=0;
int nbr_lignes_produit, nbr_etats_initiaux_produit =0, nbr_etats_finaux_produit=0;
int nbr_lignes_e, nbr_etats_initiaux_e, nbr_etats_finaux_e, nbr_etats_e;
int nbr_lignes_deterministe,nbr_etats_initiaux_deterministe,nbre_etats_fianux_deterministe;
int nbr_lignes_deterministe1,nbr_etats_initiaux_deterministe1,nbre_etats_fianux_deterministe1;
int nbr_lignes_inverse1,nbr_etats_initiaux_inverse1,nbr_etats_finaux_inverse1;

Automate Automate_u;
Automate Automate2;
Automate Automate3;
Automate Automate_produit;
Automate Automate_et;
Automate Automate_d;
Automate Automate_d1;
Automate Automate_d2;


// fonction pour calculer le nombre des entiers dans un string
int compter_numeros_dans_string(char *str) {
    int num = 0, i;
    for (int  i=0; i < strlen(str); i++) {
        if (isdigit(str[i])) {
            num++;
            while (i < strlen(str) && isdigit(str[i])) {
                i++;
            }
        }
    }
    return num;
}

// fonction pour lire un fichier contenant un automate et le stocker dans un variable de type automte
void lire_fichier (char *nom_fichier, Automate *Automate) {
    FILE* fichier = fopen(nom_fichier, "r");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    // nbr lignes
    char L[100];
    nbr_lignes = 0;
    int i;
    while (fgets(L, sizeof(L), fichier)) {
        nbr_lignes++;
    }
    rewind(fichier);

    // nbr etats initiaux
    int  line_number= 1;
    nbr_etats_initiaux=0;
    char L1[100];
    while (fgets(L1, sizeof(L1), fichier)) {
        if (line_number == nbr_lignes - 1 ) {
            nbr_etats_initiaux = compter_numeros_dans_string(L1);

        }
        line_number++;
    }
    rewind(fichier);

    // nbr etats finaux
    int  line_number1= 1;
    nbr_etats_finaux=0;
    char L2[100];
    while (fgets(L2, sizeof(L2), fichier)) {
        if (line_number1 == nbr_lignes ) {
            nbr_etats_finaux = compter_numeros_dans_string(L2);
        }
        line_number1++;
    }
    rewind(fichier);

    // allocation de memoire pour transition
    Automate->transitions = (struct Transition*) malloc((nbr_lignes - 2) * sizeof(struct Transition));

    for (int i = 0; i <nbr_lignes - 2; i++) {
        fscanf(fichier, "%d %d %c", &Automate->transitions[i].etat_depart,
               &Automate->transitions[i].etat_arrive,
               &Automate->transitions[i].etiquet);
    }
    for (int i = 0; i < nbr_etats_initiaux; i++) {
        fscanf(fichier, "%d", &Automate->etat_initial[i]);
    }
    for (int i = 0; i < nbr_etats_finaux; i++) {
        fscanf(fichier, "%d", &Automate->etat_final[i]);
    }
    fclose(fichier);
// les etats
    int Etats_repetes[100];
    int j=0;
    for (int i = 0; i < nbr_lignes - 2; i++) {
        Etats_repetes[j]=Automate->transitions[i].etat_depart;
        Etats_repetes[j+1]=Automate->transitions[i].etat_arrive;
        j=j+2;
    }

    Etats[0] = Etats_repetes[0];
    nbr_etats = 1;
    int repete;
    j = 0;

    for (i=1; i < (nbr_lignes - 2)*2 ; i++) {
        repete = 0;
        for (j = 0; j < nbr_etats; j++) {
            if (Etats[j] == Etats_repetes[i]) {
                repete = 1;
                break;
            }
        }
        if (repete == 0) {
            Etats[nbr_etats] = Etats_repetes[i];
            nbr_etats++;
        }
    }

}

// fonction pour afficher les transition d'un automate
void afficher_transitions(Automate Automate, int nombre_ligne){

    printf("\n\t- Transitions :  Depart  ->  Arrive  :  Etiquet\n");
    printf("\n");
    for (int i = 0; i <nombre_ligne - 2  ; i++) {
        printf("\t\t %d   ->   %d   :   %c\n",  Automate.transitions[i].etat_depart,
               Automate.transitions[i].etat_arrive,
               Automate.transitions[i].etiquet);
    }
    printf("\n\n");

}

// fonction pour afficher les etats initiaux et finaux d'un automate
void afficher_etats_initiaux_finaux(Automate Automate, int nombre_etat_i,int nombre_etat_f){
    printf("\n\t- Etats initiaux :  ");
    for (int i = 0; i < nombre_etat_i; i++) {
        printf("  %d  ", Automate.etat_initial[i]);
    }
    printf("\n\t- Etats finaux : ");
    for (int i = 0; i < nombre_etat_f; i++) {
        printf("  %d  ", Automate.etat_final[i]);
    }
    printf("\n\n\n");
}

// fonction pour afficher l'alphabets d'un automate
void afficher_alphabets(Automate Automate, int nbr_ligne) {

    char str[50];
    int nbr_alphabets=0;

    printf("\n\t- Ensemble des alphabets de l'automate : { ");

    for (int i=0; i < nbr_ligne -2 ; i++) {
        char symbol=Automate.transitions[i].etiquet;

        int found = 0;
        for (int j=0; j < nbr_alphabets; j++) {
            if (str[j] == symbol) {
                found=1;
                break;
            }
        }
        if (found==0) {
            str[nbr_alphabets] = symbol;
            printf(" %c ", symbol);
            Alphabet[nombre_alphabet]=symbol;
            nombre_alphabet=nombre_alphabet + 1 ;
            nbr_alphabets++;
        }
    }
    printf("}\n");
    printf("%d",nombre_alphabet);
    for(int i=0;i<nombre_alphabet;i++){
        printf("alphabet = %c",Alphabet[i]);
    }
}

// fonction pour generer un fichier dot( une representation )  correspondante a une automate
void generer_fichier_dot(Automate Automate,int nombre_etat_i,int nombre_etat_f,int nombre_ligne) {

    // les etats atteignables
    int etats_atteignables[100];
    for (int i=0; i<nombre_etat_i; i++) {
        etats_atteignables[i]=Automate.etat_initial[i];
    }
    int nbr_etats_atteignable = nombre_etat_i;
    int i=0;
    while ( i < nbr_etats_atteignable) {
        for (int j=0; j<nombre_ligne - 2; j++) {
            if (Automate.transitions[j].etat_depart == Automate.transitions[j].etat_arrive) {
            }
            else if (etats_atteignables[i] == Automate.transitions[j].etat_depart) {
                etats_atteignables[nbr_etats_atteignable] = Automate.transitions[j].etat_arrive;
                nbr_etats_atteignable++;
            }
        }
        i++;
    }

    // tous les etats
    int etats[100];
    int j=0;
    for (int i = 0; i < nombre_ligne - 2; i++) {
        etats[j]=Automate.transitions[i].etat_depart;
        etats[j+1]=Automate.transitions[i].etat_arrive;
        j=j+2;
    }

    // generer fichier .dot
    FILE *file;
    file=fopen("Y8.dot","w");
    fprintf(file,"digraph { \n");
    for (int i = 0; i < nombre_ligne - 2; i++) {
        fprintf(file,"%d->%d[label=\"%c\"];\n", Automate.transitions[i].etat_depart,
                Automate.transitions[i].etat_arrive,
                Automate.transitions[i].etiquet);
    }
    for (int i = 0; i < nombre_etat_i; i++) {
        fprintf(file, "%d [color=green];\n", Automate.etat_initial[i]);
    }
    for (int i = 0; i < nombre_etat_f; i++) {
        fprintf(file, "%d [peripheries=2];\n", Automate.etat_final[i]);
    }

    // colorage des etats innatteignables
    for (int i=0;i<(nombre_ligne - 2)*2;i++){
        int exist=0;
        int j=0;
        while (j<nbr_etats_atteignable && exist==0) {
            if(etats[i]==etats_atteignables[j]){
                exist=1;
            }
            j++;
        }
        if (exist==0){
            fprintf(file,"%d [style=filled, fillcolor=gray];\n",etats[i]);
        }
    }
    fprintf(file,"}");
    fclose(file);
    system("dot -Tpng Y8.dot -o Y8.png");
    system("start Y8.png");

}

// fonction pour tester si un mot est engendre par un automate
void tester_mot(char *mot, Automate Automate){

    bool mot_engendre=false;
    int etat_en_verification;
    char caractere_en_verification;
    int iterations=0, ini=1;
    int k, i=0, j=0;

    while ( i<nbr_etats_initiaux && mot_engendre==false ) {

        etat_en_verification = Automate.etat_initial[i];
        ini=1;
        j=0;
        iterations=0;

        while (j<strlen(mot) && ini==1) {

            caractere_en_verification = mot[j];
            k=0;

            while ( k<nbr_lignes-2 && mot_engendre==false) {
                if ( Automate.transitions[k].etiquet==caractere_en_verification && Automate.transitions[k].etat_depart==etat_en_verification ) {

                    etat_en_verification = Automate.transitions[k].etat_arrive;
                    iterations++;

                    for (int l=0; l<nbr_etats_finaux; l++){
                        if ( etat_en_verification==Automate.etat_final[l] && strlen(mot)==iterations ) {
                            mot_engendre=true;
                        }
                    }
                }
                k++;
            }
            if (iterations==0) { ini=0; }
            j++;
        }

        i++;
    }

    if (mot_engendre==true) {
        printf("\n\t\tLe mot %s est engendre' \n", mot);
    }
    else {
        printf("\n\t\tLe mot %s n'est pas engendre' \n", mot);
    }
}

// tester si plusiers mots sont engendre par un automate
void tester_fichhier_mots(char *nom_fichier_mots, Automate Automate) {

    FILE* fichier = fopen(nom_fichier_mots, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    char mot_du_fichier[20];
    while (fscanf(fichier, "%s", mot_du_fichier) != EOF) {
        tester_mot(mot_du_fichier, Automate);
    }
}

// une fonction pour faire l'union de deux automates
void Union_automate(char *nom_fichier9,Automate Automate1,Automate *Automate9,Automate *Automate_union) {
    FILE* fichier9 = fopen(nom_fichier9, "r");
    if (fichier9 == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    int nbr_lignes9, nbr_etats_initiaux9, nbr_etats_finaux9;
    char L9[100];
    nbr_lignes9 = 0;
    int i;
    while (fgets(L9, sizeof(L9), fichier9)) {
        nbr_lignes9++;
    }
    rewind(fichier9);

    // nbr etats initiaux
    int  line_number9= 1;
    nbr_etats_initiaux9=0;
    char L19[100];
    while (fgets(L19, sizeof(L19), fichier9)) {
        if (line_number9 == nbr_lignes9 - 1 ) {
            nbr_etats_initiaux9 = compter_numeros_dans_string(L19);

        }
        line_number9++;
    }
    rewind(fichier9);

    // nbr etats finaux
    int  line_number19= 1;
    nbr_etats_finaux9=0;
    char L29[100];
    while (fgets(L29, sizeof(L29), fichier9)) {
        if (line_number19 == nbr_lignes9 ) {
            nbr_etats_finaux9 = compter_numeros_dans_string(L29);
        }
        line_number19++;
    }
    rewind(fichier9);

    // allocation de memoire pour transition
    Automate9->transitions = (struct Transition*) malloc((nbr_lignes9 - 2) * sizeof(struct Transition));

    for (int i = 0; i <nbr_lignes9 - 2; i++) {
        fscanf(fichier9, "%d %d %c", &Automate9->transitions[i].etat_depart ,
               &Automate9->transitions[i].etat_arrive,
               &Automate9->transitions[i].etiquet);
        Automate9->transitions[i].etat_depart=Automate9->transitions[i].etat_depart + 100;
        Automate9->transitions[i].etat_arrive=Automate9->transitions[i].etat_arrive + 100;
    }
    for (int i = 0; i < nbr_etats_initiaux9; i++) {
        fscanf(fichier9, "%d", &Automate9->etat_initial[i]);
        Automate9->etat_initial[i]=Automate9->etat_initial[i]+100;

    }
    for (int i = 0; i < nbr_etats_finaux9; i++) {
        fscanf(fichier9, "%d", &Automate9->etat_final[i]);
        Automate9->etat_final[i]=Automate9->etat_final[i]+100;
    }
    int nombre_transition_union;

    nombre_transition_union=nbr_lignes + nbr_lignes9 + nbr_etats_initiaux + nbr_etats_initiaux9+ (nbr_etats_finaux + nbr_etats_finaux9) - 4;

    Automate_union->transitions=(struct Transition*) malloc(nombre_transition_union * sizeof(struct Transition));

    for ( int i=0; i<nbr_etats_initiaux;i++){
        Automate_union->transitions[i].etat_depart=999;
        Automate_union->transitions[i].etat_arrive=Automate1.etat_initial[i];
        Automate_union->transitions[i].etiquet='$';
    }
    for (int i=0 ;i<nbr_etats_initiaux9;i++){
        Automate_union->transitions[i+ nbr_etats_initiaux].etat_depart=999;
        Automate_union->transitions[i + nbr_etats_initiaux].etat_arrive=Automate9->etat_initial[i];
        Automate_union->transitions[i + nbr_etats_initiaux].etiquet='$';
    }
    for (int i=0; i<nbr_lignes-2; i++){
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9].etat_depart = Automate1.transitions[i].etat_depart;
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9].etat_arrive = Automate1.transitions[i].etat_arrive;
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9].etiquet = Automate1.transitions[i].etiquet;
    }
    for (int i=0; i<nbr_lignes9-2; i++){
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9+nbr_lignes -2].etat_depart = Automate9->transitions[i].etat_depart;
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9 + nbr_lignes - 2].etat_arrive = Automate9->transitions[i].etat_arrive;
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9 + nbr_lignes -2].etiquet = Automate9->transitions[i].etiquet;
    }

    for (int i=0; i<nbr_etats_finaux; i++){
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9+ nbr_lignes + nbr_lignes9 -4].etat_depart = Automate1.etat_final[i];
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9+ nbr_lignes + nbr_lignes9 -4].etat_arrive = 1000;
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9+ nbr_lignes + nbr_lignes9 -4].etiquet = '$';
    }
    for (int i=0; i<nbr_etats_finaux9; i++){
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9+ nbr_lignes + nbr_lignes9 -4 +nbr_etats_finaux].etat_depart = Automate9->etat_final[i];
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9+ nbr_lignes + nbr_lignes9 -4 +nbr_etats_finaux].etat_arrive = 1000;
        Automate_union->transitions[i+nbr_etats_initiaux+nbr_etats_initiaux9+ nbr_lignes + nbr_lignes9 -4 +nbr_etats_finaux].etiquet = '$';
    }



    nbr_etats_initiaux_union=1;
    nbr_etats_finaux_union=1;

    nbr_lignes_union=nombre_transition_union + 2;

    Automate_union->etat_initial[0]=999;
    Automate_union->etat_final[0]=1000;


    fclose(fichier9);


}

// une fonction pour faire le produit(intersection) de deux automates
void Produit_automate(char *nom_fichier9,Automate Automate1,Automate *Automate9,Automate *Automate_produit) {

    FILE* fichier9 = fopen(nom_fichier9, "r");
    if (fichier9 == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    // nbr lignes
    char L9[100];
    int  nbr_etats_initiaux9, nbr_etats_finaux9;
    nbr_lignes8 = 0;
    int i;
    while (fgets(L9, sizeof(L9), fichier9)) {
        nbr_lignes8++;
    }
    rewind(fichier9);

    int  line_number9= 1;
    nbr_etats_initiaux9=0;
    char L19[100];
    while (fgets(L19, sizeof(L19), fichier9)) {
        if (line_number9 == nbr_lignes8 - 1 ) {
            nbr_etats_initiaux9 = compter_numeros_dans_string(L19);

        }
        line_number9++;
    }
    rewind(fichier9);

    int  line_number19= 1;
    nbr_etats_finaux9=0;
    char L29[100];
    while (fgets(L29, sizeof(L29), fichier9)) {
        if (line_number19 == nbr_lignes8 ) {
            nbr_etats_finaux9 = compter_numeros_dans_string(L29);
        }
        line_number19++;
    }
    rewind(fichier9);

    // allocation de memoire pour transition
    Automate9->transitions = (struct Transition*) malloc((nbr_lignes8 - 2) * sizeof(struct Transition));

    for (int i = 0; i <nbr_lignes8 - 2; i++) {
        fscanf(fichier9, "%d %d %c", &Automate9->transitions[i].etat_depart ,
               &Automate9->transitions[i].etat_arrive,
               &Automate9->transitions[i].etiquet);
        Automate9->transitions[i].etat_depart=Automate9->transitions[i].etat_depart + 100;
        Automate9->transitions[i].etat_arrive=Automate9->transitions[i].etat_arrive + 100;
    }
    for (int i = 0; i < nbr_etats_initiaux9; i++) {
        fscanf(fichier9, "%d", &Automate9->etat_initial[i]);
        Automate9->etat_initial[i]=Automate9->etat_initial[i]+100;

    }
    for (int i = 0; i < nbr_etats_finaux9; i++) {
        fscanf(fichier9, "%d", &Automate9->etat_final[i]);
        Automate9->etat_final[i]=Automate9->etat_final[i]+100;
    }
    struct Groupe{
        int groupe[2];
    };
// les etats
    int Etats_repetes[100];
    int j=0;
    for (int i = 0; i < nbr_lignes8 - 2; i++) {
        Etats_repetes[j]=Automate9->transitions[i].etat_depart;
        Etats_repetes[j+1]=Automate9->transitions[i].etat_arrive;
        j=j+2;
    }

    int Etats9[100];
    Etats9[0] = Etats_repetes[0];

    nbr_etats9 = 1;
    int repete;
    j = 0;

    for (i=1; i < (nbr_lignes8 - 2)*2 ; i++) {
        repete = 0;
        for (j = 0; j < nbr_etats9; j++) {
            if (Etats9[j] == Etats_repetes[i]) {
                repete = 1;
                break;
            }
        }
        if (repete == 0) {
            Etats9[nbr_etats9] = Etats_repetes[i];
            nbr_etats9++;
        }
    }
    struct Groupe liste_groupe[10];
    Automate_produit->transitions = (struct Transition*) malloc(100 * sizeof(struct Transition));
    int nombre_groupe=0;

    int k=0;
    for (int i=0;i<nbr_etats_initiaux;i++){
        int k=0;
        while(k<nbr_etats_initiaux9){
            liste_groupe[nombre_groupe].groupe[0]=Automate1.etat_initial[i];
            liste_groupe[nombre_groupe].groupe[1]=Automate9->etat_initial[k];
            nombre_groupe=nombre_groupe+1;
            ++k;
        }
    }
    nombre_groupe=nbr_etats_initiaux*nbr_etats_initiaux9;

    char Alphabet3[20];
    for(int i=0;i<nbr_lignes8-2;i++){
        Alphabet[i]=Automate9->transitions[i].etiquet;
    }
    for(int j=0;j<nbr_lignes-2;j++){
        Alphabet[j+nbr_lignes8 - 2]=Automate1.transitions[j].etiquet;
    }
    int tailleAlphabet1=nbr_lignes8 + nbr_lignes -4;
    int taille=0;
    for (int i=0; i < tailleAlphabet1 ; i++) {
        char symbol=Alphabet[i];
        int found = 0;
        for (int j=0; j < taille; j++) {
            if (Alphabet3[j] == symbol) {
                found=1;
                break;
            }
        }
        if (found==0) {
            Alphabet3[taille] = symbol;
            taille++;
        }
    }



    char caractere_actuel;
    int nbr_transition=0;
    int etat_actuel1_d,etat_actuel1_a;
    int etat_actuel2_d, etat_actuel2_a;
    int nombre_transition_actuel=0;
    for(int i = 0; i < nombre_groupe; i++) {
        etat_actuel1_d = liste_groupe[i].groupe[0];
        etat_actuel2_d = liste_groupe[i].groupe[1];

        for(j = 0; j < taille; j++) {
            caractere_actuel = Alphabet3[j];
            bool trouve1 = false;
            bool trouve2 = false;
            bool groupe_trouve=false;

            // Recherche de la transition pour le premier automate
            for(int k = 0; k < nbr_lignes - 2; k++) {
                if(Automate1.transitions[k].etiquet == caractere_actuel && Automate1.transitions[k].etat_depart == etat_actuel1_d) {
                    etat_actuel1_a = Automate1.transitions[k].etat_arrive;
                    trouve1 = true;
                    break;
                }
            }

            // Recherche de la transition pour le deuxiÃ¨me automate
            for(int k = 0; k < nbr_lignes8 - 2; k++) {
                if(Automate9->transitions[k].etiquet == caractere_actuel && Automate9->transitions[k].etat_depart == etat_actuel2_d) {
                    etat_actuel2_a = Automate9->transitions[k].etat_arrive;
                    trouve2 = true;
                    break;
                }
            }
            int index=0;

            if(trouve1 && trouve2) {
                for(int k = 0; k < nombre_groupe; k++) {
                    if(liste_groupe[k].groupe[0] == etat_actuel1_a && liste_groupe[k].groupe[1] == etat_actuel2_a) {
                        groupe_trouve=true;
                        index=k;
                    }
                }
                if(groupe_trouve==true){
                    Automate_produit->transitions[nbr_transition].etat_depart=i;
                    Automate_produit->transitions[nbr_transition].etat_arrive=index;
                    Automate_produit->transitions[nbr_transition].etiquet=caractere_actuel;
                    nbr_transition=nbr_transition + 1;
                }
                else{
                    liste_groupe[nombre_groupe].groupe[0]=etat_actuel1_a;
                    liste_groupe[nombre_groupe].groupe[1]=etat_actuel2_a;
                    Automate_produit->transitions[nbr_transition].etat_depart=i;
                    Automate_produit->transitions[nbr_transition].etat_arrive=nombre_groupe;
                    Automate_produit->transitions[nbr_transition].etiquet=caractere_actuel;
                    nbr_transition=nbr_transition+1;
                    nombre_groupe=nombre_groupe+1;

                }

            }
        }
    }

    int etat1,etat2;
    for(int k = 0; k < nombre_groupe; k++) {
        bool etat_initial1_trouve=false,etat_initial2_trouve=false;
        etat1=liste_groupe[k].groupe[0];
        etat2=liste_groupe[k].groupe[1];
        for (int j=0;j<nbr_etats_initiaux;j++){
            if (Automate1.etat_initial[j]==etat1){
                etat_initial1_trouve=true;
            }
        }
        for (int j=0;j<nbr_etats_initiaux9;j++){
            if (Automate9->etat_initial[j]==etat2){
                etat_initial2_trouve=true;
            }
        }
        if (etat_initial1_trouve==true && etat_initial2_trouve==true){
            Automate_produit->etat_initial[nbr_etats_initiaux_produit]=k;
            printf(" \nk = %d \n",Automate_produit->etat_initial[nbr_etats_initiaux_produit]);
            nbr_etats_initiaux_produit=nbr_etats_initiaux_produit + 1;
        }

    }
    nbr_etats_finaux_produit=0;
    int etatf1,etatf2;
    for(int k = 0; k < nombre_groupe; k++) {
        bool etat_final1_trouve=false,etat_final2_trouve=false;
        etatf1=liste_groupe[k].groupe[0];
        etatf2=liste_groupe[k].groupe[1];
        for (int j=0;j<nbr_etats_finaux;j++){
            if (Automate1.etat_final[j]==etatf1){
                etat_final1_trouve=true;
            }
        }
        for (int j=0;j<nbr_etats_finaux9;j++){
            if (Automate9->etat_final[j]==etatf2){
                etat_final2_trouve=true;
            }
        }
        if (etat_final1_trouve==true && etat_final2_trouve==true){
            Automate_produit->etat_final[nbr_etats_finaux_produit]=k;
            nbr_etats_finaux_produit=nbr_etats_finaux_produit + 1;
        }

    }




    nbr_lignes_produit=nbr_transition + 2;


}

// une fonction pour faire l'étoile d'un automate
void etoile_automate(Automate Automate1,Automate *Automate_etoile){
    Automate_etoile->transitions = (struct Transition*) malloc((nbr_lignes+nbr_etats_initiaux + nbr_etats_finaux +2  )* sizeof(struct Transition));

    for (int k=0 ;k<nbr_etats_initiaux;k++){
        Automate_etoile->transitions[k].etat_depart=999;
        Automate_etoile->transitions[k].etat_arrive=Automate1.etat_initial[k];
        Automate_etoile->transitions[k].etiquet='$';
    }
    for (int j=0 ;j<nbr_etats_finaux;j++){
        Automate_etoile->transitions[j+nbr_etats_initiaux].etat_depart=Automate1.etat_final[j];
        Automate_etoile->transitions[j+nbr_etats_initiaux].etat_arrive=1000;
        Automate_etoile->transitions[j + nbr_etats_initiaux].etiquet='$';
    }
    Automate_etoile->transitions[nbr_etats_initiaux+ nbr_etats_finaux].etat_depart=1000;
    Automate_etoile->transitions[nbr_etats_finaux+nbr_etats_initiaux].etat_arrive=1001;
    Automate_etoile->transitions[nbr_etats_finaux + nbr_etats_initiaux].etiquet='$';

    Automate_etoile->transitions[nbr_etats_initiaux+ nbr_etats_finaux + 1].etat_depart=998;
    Automate_etoile->transitions[nbr_etats_finaux + nbr_etats_initiaux + 1].etat_arrive=999;
    Automate_etoile->transitions[nbr_etats_finaux + nbr_etats_initiaux + 1].etiquet='$';
    Automate_etoile->transitions[nbr_etats_initiaux+ nbr_etats_finaux + 2].etat_depart=998;
    Automate_etoile->transitions[nbr_etats_finaux + nbr_etats_initiaux + 2].etat_arrive=1001;
    Automate_etoile->transitions[nbr_etats_finaux + nbr_etats_initiaux + 2].etiquet='$';
    Automate_etoile->transitions[nbr_etats_initiaux+ nbr_etats_finaux + 3].etat_depart=1000;
    Automate_etoile->transitions[nbr_etats_finaux + nbr_etats_initiaux + 3].etat_arrive=999;
    Automate_etoile->transitions[nbr_etats_finaux + nbr_etats_initiaux + 3].etiquet='$';
    for (int i=0; i<nbr_lignes-2; i++){
        Automate_etoile->transitions[nbr_etats_finaux + nbr_etats_initiaux + 4 + i].etat_depart = Automate1.transitions[i].etat_depart;
        Automate_etoile->transitions[nbr_etats_finaux + nbr_etats_initiaux + 4+ i].etat_arrive = Automate1.transitions[i].etat_arrive;
        Automate_etoile->transitions[nbr_etats_finaux + nbr_etats_initiaux + 4 + i].etiquet = Automate1.transitions[i].etiquet;
    }
    nbr_lignes_etoile=nbr_etats_finaux + nbr_etats_initiaux + 4 + nbr_lignes -2;
    for (int i=0; i<nbr_etats_finaux + nbr_etats_initiaux + 4 + nbr_lignes -2; i++){
    }
    Automate_etoile->etat_initial[0]=998;
    Automate_etoile->etat_final[0]=1001;
    nbr_etats_finaux_etoile=1;
    nbr_etats_initiaux_etoile=1;
}

// une fonction pour supprimer les epsilons d'un automate
void Epsilon (char *nom_fichier_epsilon, Automate *Automate1, Automate *Automate_non_epsilon) {

    FILE* fichier = fopen(nom_fichier_epsilon, "r");

    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier.\n");
        exit(1);
    }

    // nbr lignes
    char L[100];
    nbr_lignes_e = 0;
    int i;
    while (fgets(L, sizeof(L), fichier)) {
        nbr_lignes_e++;
    }
    rewind(fichier);

    // nbr etats initiaux
    int  line_number= 1;
    nbr_etats_initiaux_e=0;
    char L1[100];
    while (fgets(L1, sizeof(L1), fichier)) {
        if (line_number == nbr_lignes_e - 1 ) {
            nbr_etats_initiaux_e = compter_numeros_dans_string(L1);

        }
        line_number++;
    }
    rewind(fichier);

    // nbr etats finaux
    int  line_number1= 1;
    nbr_etats_finaux_e=0;
    char L2[100];
    while (fgets(L2, sizeof(L2), fichier)) {
        if (line_number1 == nbr_lignes_e ) {
            nbr_etats_finaux_e = compter_numeros_dans_string(L2);
        }
        line_number1++;
    }
    rewind(fichier);

    // allocation de memoire pour transition
    Automate1->transitions = (struct Transition*) malloc((nbr_lignes_e - 2) * sizeof(struct Transition));

    for (int i = 0; i <nbr_lignes_e - 2; i++) {
        fscanf(fichier, "%d %d %c", &Automate1->transitions[i].etat_depart,
               &Automate1->transitions[i].etat_arrive,
               &Automate1->transitions[i].etiquet);
    }
    for (int i = 0; i < nbr_etats_initiaux_e; i++) {
        fscanf(fichier, "%d", &Automate1->etat_initial[i]);
    }
    for (int i = 0; i < nbr_etats_finaux_e; i++) {
        fscanf(fichier, "%d", &Automate1->etat_final[i]);
    }
    fclose(fichier);

    // les etats
    int Etats_repetes[100];
    int j=0;
    for (int i = 0; i < nbr_lignes_e - 2; i++) {
        Etats_repetes[j]=Automate1->transitions[i].etat_depart;
        Etats_repetes[j+1]=Automate1->transitions[i].etat_arrive;
        j=j+2;
    }

    int Etats[100];
    Etats[0] = Etats_repetes[0];
    nbr_etats_e = 1;
    int repete;
    j = 0;

    for (i=1; i < (nbr_lignes_e - 2)*2 ; i++) {
        repete = 0;
        for (j = 0; j < nbr_etats_e; j++) {
            if (Etats[j] == Etats_repetes[i]) {
                repete = 1;
                break;
            }
        }
        if (repete == 0) {
            Etats[nbr_etats_e] = Etats_repetes[i];
            nbr_etats_e++;
        }
    }

    //////////////////////////////////////////////////////////////////
    Automate_non_epsilon->transitions = (struct Transition*) malloc( 20 * sizeof(struct Transition));
    int nbr_transitions_union=0;
    int l=0;
    nbr_etats_initiaux_e=0;
    nbr_etats_finaux_e=0;
    int a;
    int with_ep[20], nbr_with_ep, etatt;


    for (int i=0; i<nbr_etats_e; i++) {   //etats


        for (a=0; a<nbr_etats_initiaux; a++) {  //etats initiauc

            if (Etats[i]==Automate1->etat_initial[a]) {
                Automate_non_epsilon->etat_initial[nbr_etats_initiaux_e]=Etats[i];
                nbr_etats_initiaux_e++;

            }
        }

        for (a=0; a<nbr_etats_finaux; a++) {   // etats finaux

            if (Etats[i]==Automate1->etat_final[a]) {
                Automate_non_epsilon->etat_final[nbr_etats_finaux_e]=Etats[i];
                nbr_etats_finaux_e++;

            }
        }


        for (int m=0; m<nbr_lignes_e - 2; m++) {

            if ( (Automate1->transitions[m].etat_depart == Etats[i] && Automate1->transitions[m].etiquet!='$')  ) {
                Automate_non_epsilon->transitions[l].etat_depart = Etats[i];
                Automate_non_epsilon->transitions[l].etat_arrive = Automate1->transitions[m].etat_arrive;
                Automate_non_epsilon->transitions[l].etiquet = Automate1->transitions[m].etiquet;
                l++;

            }
        }


        with_ep[0]=Etats[i];
        nbr_with_ep=1;
        etatt=Etats[i];

        for (int j=0; j<nbr_lignes_e - 2; j++) {

            if ( Automate1->transitions[j].etat_depart==etatt && Automate1->transitions[j].etiquet=='$' ) {

                with_ep[nbr_with_ep++]=Automate1->transitions[j].etat_arrive;

                etatt=Automate1->transitions[j].etat_arrive;


            }



        }
        printf("\n");
        for (int f=0; f<nbr_with_ep ; f++) {
            printf("    %d   ", with_ep[f]);
        }

        printf("\n");






        for (int d=1 ; d<nbr_with_ep; d++) {

            for (a=0; a<nbr_etats_finaux; a++) {   // etats finaux

                if (with_ep[d]==Automate1->etat_final[a]) {
                    Automate_non_epsilon->etat_final[nbr_etats_finaux_e]=with_ep[0];
                    nbr_etats_finaux_e++;

                }
            }

            for (int k=0; k<nbr_lignes_e - 2; k++) {   // transitions

                if ( (Automate1->transitions[k].etat_depart == with_ep[d] && Automate1->transitions[k].etiquet!='$')  ) {

                    Automate_non_epsilon->transitions[l].etat_depart = with_ep[0];
                    Automate_non_epsilon->transitions[l].etat_arrive = Automate1->transitions[k].etat_arrive;
                    Automate_non_epsilon->transitions[l].etiquet = Automate1->transitions[k].etiquet;

                    l++;

                }

            }
        }
    }



    nbr_lignes_e = l + 2;

}

// fonction pour determiniser un automate
void deterministe(Automate Automate1,Automate *Automate_deterministe){
    struct Groupe{
        int groupe[10];
        int nombre_dans_groupe;
    };
    struct Groupe liste_groupe[30];
    for (int i=0;i<nbr_etats_initiaux;i++){
        liste_groupe[0].groupe[i]=Automate1.etat_initial[i];
    }
    liste_groupe[0].nombre_dans_groupe=nbr_etats_initiaux;

    Automate_deterministe->transitions=(struct Transition*) malloc(100 * sizeof(struct Transition));
    int nombre_groupe=1;
    int nombre_transition=0;
    char caractere_actuel;
    int etat_actuel_d,etat_actuel_a;
    //la loop qui loop groupe par groupe
    for(int i=0;i<nombre_groupe;i++) {
        for (int j = 0; j < nombre_alphabet; j++) {
            caractere_actuel = Alphabet[j];
            bool ajout = false;
            for (int k = 0; k < liste_groupe[i].nombre_dans_groupe; k++) {
                etat_actuel_d = liste_groupe[i].groupe[k];
                for (int k = 0; k < nbr_lignes - 2; k++) {
                    if (Automate1.transitions[k].etiquet == caractere_actuel &&
                        Automate1.transitions[k].etat_depart == etat_actuel_d) {
                        etat_actuel_a = Automate1.transitions[k].etat_arrive;
                        bool repete=false;
                        for(int t=0;t<liste_groupe[nombre_groupe].nombre_dans_groupe;t++){
                            if(liste_groupe[nombre_groupe].groupe[t]==etat_actuel_a){
                                repete=true;
                            }

                        }
                        if (repete==false) {
                            liste_groupe[nombre_groupe].groupe[liste_groupe[nombre_groupe].nombre_dans_groupe] = etat_actuel_a;
                            liste_groupe[nombre_groupe].nombre_dans_groupe++;
                        }
                        ajout = true;
                    }
                }
            }
            for(int i=0;i<liste_groupe[nombre_groupe].nombre_dans_groupe;i++){
                for(int j=0;j<liste_groupe[nombre_groupe].nombre_dans_groupe-1;j++){
                    if(liste_groupe[nombre_groupe].groupe[j]>liste_groupe[nombre_groupe].groupe[j+1]){
                        int tmp=liste_groupe[nombre_groupe].groupe[j];
                        liste_groupe[nombre_groupe].groupe[j]=liste_groupe[nombre_groupe].groupe[j+1];
                        liste_groupe[nombre_groupe].groupe[j+1]=tmp;
                    }
                }
            }
            bool exist = false;
            bool att=false;
            int le_groupe;
            for (int l = 0; l < nombre_groupe; l++) {
                if (liste_groupe[l].nombre_dans_groupe == liste_groupe[nombre_groupe].nombre_dans_groupe) {
                    exist = true;
                    for (int a = 0; a < liste_groupe[nombre_groupe].nombre_dans_groupe; a++) {
                        if (liste_groupe[l].groupe[a] != liste_groupe[nombre_groupe].groupe[a]) {
                            exist = false;
                            break;
                        }
                    }
                    if (exist==true){
                        att=true;
                        le_groupe=l;
                    }

                }
            }
            if (att==true) {
                liste_groupe[nombre_groupe].nombre_dans_groupe = 0;
                Automate_deterministe->transitions[nombre_transition].etat_depart=i;
                Automate_deterministe->transitions[nombre_transition].etat_arrive=le_groupe;
                Automate_deterministe->transitions[nombre_transition].etiquet=caractere_actuel;
                nombre_transition=nombre_transition+1;

            }

            if (ajout == true && att==false) {
                Automate_deterministe->transitions[nombre_transition].etat_depart=i;
                Automate_deterministe->transitions[nombre_transition].etat_arrive=nombre_groupe;
                Automate_deterministe->transitions[nombre_transition].etiquet=caractere_actuel;
                nombre_transition=nombre_transition+1;
                nombre_groupe = nombre_groupe + 1;
            }
        }
    }

    Automate_deterministe->etat_initial[0]=0;
    nbr_etats_initiaux_deterministe=1;
    for(int k=0;k<nombre_groupe;k++){
        for(int i=0;i<nbr_etats_finaux;i++){
            for(int j=0;j<liste_groupe[k].nombre_dans_groupe;j++){
                if(liste_groupe[k].groupe[j]==Automate1.etat_final[i]){
                    Automate_deterministe->etat_final[nbre_etats_fianux_deterministe]=k;
                    nbre_etats_fianux_deterministe++;
                }
            }
        }
    }
    nbr_lignes_deterministe=nombre_transition+2;


}


void deterministe1(Automate Automate2,Automate *Automate_deterministe1){
    struct Groupe1{
        int groupe1[10];
        int nombre_dans_groupe1;
    };
    struct Groupe1 liste_groupe1[30];

    for(int i=0;i<30;i++){
        liste_groupe1[i].nombre_dans_groupe1=0;
        for(int j=0;j<10;j++){
            liste_groupe1[i].groupe1[j]=999;
        }
    }

    for (int i=0;i<nbr_etats_initiaux;i++){
        liste_groupe1[0].groupe1[i]=Automate2.etat_initial[i];
    }
    liste_groupe1[0].nombre_dans_groupe1=nbr_etats_initiaux;

    Automate_deterministe1->transitions=(struct Transition*) malloc(100 * sizeof(struct Transition));
    int nombre_groupe1=1;
    int nombre_transition1=0;
    char caractere_actuel1;
    int etat_actuel_d1,etat_actuel_a1;
    liste_groupe1[1].nombre_dans_groupe1=0;
    for(int i=0;i<nombre_groupe1;i++) {
        for (int j = 0; j < nombre_alphabet; j++) {
            caractere_actuel1 = Alphabet[j];
            bool ajout = false;
            //la loop element dans groupe
            for (int k = 0; k < liste_groupe1[i].nombre_dans_groupe1; k++) {
                etat_actuel_d1 = liste_groupe1[i].groupe1[k];
                for (int k = 0; k < nbr_lignes - 2; k++) {
                    if (Automate2.transitions[k].etiquet == caractere_actuel1 &&
                        Automate2.transitions[k].etat_depart == etat_actuel_d1) {
                        etat_actuel_a1 = Automate2.transitions[k].etat_arrive;
                        bool repete=false;
                        for(int t=0;t<liste_groupe1[nombre_groupe1].nombre_dans_groupe1;t++){
                            if(liste_groupe1[nombre_groupe1].groupe1[t]==etat_actuel_a1){
                                repete=true;
                            }

                        }
                        if (repete==false) {
                            liste_groupe1[nombre_groupe1].groupe1[liste_groupe1[nombre_groupe1].nombre_dans_groupe1] = etat_actuel_a1;
                            liste_groupe1[nombre_groupe1].nombre_dans_groupe1++;
                        }
                        ajout = true;
                    }
                }
            }
            for(int i=0;i<liste_groupe1[nombre_groupe1].nombre_dans_groupe1;i++){
                for(int j=0;j<liste_groupe1[nombre_groupe1].nombre_dans_groupe1-1;j++){
                    if(liste_groupe1[nombre_groupe1].groupe1[j]>liste_groupe1[nombre_groupe1].groupe1[j+1]){
                        int tmp=liste_groupe1[nombre_groupe1].groupe1[j];
                        liste_groupe1[nombre_groupe1].groupe1[j]=liste_groupe1[nombre_groupe1].groupe1[j+1];
                        liste_groupe1[nombre_groupe1].groupe1[j+1]=tmp;
                    }
                }
            }
            bool exist = false;
            bool att=false;
            int le_groupe;
            for (int l = 0; l < nombre_groupe1; l++) {
                if (liste_groupe1[l].nombre_dans_groupe1 == liste_groupe1[nombre_groupe1].nombre_dans_groupe1) {
                    exist = true;
                    for (int a = 0; a < liste_groupe1[nombre_groupe1].nombre_dans_groupe1; a++) {
                        if (liste_groupe1[l].groupe1[a] != liste_groupe1[nombre_groupe1].groupe1[a]) {
                            exist = false;
                            break;
                        }
                    }
                    if (exist==true){
                        att=true;
                        le_groupe=l;
                    }

                }
            }
            if (att==true) {
                liste_groupe1[nombre_groupe1].nombre_dans_groupe1 = 0;
                Automate_deterministe1->transitions[nombre_transition1].etat_depart=i;
                Automate_deterministe1->transitions[nombre_transition1].etat_arrive=le_groupe;
                Automate_deterministe1->transitions[nombre_transition1].etiquet=caractere_actuel1;
                nombre_transition1=nombre_transition1+1;

            }

            if (ajout == true && att==false) {
                Automate_deterministe1->transitions[nombre_transition1].etat_depart=i;
                Automate_deterministe1->transitions[nombre_transition1].etat_arrive=nombre_groupe1;
                Automate_deterministe1->transitions[nombre_transition1].etiquet=caractere_actuel1;
                nombre_transition1=nombre_transition1+1;
                nombre_groupe1 = nombre_groupe1 + 1;
            }
        }
    }


    Automate_deterministe1->etat_initial[0]=0;
    nbr_etats_initiaux_deterministe1=1;
    for(int k=0;k<nombre_groupe1;k++){
        for(int i=0;i<nbr_etats_finaux;i++){
            for(int j=0;j<liste_groupe1[k].nombre_dans_groupe1;j++){
                if(liste_groupe1[k].groupe1[j]==Automate2.etat_final[i]){
                    Automate_deterministe1->etat_final[nbre_etats_fianux_deterministe1]=k;
                    nbre_etats_fianux_deterministe1++;
                }
            }
        }
    }
    nbr_lignes_deterministe1=nombre_transition1+2;


}

// fonction pour inverser un automate
Automate inverser(Automate Automate1){
    Automate Automate_inv;
    nbr_lignes_inverse1=nbr_lignes;
    Automate_inv.transitions=(struct Transition*) malloc(nbr_lignes_inverse1 * sizeof(struct Transition));

    for(int i=0;i<nbr_lignes_inverse1 -2 ;i++){
        Automate_inv.transitions[i].etat_depart=Automate1.transitions[i].etat_arrive;
        Automate_inv.transitions[i].etat_arrive=Automate1.transitions[i].etat_depart;
        Automate_inv.transitions[i].etiquet=Automate1.transitions[i].etiquet;
    }
    nbr_etats_initiaux_inverse1=nbr_etats_finaux;
    nbr_etats_finaux_inverse1=nbr_etats_initiaux;
    for(int j=0;j<nbr_etats_initiaux_inverse1;j++){
        Automate_inv.etat_initial[j]=Automate1.etat_final[j];
    }
    for(int k=0;k<nbr_etats_finaux_inverse1;k++){
        Automate_inv.etat_final[k]=Automate1.etat_initial[k];
    }

    return Automate_inv;

}

// fonction pour minimiser un automate
Automate minimiser(Automate Automate1){
    Automate Automateinv1;
    Automateinv1=inverser(Automate1);
    nbr_lignes=nbr_lignes_inverse1;
    nbr_etats_initiaux=nbr_etats_initiaux_inverse1;
    nbr_etats_finaux=nbr_etats_finaux_inverse1;
    deterministe(Automateinv1,&Automate_d1);

    nbr_lignes=nbr_lignes_deterministe;
    nbr_etats_initiaux=nbr_etats_initiaux_deterministe;
    nbr_etats_finaux=nbre_etats_fianux_deterministe;
    struct Automate Automateinv2;
    Automateinv2=inverser(Automate_d1);
    nbr_etats_initiaux=nbr_etats_initiaux_inverse1;
    nbr_etats_finaux=nbr_etats_finaux_inverse1;
    nbr_lignes=nbr_lignes_inverse1;
    deterministe1(Automateinv2,&Automate_d2);

    return Automate_d2;

}


int main() {
    Automate Automate_non_epsilon;
    Automate Automate1;
    Automate Automate;
    char nom_fichier[20];
    printf("\n   Indiquer le nom du fichier ou' se trouve l'automate  :  ");
    scanf("%s", nom_fichier);
    lire_fichier(nom_fichier, &Automate);
    char nom_fichier_mots[20];
    afficher_alphabets(Automate, nbr_lignes);

    int choix;
//afficher le menu
    do {
        printf("\n\t1- Affichage de l'automate \n");
        printf("\t2- Generer un .dot\n\n");
        printf("\t3- Tester un mot\n");
        printf("\t4- Tester un groupe de mots\n\n");

        printf("\t5- L'etoile d'un automate\n");
        printf("\t6- L'union avec un autre automate\n");
        printf("\t7- Le produit avec un autre automate\n");
        printf("\t8- Supprimer les epsilons\n");
        printf("\t9- Determiniser l'automate\n");
        printf("\t10- Minimiser l'automate\n\n");

        printf("\t11- Tester un autre automate\n");
        printf("\t12- Quitter\n\n");
        printf("\n\t\tVotre choix  :  ");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                afficher_transitions(Automate, nbr_lignes);
                afficher_etats_initiaux_finaux(Automate, nbr_etats_initiaux, nbr_etats_finaux);
                afficher_alphabets(Automate, nbr_lignes);
                generer_fichier_dot(Automate, nbr_etats_initiaux, nbr_etats_finaux, nbr_lignes);
                break;
            case 2:
                generer_fichier_dot(Automate, nbr_etats_initiaux, nbr_etats_finaux, nbr_lignes);
                break;
            case 3:
                printf("\n\tVeuillez saisir un mot  :  ");
                char mots[100];
                scanf(" %s", &mots);
                tester_mot(mots, Automate);
                break;
            case 4:
                printf("\n   Indiquer le nom du fichier des mots  :  ");
                scanf(" %s", &nom_fichier_mots);
                tester_fichhier_mots(nom_fichier_mots, Automate);
                break;
            case 5:
                etoile_automate(Automate, &Automate_et);
                afficher_transitions(Automate_et, nbr_lignes_etoile + 2);
                generer_fichier_dot(Automate_et, nbr_etats_initiaux_etoile, nbr_etats_finaux_etoile,
                                    nbr_lignes_etoile + 2);
                break;
            case 6:
                char nom_fichier1[20];
                printf("\n   Indiquer le nom du fichier ou' se trouve l'automate  :  ");
                scanf("%s", nom_fichier1);
                Union_automate(nom_fichier1, Automate, &Automate2, &Automate_u);
                afficher_transitions(Automate_u, nbr_lignes_union);
                afficher_alphabets(Automate_u, nbr_lignes_union);
                generer_fichier_dot(Automate_u, nbr_etats_initiaux_union, nbr_etats_finaux_union, nbr_lignes_union);
                break;

            case 7:
                char nom_fichier2[20];
                printf("\n   Indiquer le nom du fichier ou' se trouve l'automate  :  ");
                scanf("%s", nom_fichier2);
                Produit_automate(nom_fichier2, Automate, &Automate3, &Automate_produit);
                afficher_transitions(Automate_produit, nbr_lignes_produit);
                printf("k");
                generer_fichier_dot(Automate_produit, nbr_etats_initiaux_produit, nbr_etats_finaux_produit,
                                    nbr_lignes_produit);
                break;
            case 8:
                Epsilon(nom_fichier, &Automate1, &Automate_non_epsilon);
                afficher_transitions(Automate_non_epsilon, nbr_lignes_e);
                afficher_etats_initiaux_finaux(Automate_non_epsilon, nbr_etats_initiaux_e, nbr_etats_finaux_e);
                afficher_alphabets(Automate_non_epsilon, nbr_lignes_e);
                generer_fichier_dot(Automate_non_epsilon, nbr_etats_initiaux_e, nbr_etats_finaux_e, nbr_lignes_e);
                break;
            case 9:
                //printf("b");
                deterministe1(Automate, &Automate_d);
                generer_fichier_dot(Automate_d,nbre_etats_fianux_deterministe1,nbre_etats_fianux_deterministe1,nbr_lignes_deterministe1);
                afficher_transitions(Automate_d,nbr_lignes_deterministe1);
                afficher_etats_initiaux_finaux(Automate_d,nbr_etats_initiaux_deterministe1,nbre_etats_fianux_deterministe1);
                break;
            case 10:
                struct Automate Automatemin1;
                Automatemin1=minimiser(Automate);
                afficher_transitions(Automatemin1,nbr_lignes_deterministe1);
                afficher_etats_initiaux_finaux(Automatemin1,nbr_etats_initiaux_deterministe1,nbre_etats_fianux_deterministe1);
                generer_fichier_dot(Automatemin1,nbr_etats_initiaux_deterministe1,nbre_etats_fianux_deterministe1,nbr_lignes_deterministe1);
                break;
            case 11:
                system("cls");
                printf("\n   Indiquer le nom du fichier ou' se trouve l'automate  :  ");
                scanf("%s", nom_fichier);
                lire_fichier(nom_fichier, &Automate);

                break;
            case 12:
                printf("\n\t\t\t----------------- Merci -----------------\n\n\n\n");
                exit(0);
                break;
            default:
                afficher_transitions(Automate, nbr_lignes);
                afficher_etats_initiaux_finaux(Automate, nbr_etats_initiaux, nbr_etats_finaux);
                afficher_alphabets(Automate, nbr_lignes);
                generer_fichier_dot(Automate, nbr_etats_initiaux, nbr_etats_finaux, nbr_lignes);
                break;
        }
    } while (choix != 12);
    return 0;
}
