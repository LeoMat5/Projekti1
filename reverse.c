#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////////////////////
// HUOM. Kommentointi on suomeksi, kun taas ohjelman toiminnallisuus on englanniksi.//
//////////////////////////////////////////////////////////////////////////////////////

// Aliohjelmat //

// Struct //
typedef struct nodeList {
    char *sRow;
    struct nodeList *pNext;
} LinkedListNode;



void reallocateMemory() {
    printf("Memory reallocated to linked list.\n");
}

void freeMemory () {
    printf("Memory freed from linked list.\n");
}

// Sisällön kääntöön tarkoitettu apuohjelma, parametreina ohjelman ajossa annetut argumentit.
void reverse(char *read_InputFileName, char *write_OutputFileName) {
    // Alustus //
    LinkedListNode *pNew = NULL, *ptr = NULL; // C-kielen kurssilla opetettiin alustamaan näin. 
    FILE *fWrite = NULL;
    int listSize = 0;

    // Varataan muistia Linked Listille
    if ((pNew = (LinkedListNode *)malloc(sizeof(LinkedListNode))) == NULL) {
        fprintf(stderr, "malloc failed\n");
        return(1);
    }
    printf("Memory allocated to linked list.\n");

    // Avataan tiedosto ja määritellään käsittelymuodoski "w" eri write
    if ((fWrite = fopen(read_InputFileName, "w")) == NULL) {
        fprintf(stderr, "error: cannot open file '%s'\n", read_InputFileName);
        exit(1);
    }

    printf("File opened\n");


    fclose(read_InputFileName);
    printf("File closed\n");
    return;
}

// argc on argumenttien lukumäärä ja char *argv[] ovat komentorivistä syötteitä.
int main(int argc, char *argv[]) {
    // Kirjoittelen tämmöisiä toiminnallisuudesta kertovia printtejä eri paikkoihin
    // Helpompi ja nopeampi sitten löytää ja korjata virheet 
    printf("Program started correctly\n");

    // Alustus //
    char *inputFileName = NULL; // Ei saanut olettaa pituuksia niin ei voi käyttää esim pituutta 256
    char *outputFileName = NULL;

    if (argc > 1) {
        // puts(argv[1]);
        // puts(argv[2]);

        // Ensimmäiseksi tarkistan ei sallitut asiat
        if (argc > 3) { // Liian monta argumenttia
            fprintf(stderr, "usage: reverse <input> <output>\n");
            return(1);
        } else if (argc == 1) { // Käydään argumenttien määrä yksitellen läpi
            printf("No input or output file names given, using stdin and stdout in later phase");
        } else if (argc == 2) { // Syötetiedoston nimi annettu
            inputFileName = argv[1];
        } else if (argc == 3) { // Tässä vaiheessa on molemmat argv[1](syötetiedosto) ja argv[2](tulostiedosto) määritelty, joten niitä voi vasta tässä verrata
            if (strcmp(argv[1], argv[2]) == 0) { // Samat nimet, ei jatkoon
                fprintf(stderr, "Input and output file must differ\n");
                return(1);
            } else { // Eri nimet, jatkoon
                inputFileName = argv[1];
                outputFileName = argv[2];
            }
        }

        reverse(inputFileName, outputFileName);
       
    } else {
        fprintf(stderr, "You didn't give enough arguments\n");
        return(1);
    }

    // Teen opetetun mukaisesti myös näitä printtejä:
    printf("Thank you for the program usage.\n");
    return(0); // C-kielen kurssin tyyliohjeessa sanotaan aina palauttaa jotakin ja suluissa
}