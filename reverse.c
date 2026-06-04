//////////////////////////////////////////////////////////////////////////////////////
// HUOM. Kommentointi on suomeksi, kun taas ohjelman toiminnallisuus on englanniksi.//
//////////////////////////////////////////////////////////////////////////////////////

// Määrittelyt //
#define _POSIX_C_SOURCE 200809L // Kopioitu man getline -sivulta

// Importatut kirjastot //
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Struct //
typedef struct nodeList {
    char *sRow;
    struct nodeList *pNext;
} LinkedListNode;

// Apuohjelmat //

// Muistin vapautus apuohjelma.
void freeMemory() {
    return;
}

// Sisällön kääntöön tarkoitettu apuohjelma, parametreina ohjelman ajossa annetut argumentit.
void reverse(char *pRead_InputFileName, char *pWrite_OutputFileName) {

    // Alustus //
    LinkedListNode *pNew = NULL, *ptr = NULL; // C-kielen kurssilla opetettiin alustamaan näin. 
    FILE *fRead = NULL; // C-kielen kurssilla opetettiin tekemään näin, vaikka se on vähän hassua kirjoittaa kaksi alustusta
    fRead = stdin;      // Syy on se, että jos sitä ei ensin alusteta NULLiksi, sinne saattaa jäädä jotakin väärin.
    FILE *fWrite = NULL;
    fWrite = stdout;
    char *pRow = NULL;
    size_t len = 0;
    size_t amountOfReadSymbols = 0;

    // Tarkistetaan, onko input tiedoston nimeä annettu
    if (pRead_InputFileName != NULL) { // Jos on annettu, sitä käytetään, muuten stdin.
        if ((fRead = fopen(pRead_InputFileName, "r")) == NULL) { // Tämä on myös C-kielen kurssin tyyli avata tiedosto
            fprintf(stderr, "error: cannot open file '%s'\n", pRead_InputFileName);
            exit(1);
        }

        // printf("Read file opened\n");
        
    } else { // Ohjeistus jos ensimmäistä argumenttia (input tiedosto) ei annettu.
        printf("\n");
        printf("INSTRUCTIONS:\n");
        printf("No input file given, write each text row one by one, pressing Enter after each.\n");
        printf("When you want to continue, press ctrl + D.\n\n");
    }

    // Luetaan listaan tiedot joko stdin tai tiestostosta (meille opetettiin vain fgets, piti selvittää tämä getline() erikseen)
    // https://c-for-dummies.com/blog/?p=1112
    // https://stackoverflow.com/questions/74556614/how-to-correctly-use-getline
    // Luin myös "man getline", tämä oli näistä kaikista paras.
    while ((amountOfReadSymbols = getline(&pRow, &len, fRead)) != -1) { // While for:in sijaan, koska ohjeissa sanotaan, ettei saa olettaa mitään tiedoston koosta.
        // Varataan muistia Linked Listan Nodelle
        if ((pNew = (LinkedListNode *)malloc(sizeof(LinkedListNode))) == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }
        
        pNew -> sRow = pRow; // Tallennetaan Nodeen merkkijonon osoitin. Merkkijonothan ovat muistissa symboli kerrallaan vierekkäin, joten tarvitsee vain ensimmäisen osoitteen.
        pNew -> pNext = ptr; // Osoittimen päivitys.
        ptr = pNew;          // Tämä on LIFO (Last In First Out), joka kääntää järjestyksen. Olisi voinut myös käyttää pPrevious pointteria.
        
        // Alustus seuraavalle kierrokselle
        pRow = NULL;
        len = 0;
    }

    // Muistin vapautus ja luetun tiedoston sulkeminen.
    free(pRow);
    // printf("Row pointer memory freed\n");
    fclose(fRead);
    // printf("Read file closed\n");

    // Linkitetystä listasta lukeminen joko näytölle tai tiedostoon.
    if (pWrite_OutputFileName != NULL) { // Tarkistetaan onko toista argumenttia annettu (read tiedoston nimi)
         if ((fWrite = fopen(pWrite_OutputFileName, "w")) == NULL) {
            fprintf(stderr, "error: cannot write to file '%s'\n", pWrite_OutputFileName); // Ei ollut ohjeessa, mutta on tärkeää tehdä kuitenkin.
            exit(1);
        }

        // Kirjoitus tiedostoon.
        while (ptr != NULL) {
        fprintf(fWrite, "%s", ptr->sRow);
        ptr = ptr->pNext;
    }

    // Suljetaan kirjoitettava tiedosto.
    fclose(fWrite);

    } else { // Kirjoitettavaa tiedostoa ei annettu, tulostetaan näytölle fprintf käyttäen.
        fprintf(stdout, "%s", "\nIn reverse the given rows are:\n\n");
        while (ptr != NULL) {
            fprintf(stdout, "%s", ptr->sRow);
            ptr = ptr->pNext;
        }
    }


    return;
}

// argc on argumenttien lukumäärä ja char *argv[] ovat komentorivistä syötteitä.
int main(int argc, char *argv[]) {
    // Kirjoittelen tämmöisiä toiminnallisuudesta kertovia printtejä eri paikkoihin
    // Helpompi ja nopeampi sitten löytää ja korjata virheet 
    // printf("Program started correctly\n");

    // Alustus //
    char *inputFileName = NULL; // Ei saanut olettaa pituuksia niin ei voi käyttää esim pituutta 256
    char *outputFileName = NULL;

    // Ensimmäiseksi tarkistan ei sallitut asiat
    if (argc >= 1) {
        if (argc > 3) { // Liian monta argumenttia
            fprintf(stderr, "usage: reverse <input> <output>\n");
            return(1);
        } else if (argc == 1) { // Käydään argumenttien määrä yksitellen läpi
            printf("No input or output file names given, using stdin and stdout in later phase.\n");
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
    printf("\n");
    printf("Thank you for program use.\n");
    return(0); // C-kielen kurssin tyyliohjeessa sanotaan aina palauttaa jotakin ja suluissa
}