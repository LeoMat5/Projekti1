//////////////////////////////////////////////////////////////////////////////////////
// HUOM. Kommentointi on suomeksi, kun taas ohjelman toiminnallisuus on englanniksi.//
// Lisäilin myös omia ominaisuuksia tehtävänannon lisäksi, kuten käyttäjän opastus. //
//////////////////////////////////////////////////////////////////////////////////////

// https://linux-tips.com/t/hard-link-files/125
// https://www.baeldung.com/linux/file-hard-soft-link-test

// Määrittelyt //
#define _POSIX_C_SOURCE 200809L // Kopioitu man getline -sivulta

// Importatut kirjastot //
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

// Struct //
typedef struct nodeList {
    char *sRow;
    struct nodeList *pNext;
} LinkedListNode;

// Apuohjelmat //

// Sisällön kääntöön tarkoitettu apuohjelma, parametreina ohjelman ajossa annetut argumentit.
void reverse(char *pRead_InputFileName, char *pWrite_OutputFileName) {

    // Alustus //
    LinkedListNode *pNew = NULL, *ptr = NULL, *pListStart = NULL; // C-kielen kurssilla opetettiin alustamaan näin. 
    FILE *fRead = NULL; // C-kielen kurssilla opetettiin tekemään näin, vaikka se on vähän hassua kirjoittaa kaksi alustusta
    fRead = stdin;      // Syy on se, että jos sitä ei ensin alusteta NULLiksi, sinne saattaa jäädä jotakin väärin.
    FILE *fWrite = NULL;
    fWrite = stdout;
    char *pRow = NULL;  // Osoitin riveihin
    size_t len = 0;     // getline() vaatima puskurin koko -muuttuja
    ssize_t amountOfReadSymbols; // Luettujen symbolien määrä, -1 tarkoittaa että EOF saavutettu lukematta mitään tai voi tarkoittaa myös virhettä.

    // Tarkistetaan, onko input tiedoston nimeä annettu
    if (pRead_InputFileName != NULL) { // Jos on annettu, sitä käytetään, muuten stdin.
        if ((fRead = fopen(pRead_InputFileName, "r")) == NULL) { // Tämä on myös C-kielen kurssin tyyli avata tiedosto
            fprintf(stderr, "reverse: cannot open file '%s'\n", pRead_InputFileName); // Huom! Tehtävänannossa on väärin "error: ", testissä on "reverse: "
            exit(1);
        }

        // printf("Read file opened\n");
        
    } // else {  Ohjeistus jos ensimmäistä argumenttia (input tiedosto) ei annettu. // TESTIEN TAKIA KOMMENTOITU POIS
        // printf("\n");
        // printf("INSTRUCTIONS:\n");
        // printf("No input file given, write each text row one by one, pressing Enter after each.\n");
        // printf("When you want to continue, press ctrl + D.\n\n");
    //}

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
        
        pNew->sRow = pRow; // Tallennetaan Nodeen merkkijonon osoitin. Merkkijonothan ovat muistissa symboli kerrallaan vierekkäin, joten tarvitsee vain ensimmäisen osoitteen.
        pNew->pNext = ptr; // Osoittimen päivitys.
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

    // Otetaan listan alkuosoite talteen.
    pListStart = ptr;

    // Linkitetystä listasta lukeminen joko näytölle tai tiedostoon.

    if (pWrite_OutputFileName != NULL) { // Tarkistetaan onko toista argumenttia annettu (read tiedoston nimi)
         if ((fWrite = fopen(pWrite_OutputFileName, "w")) == NULL) {
            fprintf(stderr, "reverse: cannot open file '%s'\n", pWrite_OutputFileName); // Huom! Tehtävänannossa on väärin "error: ", testissä on "reverse: "
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
        // fprintf(stdout, "%s", "\nNo write file name given, writing to the screen.\nIn reverse the given rows are:\n\n"); // TESTIEN TAKIA KOMMENTOITU POIS.
        while (ptr != NULL) {
            fprintf(stdout, "%s", ptr->sRow);
            ptr = ptr->pNext;
        }
    }

    // Linkitetyn listan muistin vapautus.
    LinkedListNode *pNext = NULL; // Avustin osoitin.  
    while (pListStart != NULL) {
        pNext = pListStart->pNext; // Structista otetaan ptr->pNext säilöön, ettei muistin vapautuksessa tarvittavat asiat katoa.
        free(pListStart->sRow);    // Vapautetaan muisti pointterin osoittamasta merkkijonosta
        free(pListStart);   // Ja sitten myös pointterilta.
        pListStart = pNext; // Siirretään ptr osoittamaan aiemmin tallennettuun pNextiin, eli seuraavan noden osoitteeseen siirtyminen.
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
    struct stat inputFile; // https://linux-tips.com/t/hard-link-files/125
    struct stat outputFile;

    // Ensimmäiseksi tarkistan ei sallitut asiat
    if (argc >= 1) {
        if (argc > 3) { // Liian monta argumenttia
            fprintf(stderr, "usage: reverse <input> <output>\n");
            return(1);
        } //else if (argc == 1) { // Käydään argumenttien määrä yksitellen läpi             // Testit ei mene läpi jos laajentaa omia juttuja.
           // printf("No input or output file names given, using stdin and stdout in later phase.\n");
        //}
         else if (argc == 2) { // Syötetiedoston nimi annettu
            inputFileName = argv[1];
        } else if (argc == 3) { // Tässä vaiheessa on molemmat argv[1](syötetiedosto) ja argv[2](tulostiedosto) määritelty, joten niitä voi vasta tässä verrata
            // Otetaan argumenteista nimet talteen.
            inputFileName = argv[1];
            outputFileName = argv[2];
            
            if (stat(argv[1], &inputFile) == 0 && stat(argv[2], &outputFile) == 0) { // https://linux-tips.com/t/hard-link-files/125
                if (inputFile.st_ino == outputFile.st_ino) { // Samat tiedostot, ei jatkoon. Tässä on myös käytetty. Ino on inode numero.
                    fprintf(stderr, "reverse: input and output file must differ\n");
                    return(1);
                }
            }
        }
        reverse(inputFileName, outputFileName);
       
    } else {
        fprintf(stderr, "You didn't give enough arguments\n");
        return(1);
    }

    // Teen opetetun mukaisesti myös näitä printtejä:
    // printf("\n");                                    // Ei näköjään testit mene läpi näillä.
    // printf("Thank you for program use.\n");
    return(0); // C-kielen kurssin tyyliohjeessa sanotaan aina palauttaa jotakin ja suluissa
}